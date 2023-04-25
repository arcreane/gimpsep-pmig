#include <iostream>

#include "Image.h"

int main(void)
{
	Image image(640, 420);
	image.show("Hello, World");
	cv::waitKey();
}