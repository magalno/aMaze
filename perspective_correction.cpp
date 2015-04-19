#include "perspective_correction.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>

#define DEBUG

//#define USE_OPENCV

PerspectiveCorrection::PerspectiveCorrection(cv::Mat &_src)
	: src(_src), result(400, 400, CV_8UC1)
{

}

PerspectiveCorrection::~PerspectiveCorrection()
{

}

bool PerspectiveCorrection::process()
{
	cv::Mat annotated = src.clone();

#ifdef DEBUG
	cv::namedWindow("Original", cv::WINDOW_NORMAL);
	cv::namedWindow("blackwhite", cv::WINDOW_NORMAL);
	cv::namedWindow("annotated", cv::WINDOW_NORMAL);
	cv::namedWindow("transformed", cv::WINDOW_NORMAL);

	cv::resizeWindow("Original", 400, 400);
	cv::resizeWindow("blackwhite", 400, 400);
	cv::resizeWindow("annotated", 400, 400);
	cv::resizeWindow("transformed", 400, 400);


	cv::imshow("Original", src);
#endif

	cv::Mat bw;
	cv::cvtColor(src, bw, CV_BGR2GRAY);
	cv::threshold(bw, bw, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

#ifdef DEBUG
	cv::imshow("blackwhite", bw);
#endif

	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(bw, lines, 1, CV_PI/180, 70, 30, 10);

	// Draw lines
	for (int i = 0; i < lines.size(); i++)
	{
		cv::Vec4i v = lines[i];
		cv::line(annotated, cv::Point(v[0], v[1]), cv::Point(v[2], v[3]), CV_RGB(255,0,0), 4);
	}

#ifdef DEBUG
	cv::imshow("annotated", annotated);
#endif

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

	cv::circle(annotated, topLeft, 20, CV_RGB(0,0,255), 2);
	cv::circle(annotated, topRight, 20, CV_RGB(0,0,255), 2);
	cv::circle(annotated, bottomLeft, 20, CV_RGB(0,0,255), 2);
	cv::circle(annotated, bottomRight, 20, CV_RGB(0,0,255), 2);

#ifdef DEBUG
	cv::imshow("annotated", annotated);
#endif

	cv::Mat quad = cv::Mat::zeros(400, 400, bw.type());

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

#ifdef DEBUG
		std::cout << "Top left: " << topLeft << std::endl;
		std::cout << "Top right: " << topRight << std::endl;
		std::cout << "Bottom left: " << topLeft << std::endl;
		std::cout << "Bottom right: " << bottomRight << std::endl;
#endif

	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);

#ifdef USE_OPENCV
	cv::warpPerspective(bw, quad, transmtx, quad.size(), cv::INTER_NEAREST);
#else
	doTransform(transmtx, bw, quad);
#endif

	bitwise_not ( quad, quad );

#ifdef DEBUG
	cv::imshow("transformed", quad);
#endif

	result = quad;

	return true;
}

bool PerspectiveCorrection::doTransform(cv::Mat &transmtx,cv::Mat &src, cv::Mat &dst)
{
#ifdef DEBUG
	std::cout << "Src: rows:" << src.rows << ", cols:" << src.cols << std::endl;
	std::cout << "Transformation matrix:" << std::endl << transmtx << std::endl;
#endif

	cv::Mat transmtx_inv = transmtx.inv();

#ifdef DEBUG
	std::cout << "Inverse transformation matrix:" << std::endl  << transmtx_inv << std::endl;
#endif

	cv::Point3d dstPoint;
	dstPoint.z = 1.0f;
	cv::Point3d srcPoint;


	// For every point in the destination image
	// Calculate the respective point in the source image in the top left corner of the pixel,
	// and the bottom right corner (could be a rather large area if source image is bigger than the destination image)
	// Search this area if any pixel is white, and if so set the pixel in the destination image to white
	for(int y = 0; y < dst.rows; y++)
	{
		for(int x = 0; x < dst.cols; x++)
		{
			// Do the math operation srcPoint = transmtx_inv * dstPoint;
			// Create the homeneous cordinate for the destination point
			dstPoint.x = x-0.9;
			dstPoint.y = y-0.9;
			cv::Mat V = transmtx_inv * cv::Mat(dstPoint, false);
			V.copyTo( cv::Mat(srcPoint, false) );

			// Convert the homegenous coordinate for source back to cartesian form
			srcPoint.x /= srcPoint.z;
			srcPoint.y /= srcPoint.z;

			int topLeftX = (int)srcPoint.x;
			int topLeftY = (int)srcPoint.y;

			// Do the same for the bottom right corner
			dstPoint.x = x+0.9;
			dstPoint.y = y+0.9;
			V = transmtx_inv * cv::Mat(dstPoint, false);
			V.copyTo( cv::Mat(srcPoint, false) );

			// Convert the homegenous coordinate for source back to cartesian form
			srcPoint.x /= srcPoint.z;
			srcPoint.y /= srcPoint.z;

			int bottomRightX = (int)srcPoint.x;
			int bottomRightY = (int)srcPoint.y;

			// Search for white pixels in this area
			// We probably don't have to search the whole area
			// So to speed it up, search along a line from the topLeft to the bottomRight
			// We simplify this even more by find the minimum bouding square and search along that diagonal
			// Calculate the length of that diagonal
			int diagLength = std::max(std::min(std::abs(bottomRightX - topLeftX), std::abs(bottomRightY - topLeftY)), 1);
			for(int delta = 0; delta < diagLength; delta++)
			{
				uint8_t data = src.at<uint8_t>(topLeftY+delta, topLeftX+delta); // Yes, its rows first.. This took me forever to figure out....
				if(data == 255)
				{
					dst.at<uint8_t>(y, x) = 255;
					break; // No need to search more when the result wont change
				}
			}

			//dst.at<uint8_t>(x, y) = 0;
		}
	}
}

cv::Mat PerspectiveCorrection::getResult()
{
	return result;
}
