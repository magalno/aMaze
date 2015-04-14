
#include "solver.h"
#include <iostream>

using namespace std;
using namespace cv;

Solver::Solver(cv::Mat &src){
	cout <<"solver created"<<endl;
}
Solver::~Solver(){
	cout <<"solver destroyd"<<endl;
}

cv::Mat Solver::getResult(){
	Mat m;
	return m;
}

