#ifndef PERSPECTIVE_CORRECTION_H
#define PERSPECTIVE_CORRECTION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

class Solver
{
public:
    Solver(cv::Mat &src);
    ~Solver();

    cv::Mat getResult();
};

#endif

int main(){
	cv::Mat m;
	Solver s(m);
	return 0;
}
