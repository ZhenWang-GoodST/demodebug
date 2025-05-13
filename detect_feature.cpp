#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "std_time.h"

// int constrastValue, brightValue;
// cv::Mat image;
// cv::Mat stretch_image;
// int minValue = 0, maxValue = 0;
void filtBySize(std::vector<cv::KeyPoint> &key_points, int size, int mode = 1) {
    std::vector<cv::KeyPoint> pt_filted = {};
    for (int i = 0; i < key_points.size(); ++i) {
        if (key_points[i].size * mode < size * mode) continue;
        pt_filted.push_back(key_points[i]);
    }
    key_points.clear();
    key_points = std::move(pt_filted);
}

void drawKeyPts(
    cv::Mat &image, const std::vector<cv::KeyPoint> &pts, int size, const  cv::Scalar color) {
    double p_size = 0;
    for (int i = 0; i < pts.size(); ++i) {
        p_size = size < 0 ? pts[i].size : size;
        cv::circle(image, pts[i].pt, p_size, color);
        // cv::imshow("image", image);
        // cv::waitKey(0);
    }
}
void drawMatchPts(
    const cv::Mat &left, const cv::Mat &right, cv::Mat &show_image,
    const std::vector<cv::KeyPoint> &p1, const std::vector<cv::KeyPoint> &p2,
    const std::vector<cv::DMatch> &match, bool update) {
    
    int pt_size = p1.size();
    if (update) {
        show_image = cv::Mat::zeros(cv::Size(left.cols + right.cols, left.rows), left.type());
        cv::Rect rect1(cv::Point2f(0, 0), left.size());
        cv::Rect rect2(cv::Point2f(left.cols, 0), left.size());
        left.copyTo(show_image(rect1));
        right.copyTo(show_image(rect2));
        // cv::cvtColor(show_image, show_image, cv::COLOR_GRAY2BGR);
    }
    int size = 5;
    for (int i = 0; i < match.size(); ++i) {
        // if (indices[i] < 0) continue;
        int L = match[i].queryIdx;
        int R = match[i].trainIdx;
        cv::KeyPoint pt1 = p1[L];
        cv::KeyPoint pt2 = p2[R];
        pt2.pt.x += left.cols;
    //    if (mask[i] == 0) {
    //         color = cv::Scalar(0, 0, 255);
    //         if (iscontinue) continue;
    //     } else if (mask[i] == 1){
    //         color = cv::Scalar(0, 255, 0);
    //     } 
        // cv::Scalar color(200 + rand()%55, 128 + rand()%128, 128 + rand()%128);
        cv::Scalar color(rand()%255, rand()%255, rand()%255);
        cv::line(show_image, pt1.pt, pt2.pt, color);
        // cv::rectangle(show_image, cv::Rect(pt1.x - size, pt1.y - size, size * 2, size * 2), color);
        // cv::rectangle(show_image, cv::Rect(pt2.x - size, pt2.y - size, size * 2, size * 2), color);
        cv::circle(show_image, pt1.pt, pt1.size, color);
        cv::circle(show_image, pt2.pt, pt1.size, color);
        // cv::imshow("matched", show_image);
        // cv::waitKey();
    }
}
void match_features(cv::Mat& query, cv::Mat& train, std::vector<cv::DMatch>& matches)
{
	std::vector<std::vector<cv::DMatch>> knn_matches;
	cv::BFMatcher matcher(cv::NORM_L2);
	matcher.knnMatch(query, train, knn_matches, 2);

	//获取满足Ratio Test的最小匹配的距离
	float min_dist = FLT_MAX;
	for (int r = 0; r < knn_matches.size(); ++r)
	{
		//Ratio Test
		if (knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance)
			continue;

		float dist = knn_matches[r][0].distance;
		if (dist < min_dist) min_dist = dist;
	}

	matches.clear();
	for (size_t r = 0; r < knn_matches.size(); ++r)
	{
		//排除不满足Ratio Test的点和匹配距离过大的点
		if (
			knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance ||
			knn_matches[r][0].distance > 5 * std::max(min_dist, 10.0f)
			)
			continue;

		//保存匹配点
		matches.push_back(knn_matches[r][0]);
	}
    std::cout << "min_dis: " << min_dist << "\n";
}

int main(int argc, char** argv)
{
    std::string path;
    if (argc > 1) {
        path = argv[1];
    } else {
        path = "/home/wz/VO-LOAM/data/1004/2021-10-04-08-52-10/camera/color/image_raw1/1633308808.948763.jpg";
    }
    std::cout << "test match\n";
    // cv::Mat image1 = cv::imread("/home/tonglu/test/testopencv/src/1633308767.458915.jpg");
    // cv::Mat image2 = cv::imread("/home/tonglu/test/testopencv/src/1633308767.510798.jpg");
    cv::Mat image1 = cv::imread("/home/tonglu/test/testopencv/src/1633308787.486811.jpg");
    cv::Mat image2 = cv::imread("/home/tonglu/test/testopencv/src/1633308787.546102.jpg");
    cv::Size gau_size(5, 5);
    cv::resize(image1, image1, image1.size() / 2);
    cv::resize(image2, image2, image2.size() / 2);
    cv::GaussianBlur(image1, image1, gau_size, 2);
    cv::GaussianBlur(image2, image2, gau_size, 2);
    cv::Ptr<cv::Feature2D> sift = cv::xfeatures2d::SURF::create(115, 2, 3);
    cv::Mat descriptor_L, descriptor_R;
    cv::KeyPoint p0;
    p0.size = 8;
    std::vector<cv::KeyPoint> key_points_L, key_points_R; 
    StdTime timer;
    // sift->detectAndCompute(image1, cv::noArray(), key_points_L, descriptor_L, false);
    std::cout << (timer[1]) ;
    // sift->detectAndCompute(image2, cv::noArray(), key_points_R, descriptor_R, false);
    sift->detect(image1, key_points_L, cv::noArray());
    std::cout << (timer[1]) ;
    sift->detect(image2, key_points_R, cv::noArray());
    std::cout << (timer[1]) ;
    filtBySize(key_points_L, 35, 1);
    filtBySize(key_points_R, 35, 1);
    std::cout << (timer[1]) ;
    sift->compute(image1, key_points_L, descriptor_L);
    std::cout << (timer[1]) ;
    sift->compute(image2, key_points_R, descriptor_R);
    std::cout << (timer[1]) ;
    // key_points_L.erase(std::remove_if(key_points_L.begin(), key_points_L.end(), [p0](cv::KeyPoint _pt) { return _pt.size < p0.size; }), key_points_L.end());
    // key_points_R.erase(std::remove_if(key_points_R.begin(), key_points_R.end(), [p0](cv::KeyPoint _pt) { return _pt.size < p0.size; }), key_points_R.end());
    std::vector<cv::DMatch> matches = {};
    std::cout << (timer[1]) ;
    match_features(descriptor_L, descriptor_R, matches);
    std::cout << (timer[1]) ;
    cv::Mat match_image;
    // left_resize = image2.clone();
    // left_resize.setTo(0);
    // image1.copyTo(left_resize(cv::Rect(0, 0, image1.cols, image1.rows)));
    drawMatchPts(image1, image2, match_image, key_points_L, key_points_R, matches, true);
    drawKeyPts(image1, key_points_L, -1, cv::Scalar(0, 255, 0));
    drawKeyPts(image2, key_points_R, -1, cv::Scalar(0, 255, 0));
    // cv::drawMatches(left_resize, key_points_L, image2, key_points_R, matches, match_image, cv::Scalar(0, 255, 0));
    cv::imshow("match_image", match_image);
    cv::imshow("image1", image1);
    cv::imshow("image2", image2);
    cv::imwrite("/home/wz/test/testopencv/output/match1.png", match_image);
    cv::waitKey(0);
    return 0;
}