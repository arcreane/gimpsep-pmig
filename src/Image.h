#pragma once

#include <vector>
#include "opencv2/opencv.hpp"

/**
	Wrapper class for OpenCV's Mat class
*/
class Image
{
public:
	Image(int width, int height);
	Image(const char* filename);
	Image() : m_Mat() {}
	Image(cv::Mat mat) : m_Mat(mat) {}

	/**
		Rotates the image around its center
	*/
	void rotate(double angle);

	/**
		Rotates the image around a given point
	*/
	void rotate(double angle, int x, int y);

	/**
		Resizes the image to a given size
	*/
	void resize(int width, int height);

	/**
		Resizes the image by a given factor
	*/
	void resize(double factor);

	/**
		Changes the brightness of the image
	*/
	void changeBrightness(double delta);

	/**
		Dilates the image
	*/
	void dilate(int shape, int size);

	/**
		Erodes the image
	*/
	void erode(int shape, int size);

	/**
		Saves the image to a file
	*/
	void save(const char* filename);

	/**
		Shows the image in a window
	*/
	void show(const char* windowName);

	inline cv::Mat getMat() { return m_Mat; }
	inline int getWidth() { return m_Mat.cols; }
	inline int getHeight() { return m_Mat.rows; }

	inline bool loadedSuccessfully() { return m_Mat.data; }


	/**
		Stitches a vector of images together
	*/
	static Image stitch(std::vector<Image>& images, cv::Stitcher::Mode mode = cv::Stitcher::SCANS);

	/**
		Stitches a vector of images together in PANORAMA mode
	*/
	static Image panorama(std::vector<Image>& images);

private:
	cv::Mat m_Mat;
};