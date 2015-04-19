#ifndef SOLVER_H
#define SOLVER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <list>

class Solver
{
private:
	cv::Mat image;
	cv::Mat orginal_image;
	int** maze_array;
	int** maze_solution_array;
	int grid_size_x;
	int grid_size_y;
	int maze_size_x;
	int maze_size_y;
	int maze_goal_x;
	int maze_goal_y;
	int maze_start_x;
	int maze_start_y;

	
	void thin_image();
	void find_gird_sizes();
	void find_maze_arrays();
	bool check_area(int pos_x,int pos_y,int size_x,int size_y);
	void solv_maze();
	bool recursive_solve(int x,int y);
	void create_maze_solution_image();
	

public:
    Solver(cv::Mat &img);
    ~Solver();

    cv::Mat getSolution();
	void print_maze();
	void print_maze_solution();
};

#endif


