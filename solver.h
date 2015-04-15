#ifndef PERSPECTIVE_CORRECTION_H
#define PERSPECTIVE_CORRECTION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

class Solver
{
private:
	cv::Mat image;
	bool** maze_array;
	bool** maze_solution_array;
	int grid_size_x;
	int grid_size_y;

	
	void thin_image(cv::Mat &img);
	void find_gird_sizes();
	void find_maze_arrays();
	void solv_maze();
	void create_maze_solution_image();
	

public:
    Solver(cv::Mat &img);
    ~Solver();

    cv::Mat getSolution();
	void print_maze();
	void print_maze_solution();
};

#endif


