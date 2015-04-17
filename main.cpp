#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "perspective_correction.h"
#include "solver.h"

//using namespace std;
//using namespace cv;

int main(){
	cv::Mat src = cv::imread("mazes/maze3.jpg");

    if (src.empty())
    {
        return 1;
    }

	PerspectiveCorrection pc(src);
	if(!pc.process())
	{
		std::cout << "Could not perform perspective correction" << std::endl;
		return 2;
	}


	cv::waitKey();
	return 0;

	/*
	//testing mase solver
	Mat m =imread("mazes/squareMaze.jpg",CV_LOAD_IMAGE_GRAYSCALE);

	if (m.empty()){
		cout<<"somthing wrong with image"<<endl;
		return -1;
	}
	Solver s(m);
	s.print_maze();
	s.print_maze_solution();
	Mat solution = s.getSolution();
	imshow("solution",solution);
	waitKey();
	return 0;
	*/
}
