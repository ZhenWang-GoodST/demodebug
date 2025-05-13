#include <iostream>
#include <opencv2/opencv.hpp>
#include <histogram.h>

int constrastValue, brightValue;
cv::Mat image;
cv::Mat stretch_image;
int minValue = 0, maxValue = 0;

template <typename T>
void printMat(const cv::Mat &mat) {
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            std::cout << mat.at<T>(i, j) << " ";
        }
        std::cout << "\n";
    }
}

void printHist(const cv::Mat &hist) {
    for (int i = 0; i < 256; ++i) {
        std::cout << i << " : " << hist.at<float>(0, i) << "\n";
    }
    
}

void adaptiveThreshold(const cv::Mat &hist, double thresh, int &minId, int &maxId) {
    cv::Point minLoc, maxLoc;
    double minV, maxV;
    cv::minMaxLoc(hist, &minV, &maxV, &minLoc, &maxLoc);
    double inlier = maxV;
    double total;
    cv::Scalar output = cv::sum(hist);
    total = output[0];
    minId = maxId = maxLoc.y;
    int minCount = 1000, maxCount = 1000;
    while ((inlier / total) < thresh || minCount > 400 || maxCount > 400) {
    // while ((inlier / total) < thresh) {
        if (minId - 1 < 0 || maxId + 1 > 255) break;
        minCount = hist.at<float>(0, minId - 1);
        maxCount = hist.at<float>(0, maxId + 1);
        if (minCount > maxCount) {
            minId -= 1;
            inlier += minCount; 
        } else {
            maxId += 1;
            inlier += maxCount; 
        }
    }
    std::cout << "final inlier: " << inlier / total << "\n";
}
//直方图拉伸
cv::Mat stretch(const cv::Mat& image, int minvalue = 0, int maxvalue = 255, bool makeFloat = false) {
    Histogram1D h;
    cv::Mat mask;
    cv::threshold(image, mask, 230, 255, cv::ThresholdTypes::THRESH_BINARY_INV);
    // cv::Mat hist = h.getHistogram(image, mask);
    cv::Mat hist = h.getHistogram(image, cv::Mat());
    for (int i = 230; i < 256; ++i) {
        hist.at<float>(0, i) = 0;
    }
    if (minvalue == 0 && maxvalue == 0) {
        adaptiveThreshold(hist, 0.98, minvalue, maxvalue);
    }
    std::cout << "minvalue: " << minvalue << " maxvalue: " << maxvalue << "\n"; 
    cv::Mat fMat, result = cv::Mat::zeros(image.size(), image.type());
    if (makeFloat) {
        image.convertTo(fMat, CV_32F);
        double sigX = 2;
        cv::Size ksize(9, 9);
        int range = maxvalue - minvalue;
        cv::GaussianBlur(fMat, fMat, ksize, sigX);
        for (int i = 0; i < image.rows; ++i) {
            uchar *ip = result.ptr<uchar>(i);
            float *fp = fMat.ptr<float>(i);
            for (int j = 0; j < image.cols; ++j) {
                float fValue = *fp++;
                if ((int)(image.at<uchar>(i, j)) < minvalue) {
                    *ip++ = 0;
                } else if ((int)(image.at<uchar>(i, j)) > maxvalue) {
                    *ip++ = 255;
                } else {
                    *ip++ = 255 * (fValue - minvalue) / range;
                }
            }
            ip = nullptr;
            fp = nullptr;
        }
        return result;
    } else {
        //创建查找表
        cv::Mat lut(1, 256, CV_8U);
        //构建查找表
        for (int i = 0; i < 256;i++) {
            if (i < minvalue)
                lut.at<uchar>(i) = 0;
            else if (i>maxvalue)
                lut.at<uchar>(i) = 255;
            else {
                lut.at<uchar>(i) = cvRound(255.0*(i - minvalue) / (maxvalue - minvalue));
            }
        }
        return Histogram1D::applyLookUp(image,lut);
    }
    return result;
}
int tileGridSize;
int clipLimit = 40.0;
void  imageClahe(int, void *) {
    cv::Mat clahe_image;
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit / 10, cv::Size(tileGridSize, tileGridSize));
    clahe->apply(image, clahe_image);
    cv::imshow("clahe_image", clahe_image);
}
void  imageStretch(int, void *) {
    
    // std::cout << minValue << " " << maxValue << "\n";
    // printMat<uchar>(image);
    // cv::cvtColor(image, image, cv::GRAY2);
    // cv::Size ksize(9, 9);
    // double sigX= 4;
    // printMat<float>(image);
    // cv::GaussianBlur(image, image, ksize, sigX);
    // printMat<float>(image);
    stretch_image = stretch(image, minValue, maxValue, 0);
    Histogram1D h;
    cv::Mat hist = h.getHistogram(stretch_image, cv::Mat());
    cv::Mat stretch_hist = h.getHistogramImage(stretch_image, 1);
    cv::Mat equal_image;
    // cv::equalizeHist(stretch_image, equal_image);
    printHist(hist);
    // cv::imshow("equal_image", equal_image);
    cv::imshow("stretch_image", stretch_image);
    cv::imshow("stretch_hist", stretch_hist);
    // for (int i = 0; i < src.rows; i++) {
    //     for (int j = 0; j < src.cols; j++) {
    //         dst.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(constrastValue*0.01*src.at<Vec3b>
    //             (i, j)[0] + brightValue);
    //         dst.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(constrastValue*0.01*src.at<Vec3b>
    //             (i, j)[1] + brightValue);
    //         dst.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(constrastValue*0.01*src.at<Vec3b>
    //             (i, j)[2] + brightValue);
    //     }
    // }
    // imshow("image", dst);
}

int main(int argc, char** argv)
{
    std::string path;
    if (argc > 1) {
        path = argv[1];
    } else {
        path = "/home/wz/VO-LOAM/data/1004/2021-10-04-08-52-10/camera/color/image_raw1/1633308808.948763.jpg";
    }
    // path = "/home/wz/VO-LOAM/data/1004/2021-10-04-09-02-01/camera/color/image_raw1/1633309386.175266.jpg";
    std::cout << path << "\n";
    cv::namedWindow("stretch_image", cv::WINDOW_NORMAL);
    std::string image_path = path;
    image = cv::imread(image_path);
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    Histogram1D h;
    cv::Mat hist = h.getHistogram(image, cv::Mat());
    cv::Mat origin_hist = h.getHistogramImage(image, 1);
    printHist(hist);
    // image += 100;
    cv::imshow("image", image);
    cv::imshow("origin_hist", origin_hist);
    cv::Mat mask = cv::Mat::zeros(image.size(), image.type());
    cv::threshold(image, mask, 32, 255, cv::ThresholdTypes::THRESH_BINARY);
    cv::imshow("mask", mask);
    // std::cout << hist.type() << "\n";
    imageStretch(0, 0);
    imageClahe(0, 0);
    // cv::Ptr<cv::LineSegmentDetector> detector = cv::createLineSegmentDetector(cv::LSD_REFINE_ADV);
    // cv::Mat show_line, lines;
    // detector->detect(image, lines);
    // show_line = image.clone();
    // detector->drawSegments(show_line, lines);
    // cv::imshow("show_line", show_line);
    // cv::LineSegmentDetector lsd;
    cv::createTrackbar("Max", "stretch_image", &minValue, 255, imageStretch);
    cv::createTrackbar("Min", "stretch_image", &maxValue, 255, imageStretch);
    cv::createTrackbar("clahe_image", "clahe", &clipLimit, 2550, imageClahe);
    cv::createTrackbar("clahe_image", "clahe", &tileGridSize, 255, imageClahe);
    cv::waitKey(0);
    return 0;
}