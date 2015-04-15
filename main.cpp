#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "solver.h"

using namespace std;
using namespace cv;

int main(){

	//testing mase solver
	Mat m =imread("mazes/squareMaze.jpg");
	if (m.empty()){
		cout<<"somthing wrong with image"<<endl;
		return -1;
	}
	Solver s(m);
	Mat solution = s.getSolution();
	imshow("solution",solution);
	waitKey();
	return 0;
}
