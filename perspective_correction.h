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
    bool doTransform(cv::Mat &transmtx,cv::Mat &src, cv::Mat &dst);

    cv::Mat src;
    cv::Mat result;
};

#endif
