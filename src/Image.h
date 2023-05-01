#pragma once

#include <vector>
#include "opencv2/opencv.hpp"

class Image
{
public:
	Image(int width, int height);
	Image(const char* filename);
	Image(cv::Mat mat) : m_Mat(mat) {}

	void rotate(double angle);
	void rotate(double angle, int x, int y);
	void resize(int width, int height);
	void resize(double factor);
	void changeBrightness(double delta);
	void dilate(int shape, int size);
	void erode(int shape, int size);

	void save(char* filename);
	void show(const char* windowName);

	inline cv::Mat getMat() { return m_Mat; }

private:
	cv::Mat m_Mat;
};

Image stitch(std::vector<Image>& images);