#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class SfmlScreen
{
public:
	SfmlScreen(int width=1024,int height=1024);
	int height_;
	int width_;
	sf::RenderWindow window_;
	std::string windowName_ = "v";
	void initDisplay();
	bool isWindowOpen();
	void clearDisplay();
	void updateDisplay();
};

