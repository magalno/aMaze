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
    cv::Mat src;
    cv::Mat result;
};

#endif
