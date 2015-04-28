#include "preprocessing.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

//#define DEBUG

#define THRESHOLD_VAL	55
#define WEIGHT_RED      1
#define WEIGHT_GREEN    1
#define WEIGHT_BLUE     1

#define KERNEL_SIZE 11

#define COLOR_RES       8
#define N_COLORS        pow(2, COLOR_RES)
#define COLOR_MAX_VAL   (N_COLORS - 1)

Preprocessing::Preprocessing(cv::Mat &_src)
	: src(_src)
{

}

Preprocessing::~Preprocessing()
{

}

bool Preprocessing::process()
{
    // Convert to grayscale
    Mat gray;
    gray.create(src.rows, src.cols, CV_8U);
    convert_to_grayscale(src, gray);
    
    //OpenCV
    #define KERNEL_SIZE 11
    Mat element = getStructuringElement(MORPH_RECT, Size(KERNEL_SIZE*2+1, KERNEL_SIZE*2+1), Point(KERNEL_SIZE,KERNEL_SIZE) );

    //OpenCV
    morphologyEx(gray, gray, 6, element );
    
    //morphed
    namedWindow("bottomhat", WINDOW_NORMAL);
	resizeWindow("bottomhat", 600, 600);
    imshow("bottomhat",gray);
    
    //int thresh = get_otsu_thresh_val(gray,N_COLORS);
    int thresh = 20;
    threshold_grayscale(gray, thresh, 0);
    
    result = gray.clone(); 
    
    //display original
    namedWindow("res", WINDOW_NORMAL);
	resizeWindow("res", 600, 600);
    imshow("res",gray);
    
    return true;    
}

void Preprocessing::get_histogram(Mat src, int hist[], int n_colors){
    int index;
    for(int x = 0; x < src.rows - 0; x++) {
        for(int y = 0; y < src.cols - 0; y++) {
            hist[src.at<uchar>(x,y)]++;  
        }
     }   
}

void Preprocessing::print_hist(int hist[], int n_colors){
    cout << "\nHistogram is:" <<endl;
    for(int i = 0; i < n_colors - 0; i++){
        cout << hist[i] << " ";
    }
    cout<<endl;
}

int Preprocessing::get_otsu_thresh_val(Mat src, int n_colors){
    //required variables for otsu calculation
    int otsu_thresh_val = 0;
    float cuml_first = 0.0;
    float cuml_second = 0.0;
    float mean_total = 0.0;
    float tmp1 = 0.0; 
    float tmp2 = 0.0; 
    double tmp3 = 0.0;

    //Get histogram
    int hist[n_colors] = {0};     
    get_histogram(src, hist, n_colors);
    
    
    //Normalize histogram and clculate global mean
    float hist_norm[n_colors];
    for(int i = 0; i < n_colors ; ++i){
        hist_norm[i] = hist[i] / (float) (src.rows*src.cols);
        mean_total += ((float)i) * hist_norm[i];
    }
    
    //Find Otsu threshold
    for(int i = 0; i < n_colors; i++){
        cuml_first += ((float)hist_norm[i]);
        cuml_second += (((float)i) * hist_norm[i]);
        
        tmp1 = (float)((mean_total * cuml_first) - cuml_second);
        tmp2 = (float)(tmp1 * tmp1) / (float)(cuml_first * (1.0f - cuml_first));
        
        if(tmp2 > tmp3){
            tmp3 = tmp2;
            otsu_thresh_val = i;
        } 
    }
    return otsu_thresh_val;
}

void Preprocessing::threshold_grayscale(Mat img, int threshold, bool invert){
    for(int j=0;j<img.rows;j++) {
        for(int i=0;i<img.cols;i++){
            if( img.at<uchar>(j,i) >= threshold ){ 
                if(invert){ 
                    img.at<uchar>(j,i) = 0; 
                }else{
                    img.at<uchar>(j,i) = COLOR_MAX_VAL;
                }
            }else{
                if(invert){
                    img.at<uchar>(j,i) = COLOR_MAX_VAL;
                }else{
                    img.at<uchar>(j,i) = 0;
                }
            }
        }
    }

}

void Preprocessing::filter(Mat src, Mat dst, Mat kernel){
    cout << "filter" << endl;
    
    // Calculate the sum of the filter
    double kernel_sum = 0.0;
    for(int x=0; x<kernel.rows - 0; x++) {
        for(int y=0; y<kernel.cols - 0; y++) {
        	kernel_sum += kernel.at<double>(x,y);
        }
    }

	double pixel_sum = 0.0;
	int kernel_offset = kernel.rows / 2;

    for(int src_x = 0; src_x < src.rows - 0; src_x++) {
        for(int src_y = 0; src_y < src.cols - 0; src_y++) {
            pixel_sum = 0.0;
            for(int ker_x = -kernel_offset; ker_x <= kernel_offset; ker_x++) {
                for(int ker_y = -kernel_offset; ker_y <= kernel_offset; ker_y++) {
                    if(src_x + ker_x > src.rows || src_y + ker_y > src.cols || src_y + ker_y < 0) {
                    	continue;
                    }else{
                        pixel_sum += (src.at<uchar>(src_x + ker_x, 
                                src_y + ker_y) * kernel.at<double>(
                                ker_x + kernel_offset, ker_y +
                                kernel_offset));                    
                    }
                }
            }
            dst.at<uchar>(src_x, src_y) = pixel_sum / kernel_sum;
        }
    }
}

void Preprocessing::filter_sep(Mat src, Mat dst, Mat kernel_x, Mat kernel_y){

}

void Preprocessing::print_mat(Mat img){
	for(int x=0; x<img.rows - 0; x++) {
        for(int y=0; y<img.cols - 0; y++) {
        	cout << img.at<double>(x,y) << "\t";
        }
        cout << endl;
    }
}

void Preprocessing::convert_to_grayscale(Mat src, Mat dst){
    int gray_val;
    for(int x=0; x<src.rows; x++){
        for(int y=0; y<src.cols; y++){
            //Get pixel RGB values from source image
            Vec3b color = src.at<Vec3b>(Point(y,x));
            
            //Calculate grayscale value
            gray_val = (color.val[RED]*WEIGHT_RED +
                    color.val[GREEN]*WEIGHT_GREEN +
                    color.val[BLUE]*WEIGHT_BLUE)/3;
                    
            //Write grayscale pixel to destination image
            dst.at<uchar>(x,y) = gray_val;
        }
    }
}

void Preprocessing::threshold_adaptive(Mat src, Mat dst, int tile_size){
    Mat tile;
    Mat tile_copy;
    int threshold;
    
    for(int row = 0; row < src.rows; row += tile_size){
        for(int col = 0; col < src.cols; col += tile_size){
            tile = src( Range(row, min(row + tile_size, src.rows)), 
                    Range(col, min(col + tile_size, src.cols)));
            threshold = get_otsu_thresh_val(tile, N_COLORS); 
            threshold_grayscale(tile, threshold, 0);        
        }
    }
}

void Preprocessing::erode(Mat src, Mat se){
    int se_center_r = se.rows/2;
    int se_center_c = se.cols/2;
    int offset_r = 0;
    int offset_c = 0;
    unsigned char color_val = COLOR_MAX_VAL;
    
    for(int src_row = 0; src_row < src.rows; src_row++){
        for(int src_col = 0; src_col < src.cols; src_col++){
            for(int se_row = -se_center_r; se_row < se.rows - se_center_r; se_row++){
                for(int se_col = -se_center_c; se_col < se.cols - se_center_c; se_col++){
                    //get the current offset
                    offset_r = src_row + se_row;
                    offset_c = src_col + se_col;
                    
                    //ignore pixels out of bounds
                    if(offset_r < 0 || 
                            offset_c < 0 || 
                            offset_c > src.cols || 
                            offset_r > src.rows){
                        continue;     
                    }else if((src.at<uchar>(offset_r, offset_c) < color_val) && 
                            se.at<uchar>(se_row + se_center_r, se_col + se_center_c)){
                        color_val = src.at<uchar>(offset_r, offset_c);       
                    }
                }
            }
            
            //update pixel
            src.at<uchar>(src_row, src_col) = color_val;
            color_val = COLOR_MAX_VAL;
        }
    }
    namedWindow("eroded", WINDOW_NORMAL);
	resizeWindow("eroded", 600, 600);
    imshow("eroded",src);
}

void Preprocessing::dialate(Mat src, Mat se){
    int se_center_r = se.rows/2;
    int se_center_c = se.cols/2;
    int offset_r = 0;
    int offset_c = 0;
    unsigned char color_val = 0;
    
    for(int src_row = 0; src_row < src.rows; src_row++){
        for(int src_col = 0; src_col < src.cols; src_col++){
            for(int se_row = -se_center_r; se_row < se.rows - se_center_r; se_row++){
                for(int se_col = -se_center_c; se_col < se.cols - se_center_c; se_col++){
                    //get the current offset
                    offset_r = src_row + se_row;
                    offset_c = src_col + se_col;
                    
                    //ignore pixels out of bounds
                    if(offset_r < 0 || 
                            offset_c < 0 || 
                            offset_c > src.cols || 
                            offset_r > src.rows){
                        continue;     
                    }else if((src.at<uchar>(offset_r, offset_c) > color_val) && 
                            se.at<uchar>(se_row + se_center_r, se_col + se_center_c)){
                        color_val = src.at<uchar>(offset_r, offset_c);       
                    }
                }
            }
            
            //update pixel
            src.at<uchar>(src_row, src_col) = color_val;
            color_val = 0;
        }
    }
    namedWindow("dialated", WINDOW_NORMAL);
	resizeWindow("dialated", 600, 600);
    imshow("dialated",src);
}

void Preprocessing::close(Mat img, Mat se){
    dialate(img, se);
    erode(img, se);
    
    namedWindow("closed", WINDOW_NORMAL);
	resizeWindow("closed", 600, 600);
    imshow("closed",img);
}

void Preprocessing::open(Mat img, Mat se){
    erode(img, se);
    dialate(img, se);
    
    namedWindow("opened", WINDOW_NORMAL);
	resizeWindow("opened", 600, 600);
    imshow("opened",img);
}

void Preprocessing::bottom_hat(Mat img, Mat se){
    Mat clone = img.clone();
    close(img, se);
    
    for(int row = 0; row < img.rows; row++){
        for(int col = 0; col < img.cols; col++){
            img.at<uchar>(row, col) -= clone.at<uchar>(row, col);
        }    
    }
    namedWindow("bottom hat", WINDOW_NORMAL);
	resizeWindow("bottom hat", 600, 600);
    imshow("bottom hat",img);
}

void Preprocessing::top_hat(Mat img, Mat se){
    Mat clone = img.clone();
    open(img, se);
    
    for(int row = 0; row < img.rows; row++){
        for(int col = 0; col < img.cols; col++){
            img.at<uchar>(row, col) -= clone.at<uchar>(row, col);
        }    
    }
    namedWindow("top hat", WINDOW_NORMAL);
	resizeWindow("top hat", 600, 600);
    imshow("top hat",img);
}

void Preprocessing::invert(Mat img){
    for(int row = 0; row < img.rows; row++){
        for(int col = 0; col < img.cols; col++){
            img.at<uchar>(row,col) = COLOR_MAX_VAL - img.at<uchar>(row, col);
        }
    }
}

