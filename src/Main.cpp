#include <iostream>

#include "Image.h"


int main(void)
{
	std::cout << " " << std::endl;
	std::cout << "  ____            _       " << std::endl;
	std::cout << " |  _ \\ _ __ ___ (_) __ _ " << std::endl;
	std::cout << " | |_) | '_ ` _ \\| |/ _` |" << std::endl;
	std::cout << " |  __/| | | | | | | (_| |" << std::endl;
	std::cout << " |_|   |_| |_| |_|_|\\__, |" << std::endl;
	std::cout << "                     |___/" << std::endl;
	std::cout << " " << std::endl;

	std::cout << "Input the command you want to use:<" << std::endl;
	std::cout << "1. Open an image" << std::endl;
	std::cout << "2. Créer une nouvelle image" << std::endl;
	std::cout << "3. Créer un panorama à partir de plusieurs images" << std::endl;
	std::cout << "4. Quitter l'application" << std::endl;
}

void enterEditMode()
{
	std::cout << "1. Sauvegarder l'image" << std::endl;
	std::cout << "2. Montrer l'image" << std::endl;
	std::cout << "3. Tourner l'image" << std::endl;
	std::cout << "4. Redimensionner l'image " << std::endl;
	std::cout << "5. Changer la luminosité de l'image " << std::endl;
	std::cout << "6. Dilater l'image " << std::endl;
	std::cout << "7. Eroder l'image " << std::endl;
}