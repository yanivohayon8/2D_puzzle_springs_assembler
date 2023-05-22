#include "SfmlScreen.h"

SfmlScreen::SfmlScreen(int width,int height, float widthScale,float heightScale)
{
	width_ = width;
	height_ = height;
	widthScale_ = widthScale;
	heightScale_ = heightScale;
	//sf::RenderWindow window_(sf::VideoMode(width_, height_), "Vika");
}

void SfmlScreen::initDisplay()
{
	window_.create(sf::VideoMode(width_, height_), "Vika");
}

bool SfmlScreen::pollEvent(sf::Event& nextEvent)
{
	return window_.pollEvent(nextEvent);
}

void SfmlScreen::closeWindow()
{
	window_.close();
}

void SfmlScreen::initBounds(std::vector<std::vector<b2Vec2>>& boundsBodyCoordinates)
{

	std::vector<sf::Color> debugColors = {
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Yellow
	};

	int iColor = 0;

	for (auto& bound: boundsBodyCoordinates)
	{
		/*float rectangleWidth = (bound.at(1).x - bound.at(0).x)* widthScale_;
		float rectangleHeight = (bound.at(3).y - bound.at(0).y)*heightScale_;*/
		float rectangleWidth = (bound.at(1).x - bound.at(0).x);
		float rectangleHeight = (bound.at(3).y - bound.at(0).y);
		sf::RectangleShape rectangle(sf::Vector2f(rectangleWidth, rectangleHeight));
		rectangle.setScale(sf::Vector2f(widthScale_, heightScale_));
		rectangle.setFillColor(debugColors[iColor++]);
		rectangle.setPosition(sf::Vector2f(widthScale_ *bound.at(0).x, heightScale_ *bound.at(0).y));
		//rectangle.setOrigin(rectangle.getSize().x/2, rectangle.getSize().y/2);
		rectangle.setOrigin(0, 0);
		boundsRectangles_.push_back(rectangle);
	}



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

void SfmlScreen::drawBounds()
{
	for (auto &bd:boundsRectangles_)
	{
		window_.draw(bd);
	}
}

void SfmlScreen::drawPolygon(std::vector<b2Vec2>& coordinates, const b2Transform& trans, const b2Vec2& centerMass)
{
	// create an empty shape
	sf::ConvexShape convex;

	// resize it to 5 points
	convex.setPointCount(coordinates.size());

	for (int i = 0; i < coordinates.size(); i++)
	{
		sf::Vector2f vec(coordinates[i].x, coordinates[i].y);
		convex.setPoint(i, vec);
	}

	double rotateRadians = trans.q.GetAngle();
	double rotateDegrees = rotateRadians * 180.0 / M_PI;
	convex.rotate(rotateDegrees);

	auto& position = trans.p;
	convex.setPosition(sf::Vector2f(widthScale_ * position.x, heightScale_ * position.y));

	convex.setScale(widthScale_,heightScale_);
	window_.draw(convex);
}


void SfmlScreen::drawCircle(const b2Vec2& center, float radius, sf::Color& color)
{
	sf::CircleShape shape(radius);
	shape.setOrigin(shape.getRadius() / 2, shape.getRadius() / 2);
	//shape.setPosition(sf::Vector2f(center.x* widthScale_, center.y* widthScale_));
	shape.setPosition(sf::Vector2f(center.x* widthScale_, center.y* widthScale_));
	shape.setScale(sf::Vector2f(widthScale_, heightScale_));
	
	// set the shape color to green
	shape.setFillColor(color);
	window_.draw(shape);
}


void SfmlScreen::drawLine(b2Vec2& point1, b2Vec2& point2, sf::Color& color, float thickness)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(widthScale_*point1.x,heightScale_*point1.y),color),
		sf::Vertex(sf::Vector2f(widthScale_*point2.x,heightScale_*point2.y),color)
	};


	window_.draw(line, 2, sf::Lines);
}

