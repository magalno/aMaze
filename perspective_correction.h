#ifndef PERSPECTIVE_CORRECTION_H
#define PERSPECTIVE_CORRECTION_H

#include <opencv2/imgproc/imgproc.hpp>

class PerspectiveCorrection
{
public:
    PerspectiveCorrection(cv::Mat &src);
    ~PerspectiveCorrection();

    bool process();
    cv::Mat getResult();

private:
    /**
    Finds an aproximation of the center point of the maze
    **/
    cv::Point2f findCenter(cv::Mat &bw);
    /**
    Finds the corners of the maze, by finding the points furthest away from the center point.
    They are placed in the corners vector in the order topLeft, topRight, bottomRight, bottomLeft
    **/
    bool findCornes(cv::Mat &bw, cv::Point2f &centerPoint, std::vector<cv::Point2f> &corners);

    /**
    Performs a perspective transform on the source image and maps it over to the destination image
    **/
    bool doTransform(cv::Mat &transmtx,cv::Mat &src, cv::Mat &dst);

    cv::Mat src;
    cv::Mat result;
};

#endif
