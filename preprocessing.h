#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <opencv2/imgproc/imgproc.hpp>

enum color{
    RED,
    GREEN,
    BLUE
};

class Preprocessing
{
public:
    Preprocessing(cv::Mat &src);
    ~Preprocessing();

    bool process();
    cv::Mat getResult();

private:
    void get_histogram(cv::Mat src, int hist[], int n_colors);
    void print_hist(int hist[], int n_colors);
    int get_otsu_thresh_val(cv::Mat src,int n_colors);
    void threshold_grayscale(cv::Mat img, int threshold, bool invert);
    void filter(cv::Mat src, cv::Mat dst, cv::Mat kernel);
    void filter_sep(cv::Mat src, cv::Mat dst, cv::Mat kernel_x, cv::Mat kernel_y);
    void print_mat(cv::Mat img);
    void convert_to_grayscale(cv::Mat src, cv::Mat dst);
    void threshold_adaptive(cv::Mat src, cv::Mat dst, int tile_size);
    void erode(cv::Mat src, cv::Mat se);
    void dialate(cv::Mat src, cv::Mat se);
    void open(cv::Mat src, cv::Mat se);
    void close(cv::Mat src, cv::Mat se);
    void top_hat(cv::Mat src, cv::Mat se);
    void bottom_hat(cv::Mat src, cv::Mat se);
    void invert(cv::Mat img);

    cv::Mat src;
    cv::Mat result;
};

#endif
