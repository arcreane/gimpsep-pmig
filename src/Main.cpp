#include <iostream>

#include "Image.h"

int main(void)
{
	std::vector<Image> images;
	//Read images in folder
	for (int i = 0; i <= 22; i++)
	{
		//format int on two digits
		std::string name = std::to_string(i);

		if(i < 10)
			name = "0" + name;

		std::string filename = "assets/diamondhead/diamondhead-" + name + ".png";
		images.push_back(Image(filename.c_str()));
	}

	Image result = stitch(images);
	result.resize(1280, 960);
	result.show("Result");

	cv::waitKey();
}