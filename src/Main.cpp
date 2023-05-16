#include <iostream>
#include <windows.h>

#include "Image.h"

Image image;

int askNumber(int min, int max, std::string message);
std::string askString(std::string message);
double askDouble(double min, double max, std::string message);
Image openImage(std::string line = "Entrez le nom du fichier: ");
void enterEditMode();
void enterStitchMode();

int main(void)
{
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
		case 1:
			image = openImage();
			enterEditMode();
			break;
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

void enterEditMode()
{
	std::cout << "1. Sauvegarder l'image" << std::endl;
	std::cout << "2. Montrer l'image" << std::endl;
	std::cout << "3. Tourner l'image (en son centre)" << std::endl;
	std::cout << "4. Tourner l'image (autours d'un point)" << std::endl;
	std::cout << "5. Redimensionner l'image (par un facteur)" << std::endl;
	std::cout << "6. Redimensionner l'image (largeur et hauteur)" << std::endl;
	std::cout << "7. Changer la luminosité de l'image " << std::endl;
	std::cout << "8. Dilater l'image" << std::endl;
	std::cout << "9. Eroder l'image" << std::endl;
	std::cout << "10. Détection des bords" << std::endl;
	std::cout << "11. Retour au menu principal" << std::endl;

	int command = askNumber(1, 11, "Entrez un nombre entre 1 et 11.");
	bool returnToMain = false;

	switch (command)
	{
	case 1:
		image.save(askString("Entrez le nom du fichier.").c_str());
		break;
	case 2: 
		image.show("Image");
		break;
	case 3:
		image.rotate(askDouble(0, 360, "Entrez un angle entre 0 et 360°"));
		break;
	case 4:
		image.rotate(askDouble(0, 360, "Entrez un angle entre 0 et 360°"), askNumber(0, image.getWidth(), "Entrez la coordonnée x du point de rotation"), askNumber(0, image.getHeight(), "Entrez la coordonnée y du point de rotation"));
		break;
	case 5:
		image.resize(askDouble(0, 100, "Entrez le facteur de redimensionnement (entre 0 et 100)"));
		break;
	case 6:
		image.resize(askNumber(1, 1920, "Entrez la largeur (entre 1 et 1920)"), askNumber(1, 1080, "Entrez la hauteur (entre 1 et 1080)"));
		break;
	case 7:
		image.changeBrightness(askDouble(0, 100, "Entrez la luminosité (entre 0 et 100)"));
		break;
	case 8:
		image.dilate(askNumber(1, 3, "Entrez la forme de l'élément structurant (1: rectangle, 2: ellipse, 3: croix)"), askNumber(1, 8, "Entrez le nombre de fois que vous voulez dilater l'image (entre 1 et 8)"));
		break;
	case 9:
		image.erode(askNumber(1, 3, "Entrez la forme de l'élément structurant (1: rectangle, 2: ellipse, 3: croix)"), askNumber(1, 8, "Entrez le nombre de fois que vous voulez eroder l'image (entre 1 et 8)"));
		break;
	case 10:
		//TODO
	case 11:
		returnToMain = true;
	default:
		break;
	}

	if (!returnToMain) enterEditMode();
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