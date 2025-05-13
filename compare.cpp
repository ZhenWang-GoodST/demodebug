#include <iostream>
#include <opencv2/opencv.hpp>

int constrastValue, brightValue;
cv::Mat image;
cv::Mat stretch_image;
int minValue = 0, maxValue = 0;
int main(int argc, char** argv)
{
    std::string path;
    if (argc > 1) {
        path = argv[1];
    } else {
        path = "/home/wz/VO-LOAM/data/1004/2021-10-04-08-52-10/camera/color/image_raw1/1633308808.948763.jpg";
    }
    cv::Mat image1 = cv::imread("/home/cvrsg/test/demodebug/src/876588627.jpg");
    // cv::Mat image2 = cv::imread("/home/wz/VO-LOAM/github/output/blur_good.png");
    cv::Mat diff;
    path = "test";
    cv::imshow("test", image1);
    cv::waitKey(0);
    return 0;
}