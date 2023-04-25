#pragma once

#include "opencv2/opencv.hpp"

class Image
{
public:
	Image(int width, int height);
	Image(char* filename);

	void rotate(double angle);
	void rotate(double angle, int x, int y);
	void resize(int width, int height);
	void changeBrightness(double delta);
	void dilate();
	void erode();
private:
	cv::Mat m_Mat;
};