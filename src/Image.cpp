#include "Image.h"

#include <algorithm>

Image::Image(int width, int height)
{
	m_Mat = cv::Mat::zeros(cv::Size(width, height), CV_64F);
}

Image::Image(const char* filename)
{
	m_Mat = cv::imread(filename);
}

void Image::rotate(double angle)
{
	rotate(angle, m_Mat.cols / 2, m_Mat.rows / 2);
}

void Image::rotate(double angle, int x, int y)
{
	cv::Mat transform = cv::getRotationMatrix2D(cv::Point2f(x, y), angle, 1.0);
	cv::warpAffine(m_Mat, m_Mat, transform, m_Mat.size());
}

void Image::resize(int width, int height)
{
	cv::resize(m_Mat, m_Mat, cv::Size(width, height));
}

void Image::resize(double factor)
{
	resize(m_Mat.cols * factor, m_Mat.rows * factor);
}

void Image::changeBrightness(double delta)
{
	m_Mat.convertTo(m_Mat, m_Mat.type(), 1, delta);
}

void Image::dilate(int shape, int size)
{
	cv::Mat element = cv::getStructuringElement(shape, cv::Size(2 * size + 1, 2 * size + 1), cv::Point(size, size));
	cv::dilate(m_Mat, m_Mat, element);
}

void Image::erode(int shape, int size)
{
	cv::Mat element = cv::getStructuringElement(shape, cv::Size(2 * size + 1, 2 * size + 1), cv::Point(size, size));
	cv::erode(m_Mat, m_Mat, element);
}

void Image::save(char* filename)
{
	cv::imwrite(filename, m_Mat);
}

void Image::show(const char* windowName)
{
	cv::imshow(windowName, m_Mat);
}

Image stitch(std::vector<Image>& images)
{
	std::vector<cv::Mat> mats;

	std::transform(images.begin(), images.end(), std::back_inserter(mats), [](Image image) { return image.getMat(); });

	cv::Mat stitched;

	cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::SCANS);
	stitcher->stitch(mats, stitched);

	return Image(stitched);
}
