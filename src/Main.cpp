#include <iostream>

#include <opencv2/opencv.hpp>

int main(void)
{
	cv::Size size(640, 420);
	cv::Mat im = cv::Mat::zeros(size, CV_64F);
	cv::imshow("Hello, World", im);
	cv::waitKey();
}