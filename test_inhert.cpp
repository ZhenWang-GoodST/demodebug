#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <set>

class Father {
private:
    std::string pra_str = "publish father";
public:
    std::string pub_str = "publish father";
};

class Son : public Father
{

};

class MyPoint : public cv::KeyPoint
{
private:
    /* data */
public:
    std::string str = "test";
};


int main() {
    std::set<int> set_int;
    set_int.insert(0);
    set_int.insert(1);
    set_int.insert(0);
    
    // cv::Mat image;
    // std::vector<MyPoint> pts = {};
    // pts.push_back(cv::KeyPoint());
    // cv::drawKeypoints(image, pts, image, cv::Scalar(0, 255, 0));
    // std::vector<cv::KeyPoint*> pts_cv = {};
    // cv::KeyPoint pt;
    // pts_cv.push_back(&pt);
    // MyPoint * mpt = dynamic_cast<MyPoint*>(&pt);
    // cv::drawKeypoints(image, pts_cv, image, cv::Scalar(0, 255, 0));



    // Son son;
    // Father father;
    // std::cout << son.pub_str << "\n";
    // std::vector<Son> sons = {};
    // sons.push_back(Son());
    // sons.push_back(dynamic_cast<Son&>(father));
    return 0;
}

