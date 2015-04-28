#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <opencv2/imgproc/imgproc.hpp>

class Preprocessing
{
public:
    Preprocessing(cv::Mat &src);
    ~Preprocessing();

    bool process();
    cv::Mat getResult();

private:
    void get_histogram(Mat src, int hist[], int n_colors);
    void print_hist(int hist[], int n_colors);
    int get_otsu_thresh_val(Mat src,int n_colors);
    void threshold_grayscale(Mat img, int threshold, bool invert);
    void filter(Mat src, Mat dst, Mat kernel);
    void filter_sep(Mat src, Mat dst, Mat kernel_x, Mat kernel_y);
    void print_mat(Mat img);
    void convert_to_grayscale(Mat src, Mat dst);
    void threshold_adaptive(Mat src, Mat dst, int tile_size);
    void erode(Mat src, Mat se);
    void dialate(Mat src, Mat se);
    void open(Mat src, Mat se);
    void close(Mat src, Mat se);
    void top_hat(Mat src, Mat se);
    void bottom_hat(Mat src, Mat se);
    void invert(Mat img);

    cv::Mat src;
    cv::Mat result;
};

#endif
