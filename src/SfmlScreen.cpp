#include "SfmlScreen.h"

SfmlScreen::SfmlScreen(int height, int width)
{
	height_ = height;
	width_ = width;
	//sf::RenderWindow window_(sf::VideoMode(width_, height_), "Vika");
}

void SfmlScreen::initDisplay()
{
	window_.create(sf::VideoMode(width_, height_), "Vika");
}

void SfmlScreen::clearDisplay()
{
	window_.clear();
}

void SfmlScreen::updateDisplay()
{
	window_.display();
}

bool SfmlScreen::isWindowOpen()
{
	return window_.isOpen();
}


