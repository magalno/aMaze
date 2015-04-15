
#include "solver.h"
#include <iostream>

using namespace std;
using namespace cv;


Solver::Solver(cv::Mat &src){
	this->image = src.clone();
	thin_image(this->image);
	find_gird_sizes();
	find_maze_arrays();
	solv_maze();
	create_maze_solution_image();
	cout <<"solver created"<<endl;

}
Solver::~Solver(){
	cout <<"solver destroyd"<<endl;
}


void Solver::thin_image(cv::Mat &img){

}
void Solver::find_gird_sizes(){

}
void Solver::find_maze_arrays(){

}
void Solver::solv_maze(){

}
void Solver::create_maze_solution_image(){

}
void Solver::print_maze(){

}
void Solver::print_maze_solution(){

}
cv::Mat Solver::getSolution(){
	cout <<"retriving result"<<endl;
	return this->image;
}

