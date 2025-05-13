#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

struct rect
{
    double x1, y1, x2, y2;    // first and second point of the line segment
    double width;             // rectangle width
    double x, y;              // center of the rectangle
    double theta;             // angle
    double dx,dy;             // (dx,dy) is vector oriented as the line segment
    double prec;              // tolerance angle
    double p;                 // probability of a point with angle within 'prec'
    void draw(cv::Mat im, cv::Scalar color = cv::Scalar(0, 255, 0), bool setw = false) {
        cv::line(im, cv::Point2f(x1, y1), cv::Point2f(x2, y2), color, setw ? 2:width);
    }
    std::vector<cv::Point> alligned_pts = {};
    std::vector<cv::Point> unalligned_pts = {};
};

void getPixel(rect &rec) {
    
}
int main(int argc, char** argv)
{
    cv::Mat image = cv::Mat::zeros(cv::Size(500, 500), CV_8U);
    rect rec;
    rec.theta = 0;
    rec.x = 250;
    rec.y = 250;
    getPixel(rec);

    return 0;
}