#ifndef PERSPECTIVE_CORRECTION_H
#define PERSPECTIVE_CORRECTION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <functional>

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
    Finds the corners by first detecting all lines in the image, then iterating all end points.
    This method uses the hough lines implementation by openCV, and is slightly more robust against noise.
    **/
    bool findCornersHoughLines(cv::Mat &bw, std::vector<cv::Point2f> &corners, cv::Mat &annotated);

    /**
    Performs a perspective transform on the source image and maps it over to the destination image
    **/
    bool doTransform(cv::Mat &transmtx,cv::Mat &src, cv::Mat &dst);

    /**
    A function for spiraling out from a point. (Square not circling)
    delta s the maximum distance to go out in x and y directions

    Func should return true if the count should increase
    **/
    int countSpiralOut(int centerX, int centerY, int delta, std::function<bool (const int x, const int y)> func);

    cv::Mat src;
    cv::Mat result;
};

#endif
