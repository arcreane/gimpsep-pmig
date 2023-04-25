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
	void resize(double factor);
	void changeBrightness(double delta);
	void dilate(int shape, int size);
	void erode(int shape, int size);

	void save(char* filename);
	void show(const char* windowName);
private:
	cv::Mat m_Mat;
};