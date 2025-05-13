#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

// int constrastValue, brightValue;
// cv::Mat image;
// cv::Mat stretch_image;
// int minValue = 0, maxValue = 0;
void drawKeyPts(
    cv::Mat &image, const std::vector<cv::KeyPoint> &pts, int size, const  cv::Scalar color) {
    for (int i = 0; i < pts.size(); ++i) {
        // cv::circle(image, pts[i].pt, color, );
        // cv::imshow("key", image);
        // cv::waitKey();
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
		// if (
		// 	knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance ||
		// 	knn_matches[r][0].distance > 5 * std::max(min_dist, 10.0f)
		// 	)
		// 	continue;

		//保存匹配点
		matches.push_back(knn_matches[r][0]);
	}
    std::cout << min_dist << "\n";
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
    cv::Mat image1 = cv::imread("/home/wz/test/testopencv/src/876588627.jpg");
    cv::Mat image2 = cv::imread("/home/wz/test/testopencv/src/1444550589.jpg");
     cv::Mat left_resize, right_resize;
    left_resize = image2.clone();
    left_resize.setTo(0);
    image1.copyTo(left_resize(cv::Rect(0, 0, image1.cols, image1.rows)));
    cv::Ptr<cv::Feature2D> sift = cv::xfeatures2d::SIFT::create();
    cv::Mat descriptor_L, descriptor_R;
    std::vector<cv::KeyPoint> key_points_L, key_points_R; 
    sift->detectAndCompute(image1, cv::noArray(), key_points_L, descriptor_L, false);
    sift->detectAndCompute(image2, cv::noArray(), key_points_R, descriptor_R, false);
    std::vector<cv::DMatch> matches = {};
    match_features(descriptor_L, descriptor_R, matches);
    cv::Mat match_image;
    // left_resize = image2.clone();
    // left_resize.setTo(0);
    // image1.copyTo(left_resize(cv::Rect(0, 0, image1.cols, image1.rows)));
    cv::drawKeypoints(left_resize, key_points_L, left_resize, cv::Scalar(0, 255, 0));
    cv::drawKeypoints(image2, key_points_R, image2, cv::Scalar(0, 255, 0));
    drawMatchPts(left_resize, image2, match_image, key_points_L, key_points_R, matches, true);
    // cv::drawMatches(left_resize, key_points_L, image2, key_points_R, matches, match_image, cv::Scalar(0, 255, 0));
    cv::imshow("match_image", match_image);
    cv::imshow("left_resize", left_resize);
    cv::imshow("image2", image2);
    cv::imwrite("/home/wz/test/testopencv/output/match1.png", match_image);
    cv::waitKey(0);
    return 0;
}