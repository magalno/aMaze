
#include "solver.h"
#include <iostream>

using namespace std;
using namespace cv;


Solver::Solver(cv::Mat &src){
	this->image = src.clone();
	this->orginal_image = this->image.clone();
	thin_image();
	find_gird_sizes();
	find_maze_arrays();
	solv_maze();
	create_maze_solution_image();
	cout <<"solver created"<<endl;

}
Solver::~Solver(){
	cout <<"solver destroyd"<<endl;
}


void Solver::thin_image(){
	// using Zhang-Suen's thinning algorithem
	// http://opencv-code.com/puick-tips/implementation-of-thinning-algorithem-in-opencv/
	Mat marker = Mat::zeros(this->image.size(),CV_8UC1);
	//invertin image
	this->image = Scalar::all(255) - this->image;

	int iter =0;
	for (int a = 0; a<10;a++){
		// 10 is bigg enogh, cooud be doone in a wile and contiue until no more changes
		for(int i = 1; i<this->image.rows-1;i++){
			for (int j = 1;j<this->image.cols -1;j++){
				// pizels <155 == 0, >155 ==1	
				int p2 = (int)this->image.at<uchar>(i-1,j)/155;
				int p3 = (int)this->image.at<uchar>(i-1,j+1)/155;
				int p4 = (int)this->image.at<uchar>(i,j+1)/155;
				int p5 = (int)this->image.at<uchar>(i+1,j+1)/155;
				int p6 = (int)this->image.at<uchar>(i+1,j)/155;
				int p7 = (int)this->image.at<uchar>(i+1,j-1)/155;
				int p8 = (int)this->image.at<uchar>(i,j-1)/155;
				int p9 = (int)this->image.at<uchar>(i-1,j-1)/155;

				int A = ((p2 && p3) +(p3 && p4) +
						(p4 && p5) +(p5 && p6) +
						(p6 && p7) +(p7 && p8) +
						(p8 && p9) +p9 && p2); 
				int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;	
				int m1 = iter ? (p2*p4*p6) : (p2*p4*p8);
				int m2 = iter ? (p4*p6*p8) : (p2*p6*p8);
			//	cout << A << "  " << B << "  " << m1 << "  " << m2<<endl;
				if (A==1 &&(B>=2 &&B<=6) && m1== 0 && m2 ==0)
					marker.at<uchar>(i,j) = 255;
					
			}
		}
		for (int x =0; x < this->image.rows;x++){
				for (int y =0; y< this->image.cols;y++){
					if (marker.at<uchar>(x,y) == 255){

						this->image.at<uchar>(x,y) = 0;
					}

				//cout <<(int)this->image.at<uchar>(x,y)<<endl;

			}
		}
		iter++;
		iter %=2;
	}
	
	//invertin image
	this->image = Scalar::all(255) - this->image;

}
void Solver::find_gird_sizes(){
	vector<int> minDistList;

	for (int row = 0; row < this->image.rows;row++){
		int pixel =0;
		int distance = this->image.rows;
		int minDistance = this->image.rows;

		while (pixel < this->image.cols){
			if(this->image.at<uchar>(row,pixel) == 0){
				if(distance < minDistance){
					minDistance = distance;
				}
				distance =0;
			}
			pixel++;
			distance++;
		}
		if(minDistance !=1 && minDistance != this->image.rows){
			minDistList.push_back(minDistance);
		}
	}
	this->grid_size_x = minDistList.at(minDistList.size()/2);
	

	minDistList.clear();

	for (int col = 0; col < this->image.cols;col++){
		int pixel =0;
		int distance = this->image.cols;
		int minDistance = this->image.cols;

		while (pixel < this->image.rows){
			if(this->image.at<uchar>(col,pixel) == 0){
				if(distance < minDistance){
					minDistance = distance;
				}
				distance =0;
			}
			pixel++;
			distance++;
		}
		if(minDistance !=1 && minDistance != this->image.rows){
			minDistList.push_back(minDistance);
		}
	}
	this->grid_size_y = minDistList.at(minDistList.size()/2);

	cout <<this->grid_size_y<<endl;
}
void Solver::find_maze_arrays(){
	
	this->maze_size_x = (this->image.rows/this->grid_size_x)*2+1;
	this->maze_size_y = (this->image.cols/this->grid_size_y)*2+1;
	
	this->maze_array = new int * [this->maze_size_y];
	for(int i = 0; i < this->maze_size_y; ++i) {
    	this->maze_array[i] = new int[this->maze_size_x];
	}
	for (int x =0;x<this->maze_size_x;x++){
		for(int y = 0;y<this->maze_size_y+1;y++){
			if(this->check_area(x*this->grid_size_x/2,y*this->grid_size_y/2,
								this->grid_size_x/4,this->grid_size_y/4)){
				this->maze_array[x][y] =1;
			}
			else{
				this->maze_array[x][y] =0;
			}
		}
	}
	
}
bool Solver::check_area(int pos_x,int pos_y,int size_x,int size_y){
	int black_pixle_count =0;
	for(int x =0; x<size_x;x++){
		for (int y = 0; y<size_y;y++){
			if (this->orginal_image.rows > pos_x+x && 
				this->orginal_image.cols > pos_y+y){
				if(this->orginal_image.at<uchar>(pos_x+x,pos_y+y)==0){
					black_pixle_count++;
				}
			}
		}
	
	}
	return (black_pixle_count<=2);
}
void Solver::solv_maze(){
	
	/*
	this->maze_solution_array = new int * [this->maze_size_y];
	for(int i = 0; i < this->maze_size_y;i ++) {
    	this->maze_array[i] = new int[this->maze_size_x];
	}
	
	for (int x =0; x<this->maze_size_x-1;x++){
		for (int y = 0;y<this->maze_size_y-1;y++){
			cout <<x<<" "<<y<<endl;
			this->maze_solution_array[x][y] = 0;//this->maze_array[x][y];
		}
	}
	
	//finding start and goal positions
	for (int x = 0; x<this->maze_size_x;x++){
		if (this->maze_solution_array[x][0] == 1){
			this->maze_start_x = x;
			this->maze_start_y = 0;
		}
	}
	for (int y = 0; y<this->maze_size_y;y++){
		if (this->maze_solution_array[0][y] == 1){
			this->maze_start_x = 0;
			this->maze_start_y = y;
		}
	}
	for (int x = 0; x<this->maze_size_x;x++){
		if (this->maze_solution_array[x][this->maze_size_x] == 1){
			this->maze_goal_x = x;
			this->maze_goal_y = 0;
		}
	}
	for (int y = 0; y<this->maze_size_y;y++){
		if (this->maze_solution_array[this->maze_size_y][y] == 1){
			this->maze_goal_x = 0;
			this->maze_goal_y = y;
		}
	}
	cout<<this->maze_goal_x<<" "<<this->maze_goal_y<<" "<<this->maze_start_x<<" "<<this->maze_start_y;
	*/
}
bool Solver::recursive_solve(int x,int y){


}
void Solver::create_maze_solution_image(){

}
void Solver::print_maze(){
	cout<<endl<<endl;
	for (int x =0; x<this->maze_size_x;x++){
		for (int y=0; y< this->maze_size_y;y++){
			if (this->maze_array[x][y] == 0){
				cout<<"x";
			}
			else if (this->maze_array[x][y] ==1){
				cout<<"o";
			}
		}
		cout<<endl;
	}
}
void Solver::print_maze_solution(){

}
cv::Mat Solver::getSolution(){
	cout <<"retriving result"<<endl;
	return this->image;
}

