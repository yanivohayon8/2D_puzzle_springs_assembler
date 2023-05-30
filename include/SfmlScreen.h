#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <box2d/box2d.h>
# define M_PI           3.14159265358979323846  /* pi */
#include "Piece.h"


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
	std::map<std::string, sf::Sprite> pieceId2Sprite_;
	std::map<std::string, sf::Texture> pieceId2texture_;
	std::map<std::string, sf::ConvexShape> pieceId2Polygon_;


	void initDisplay();
	void initBounds(std::vector<std::vector<b2Vec2>>& boundsBodyCoordinates);
	void initSprite(Piece& piece);
	void initPolygon(Piece& piece);

	bool isWindowOpen();
	void clearDisplay();
	void updateDisplay();
	void drawBounds();
	void drawPolygon(std::string pieceId, const b2Transform& trans);
	void drawCircle(const b2Vec2& center, float radius, sf::Color& color);
	void drawLine(b2Vec2& point1, b2Vec2& point2, sf::Color& color, float thickness);

	void drawSprite(std::string pieceId, const b2Transform& trans);

	bool pollEvent(sf::Event& event);
	void closeWindow();

};

