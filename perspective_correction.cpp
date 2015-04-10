#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


int main()
{
	cv::Mat src = cv::imread("maze2.jpg");

    if (src.empty())
    {
        return -1;
    }

    cv::Mat annotated = src.clone();

    cv::imshow("Original", src);

	cv::Mat bw;
	cv::cvtColor(src, bw, CV_BGR2GRAY);
    cv::threshold(bw, bw, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

	cv::imshow("blackwhite", bw);
	cv::waitKey();

    std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(bw, lines, 1, CV_PI/180, 70, 30, 10);

    std::cout << "Lines = " << lines.size() << std::endl;

	// Draw lines
	for (int i = 0; i < lines.size(); i++)
	{
		cv::Vec4i v = lines[i];
		cv::line(annotated, cv::Point(v[0], v[1]), cv::Point(v[2], v[3]), CV_RGB(255,0,0), 4);
	}
    cv::imshow("annotated", annotated);
    cv::waitKey();

    cv::Point2f topLeft;
    float topLeftMin = 9999999999999999.0f;
    cv::Point2f topRight;
    float topRightMin = 9999999999999999.0f;
    cv::Point2f bottomLeft;
    float bottomLeftMin = 9999999999999999.0f;
    cv::Point2f bottomRight;
    float bottomRightMin = 9999999999999999.0f;

    // For each point we see if it's the closest one to the edges of the picture
    for(int lIdx  = 0; lIdx < lines.size(); lIdx++)
    {
        cv::Vec4i v = lines[lIdx];
        for(int pIdx = 0; pIdx < 2; pIdx++)
        {

            cv::Point2f p((float)v[pIdx*2], (float)v[pIdx*2+1]);
            cv::Point2f diff;

            // Check if its the top left corner
            diff.x = p.x - 0.0f;
            diff.y = p.y - 0.0f;
            float length = diff.x*diff.x + diff.y*diff.y;
            if(length < topLeftMin)
            {
                topLeftMin = length;
                topLeft.x = p.x;
                topLeft.y = p.y;
            }

            // Check if top right corner
            diff.x = p.x - bw.cols;
            diff.y = p.y - 0.0f;
            length = diff.x*diff.x + diff.y*diff.y;
            if(length < topRightMin)
            {
                topRightMin = length;
                topRight.x = p.x;
                topRight.y = p.y;
            }

            // Check if bottom left corner
            diff.x = p.x - 0.0f;
            diff.y = p.y - bw.cols;
            length = diff.x*diff.x + diff.y*diff.y;
            if(length < bottomLeftMin)
            {
                bottomLeftMin = length;
                bottomLeft.x = p.x;
                bottomLeft.y = p.y;
            }

            // Check if bottom right corner
            diff.x = p.x - bw.cols;
            diff.y = p.y - bw.rows;
            length = diff.x*diff.x + diff.y*diff.y;
            if(length < bottomRightMin)
            {
                bottomRightMin = length;
                bottomRight.x = p.x;
                bottomRight.y = p.y;
            }
        }
    }

    cv::circle(annotated, topLeft, 3, CV_RGB(0,0,255), 2);
    cv::circle(annotated, topRight, 3, CV_RGB(0,0,255), 2);
    cv::circle(annotated, bottomLeft, 3, CV_RGB(0,0,255), 2);
    cv::circle(annotated, bottomRight, 3, CV_RGB(0,0,255), 2);

    cv::imshow("annotated", annotated);
    cv::waitKey();

    cv::Mat quad = cv::Mat::zeros(400, 400, CV_8UC3);

    std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
	quad_pts.push_back(cv::Point2f(0, quad.rows));

    std::vector<cv::Point2f> corners;
    corners.push_back(topLeft);
    corners.push_back(topRight);
    corners.push_back(bottomRight);
    corners.push_back(bottomLeft);


	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
	cv::warpPerspective(src, quad, transmtx, quad.size());

    cv::imshow("transformed", quad);

	cv::waitKey();
	return 0;
}
