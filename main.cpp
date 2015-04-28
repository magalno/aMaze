#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "preprocessing.h"
#include "perspective_correction.h"
#include "solver.h"

//using namespace std;
//using namespace cv;

int main(){
	cv::Mat src = cv::imread("mazes/maze1.jpg");

    if (src.empty())
    {
        return 1;
    }

	// Do a simple threshold before sending it to the perspective correction module
	Preprocessing pp(src);
    if(!pp.process())
	{
		std::cout << "Could not perform preprocessing" << std::endl;
		return 2;
	}
	cv::Mat bw = pp.getResult();
	PerspectiveCorrection pc(bw);
	if(!pc.process())
	{
		std::cout << "Could not perform perspective correction" << std::endl;
		return 2;
	}

	cv::Mat transformed = pc.getResult();
	Solver solver(transformed);

	
	if(!solver.process()) {

	}
	
	cv::Mat solution = solver.getResult();
	cv::imshow("SOLUTION", solution);


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
