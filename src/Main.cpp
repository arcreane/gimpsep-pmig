#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <windows.h>


#include "Image.h"

#include <opencv2/core/utils/logger.hpp>

Image image;

int askNumber(int min, int max, std::string message);
std::string askString(std::string message);
double askDouble(double min, double max, std::string message);
Image openImage(std::string line = "Entrez le nom du fichier: ");
void showSliders(int command, Image& image);
bool enterEditMode();
void enterStitchMode();

int main(void)
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	bool exit = false;
	SetConsoleOutputCP(CP_UTF8);

	do
	{
		std::cout << " " << std::endl;
		std::cout << "  ____            _       " << std::endl;
		std::cout << " |  _ \\ _ __ ___ (_) __ _ " << std::endl;
		std::cout << " | |_) | '_ ` _ \\| |/ _` |" << std::endl;
		std::cout << " |  __/| | | | | | | (_| |" << std::endl;
		std::cout << " |_|   |_| |_| |_|_|\\__, |" << std::endl;
		std::cout << "                     |___/" << std::endl;
		std::cout << " " << std::endl;

		std::cout << "Entrez la commande que vous voulez utiliser: " << std::endl;
		std::cout << "1. Ouvrir l'image" << std::endl;
		std::cout << "2. Créer un panorama à partir de plusieurs images" << std::endl;
		std::cout << "3. Quitter l'application" << std::endl;

		int command = askNumber(1, 3, "Entrez un nombre entre 1 et 3.");
		switch (command)
		{
		case 1: {
			image = openImage();
			image.show("Image");
			bool exitEditMode = false;
			while (!exitEditMode) {
				exitEditMode = enterEditMode();
			}
			break;
		}
		case 2:
			enterStitchMode();
		case 3:
			exit = true;
		}
	} while (!exit);
	return 0;
}

Image openImage(std::string line)
{
	std::string filename;
	Image image;
	do
	{
		std::cout << line;
		std::cin >> filename;

		image = Image((filename.c_str()));
	} while (!image.loadedSuccessfully());

	return image;
}

int askNumber(int min, int max, std::string message)
{
	bool error = false;
	int number;
	do
	{
		std::cout << message << std::endl;
		std::string enteredCommand;
		std::cin >> enteredCommand;

		try
		{
			number = std::stoi(enteredCommand);
			error = false;
		}
		catch (std::invalid_argument)
		{
			error = true;
		}
	} while ((number < min || number > max) && !error);
	return number;
}
std::string askString(std::string message)
{
	std::cout << message << std::endl;
	std::string str;
	std::cin >> str;
	return str;
}

double askDouble(double min, double max, std::string message) {
	bool error = false;
	double number;
	do
	{
		std::cout << message << std::endl;
		std::string enteredCommand;
		std::cin >> enteredCommand;
		try
		{
			number = std::stod(enteredCommand);
			error = false;
		}
		catch (std::invalid_argument)
		{
			error = true;
		}
	} while ((number < min || number > max) && !error);
	return number;
}

bool enterEditMode()
{
	Image copy = image.copy();

	copy.show("Image");
	std::cout << "1. Sauvegarder l'image" << std::endl;
	std::cout << "2. Tourner l'image (en son centre)" << std::endl;
	std::cout << "3. Tourner l'image (autours d'un point)" << std::endl;
	std::cout << "4. Redimensionner l'image (par un facteur)" << std::endl;
	std::cout << "5. Redimensionner l'image (largeur et hauteur)" << std::endl;
	std::cout << "6. Changer la luminosité de l'image " << std::endl;
	std::cout << "7. Dilater l'image" << std::endl;
	std::cout << "8. Eroder l'image" << std::endl;
	std::cout << "9. Détection des bords" << std::endl;
	std::cout << "10. Retour au menu principal" << std::endl;

	int command = askNumber(1, 10, "Entrez un nombre entre 1 et 10.");

	showSliders(command, copy);

	if (command == 1) {
		image.save(askString("Entrez le nom du fichier.").c_str());
		return false;
	}
	if (command == 10) {
		return true;
	}

	std::promise<bool> finished;
	auto future = finished.get_future();

	int subCommand = -1;

	std::thread inputThread([&finished, &subCommand] {
		subCommand = askNumber(1, 3, "Voulez vous appliquer les changements (1) ou entrer une valeur en dure (2) ou annuler (3) ?");
		finished.set_value(true);
	});

	while (future.wait_for(std::chrono::duration<int, std::milli>(0)) != std::future_status::ready) {
		copy.show("Image");
	}

	inputThread.join();

	cv::destroyWindow("Image");

	if (subCommand == 1) {
		image = copy;
		return false;
	}

	bool returnToMain = false;

	if (subCommand == 2) {
		switch (command)
		{
		case 1:
			image.show("Image");
			break;
		case 2:
			image.rotate(askDouble(0, 360, "Entrez un angle entre 0 et 360°"));
			break;
		case 3:
			image.rotate(askDouble(0, 360, "Entrez un angle entre 0 et 360°"), askNumber(0, image.getWidth(), "Entrez la coordonnée x du point de rotation"), askNumber(0, image.getHeight(), "Entrez la coordonnée y du point de rotation"));
			break;
		case 4:
			image.resize(askDouble(0, 300, "Entrez le facteur de redimensionnement (entre 0 et 300)"));
			break;
		case 5:
			image.resize(askNumber(1, 1920, "Entrez la largeur (entre 1 et 1920)"), askNumber(1, 1080, "Entrez la hauteur (entre 1 et 1080)"));
			break;
		case 6:
			image.changeBrightness(askDouble(0, 100, "Entrez la luminosité (entre 0 et 100)"));
			break;
		case 7:
			image.dilate(askNumber(0, 2, "Entrez la forme de l'élément structurant (0: rectangle, 1: croix, 2: ellipse)"), askNumber(1, 8, "Entrez le nombre de fois que vous voulez dilater l'image (entre 1 et 8)"));
			break;
		case 8:
			image.erode(askNumber(0, 2, "Entrez la forme de l'élément structurant (0: rectangle, 1: croix, 2: ellipse)"), askNumber(1, 8, "Entrez le nombre de fois que vous voulez eroder l'image (entre 1 et 8)"));
			break;
		case 9:
			image = image.detectEdges(2*askNumber(0, 2, "Entrez la taille (entre 0 et 2)")+3, askDouble(0, 256, "Entrez le seuil (entre 0 et 256)"));
			break;
		default:
			break;
		}
	}

	return false;
}

void showSliders(int command, Image& modified) {
	switch (command) {
	case 2: {
		cv::createTrackbar("Angle", "Image", NULL, 360, [](int angle, void* ptr) {
			Image copy = image.copy();
			copy.rotate(angle);
			*(Image*)ptr = copy;
		}, &modified);
		break;
	}
	case 3: {
		cv::Point* point = new cv::Point(0,0);

		struct PointImage {
			cv::Point* point;
			Image* image;
		};

		PointImage* pi = new PointImage();
		pi->point = point;
		pi->image = &modified;
		std::cout << "Cliquez pour affecter le centre de rotation." << std::endl;
		cv::setMouseCallback("Image", [](int event, int x, int y, int flags, void* userdata) {
			if (event == cv::EVENT_LBUTTONDOWN)
			{
				PointImage pi = *((PointImage*)userdata);
				pi.point->x = x;
				pi.point->y = y;
			}
		}, pi);
		cv::createTrackbar("Angle", "Image", NULL, 360, [](int angle, void* ptr) {
			PointImage pi = *((PointImage*)ptr);
			
			Image copy = image.copy();
			copy.rotate(angle, pi.point->x, pi.point->y);
			
			*(pi.image) = copy;
		}, pi);
		break;
	}
	case 4: {
		int defaultValue = 100;
		cv::createTrackbar("Facteur", "Image", &defaultValue, 300, [](int factor, void* ptr) {
			Image copy = image.copy();
			copy.resize(factor);
			*(Image*)ptr = copy;
		}, &modified);
		break;
	}
	case 5: {
		struct Data {
			Image* image;
			int* width;
			int* height;
		};

		Data* data = new Data();
		data->image = &modified;

		int* width = new int;
		*(width) = image.getWidth();
		int* height = new int;
		*(height) = image.getHeight();

		data->width = width;
		data->height = height;

		cv::createTrackbar("Largeur", "Image", width, 1920, [](int width, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.width) = width;

			Image copy = image.copy();

			copy.resize(*(data.width), *(data.height));

			*(data.image) = copy;
			}, data);
		cv::createTrackbar("Hauteur", "Image", height, 1080, [](int height, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.height) = height;

			Image copy = image.copy();

			copy.resize(*(data.width), *(data.height));

			*(data.image) = copy;
			}, data);
		break;
	}
	case 6: {
		cv::createTrackbar("Luminosite", "Image", NULL, 100, [](int brightness, void* ptr) {
			Image copy = image.copy();
			copy.changeBrightness(brightness);
			*(Image*)ptr = copy;
		}, &modified);
		break;
	}
	case 7: {
		struct Data {
			Image* image;
			int* element;
			int* times;
		};

		Data* data = new Data();
		data->image = &modified;

		int* element = new int;
		*(element) = 0;
		int* times = new int;
		*(times) = 0;

		data->element = element;
		data->times = times;

		cv::createTrackbar("Forme", "Image", element, 2, [](int element, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.element) = element;

			Image copy = image.copy();

			copy.dilate(*(data.element), *(data.times) + 1);

			*(data.image) = copy;
			}, data);
		cv::createTrackbar("Nombre", "Image", times, 7, [](int times, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.times) = times;

			Image copy = image.copy();

			copy.dilate(*(data.element), *(data.times)+1);

			*(data.image) = copy;
			}, data);
		break;
	}
	case 8: {
		struct Data {
			Image* image;
			int* element;
			int* times;
		};

		Data* data = new Data();
		data->image = &modified;

		int* element = new int;
		*(element) = 0;
		int* times = new int;
		*(times) = 0;

		data->element = element;
		data->times = times;

		cv::createTrackbar("Forme", "Image", element, 2, [](int element, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.element) = element;

			Image copy = image.copy();

			copy.erode(*(data.element), *(data.times)+1);

			*(data.image) = copy;
		}, data);
		cv::createTrackbar("Nombre", "Image", times, 7, [](int times, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.times) = times;

			Image copy = image.copy();

			copy.erode(*(data.element), *(data.times)+1);

			*(data.image) = copy;
		}, data);
		break;
	}
	case 9: {
		struct Data {
			Image* image;
			int* kernelSize;
			int* threshold;
		};

		Data* data = new Data();
		data->image = &modified;

		int* kernelSize = new int;
		*(kernelSize) = 0;
		int* threshold = new int;
		*(threshold) = 0;

		data->kernelSize = kernelSize;
		data->threshold = threshold;

		cv::createTrackbar("Taille", "Image", kernelSize, 2, [](int size, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.kernelSize) = size;

			Image copy = image.detectEdges(2**(data.kernelSize)+3, *(data.threshold));

			*(data.image) = copy;
		}, data);
		cv::createTrackbar("Seuil", "Image", threshold, 256, [](int threshold, void* ptr) {
			Data data = *(Data*)ptr;

			*(data.threshold) = threshold;

			Image copy = image.detectEdges(2 * *(data.kernelSize) + 3, *(data.threshold));

			*(data.image) = copy;
		}, data);
		break;
	}
	default:
		break;
	}
}

void enterStitchMode()
{
	std::cout << "1. Créer un panorama" << std::endl;
	std::cout << "2. Créer une image à partir de plusieurs images" << std::endl;
	int command = askNumber(1, 2, "Entrez un nombre entre 1 et 2.");

	bool panorama = command == 1;

	bool error = false;
	int numberOfImages = 0;
	do
	{
		std::cout << "Combien d'images voulez-vous utiliser ? (Nombre supérieur à 2)" << std::endl;
		std::string enteredCommand;
		std::cin >> enteredCommand;
		try
		{
			numberOfImages = std::stoi(enteredCommand);
			error = false;
		}
		catch (std::invalid_argument)
		{
			error = true;
		}
	} while (numberOfImages < 2 && !error);

	std::vector<Image> images;
	for (int i = 0; i < numberOfImages; i++)
	{
		std::string line = "Entrez le nom du fichier de l'image " + std::to_string(i + 1) + " : ";
		images.push_back(openImage(line));
	}

	image = panorama ? Image::panorama(images) : Image::stitch(images);
	enterEditMode();
}