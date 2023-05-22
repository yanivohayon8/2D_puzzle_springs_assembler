#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <box2d/box2d.h>
# define M_PI           3.14159265358979323846  /* pi */

class SfmlScreen
{
public:
	SfmlScreen(int width, int height, float widthScale, float heightScale);
	int height_;
	int width_;
	sf::RenderWindow window_;
	std::string windowName_ = "v";
	std::vector<sf::RectangleShape> boundsRectangles_;
	std::vector<sf::ConvexShape> convexPolygons_;
	float bodiesScale_ = 0.05;//0.1f; //
	float widthScale_;
	float heightScale_;

	void initDisplay();
	void initBounds(std::vector<std::vector<b2Vec2>>& boundsBodyCoordinates);
	void initSprites();

	bool isWindowOpen();
	void clearDisplay();
	void updateDisplay();
	void drawBounds();
	void drawPolygon(std::vector<b2Vec2>& coordinates, const b2Transform& trans, const b2Vec2& centerMass);
	void drawCircle(const b2Vec2& center, float radius, sf::Color& color);
	void drawLine(b2Vec2& point1, b2Vec2& point2, sf::Color& color, float thickness);

	float castToImageY(float y);

	bool pollEvent(sf::Event& event);
	void closeWindow();

};

