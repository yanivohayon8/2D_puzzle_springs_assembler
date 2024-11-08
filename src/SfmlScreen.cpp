#include "sfmlScreen.h"

SfmlScreen::SfmlScreen(int width,int height, float widthScale,float heightScale)
{
	width_ = width;
	height_ = height;
	widthScale_ = widthScale;
	heightScale_ = heightScale;
	nextPolygonColorIndex_ = 0;
}

void SfmlScreen::initDisplay(bool isVisible)
{
	window_.create(sf::VideoMode(width_, height_), "Physical Optimization");
	window_.setVisible(isVisible);
}

bool SfmlScreen::pollEvent(sf::Event& nextEvent)
{
	return window_.pollEvent(nextEvent);
}

void SfmlScreen::closeWindow()
{
	window_.close();
	pieceId2Sprite_.clear();
	pieceId2texture_.clear();
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

		float rectangleWidth = (bound.at(1).x - bound.at(0).x);
		float rectangleHeight = (bound.at(3).y - bound.at(0).y);
		sf::RectangleShape rectangle(sf::Vector2f(rectangleWidth, rectangleHeight));
		rectangle.setScale(sf::Vector2f(widthScale_, heightScale_));
		//rectangle.setFillColor(debugColors[iColor++]);
		rectangle.setPosition(sf::Vector2f(widthScale_ *bound.at(0).x, heightScale_ *bound.at(0).y));
		rectangle.setOrigin(0, 0);
		boundsRectangles_.push_back(rectangle);
	}
}

void SfmlScreen::initSprite(Piece& piece)
{
	
	sf::Texture texture;
	if (!texture.loadFromFile(piece.imagePath_))
	{
		throw std::runtime_error("Could not open file " + piece.imagePath_);
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale(widthScale_ * SCALE_IMAGE_COORDINATES_TO_BOX2D, heightScale_* SCALE_IMAGE_COORDINATES_TO_BOX2D); 

	float debugX = texture.getSize().x/2;
	float debugY = texture.getSize().y/2;

	pieceId2Sprite_.insert({ piece.id_,sprite });
	pieceId2texture_.insert({ piece.id_, texture });
}

void SfmlScreen::initPolygon(Piece& piece)
{
	const b2Transform& trans = piece.refb2Body_->GetTransform();
	sf::ConvexShape convex;
	convex.setPointCount(piece.localCoordsAsVecs_.size());

	for (int i = 0; i < piece.localCoordsAsVecs_.size(); i++)
	{
		sf::Vector2f vec(piece.localCoordsAsVecs_[i].x, piece.localCoordsAsVecs_[i].y);
		convex.setPoint(i, vec);
	}

	double rotateRadians = trans.q.GetAngle();
	double rotateDegrees = rotateRadians * 180.0 / M_PI;
	convex.rotate(rotateDegrees);

	auto& position = trans.p;
	convex.setPosition(widthScale_ * position.x, heightScale_ * position.y);
	convex.setScale(widthScale_, heightScale_);

	pieceId2Polygon_.insert({ piece.id_,convex });
	nextPolygonColorIndex_ = (nextPolygonColorIndex_ + 1) % SfmlScreen::sfmlColors.size();	
	auto nextColor = SfmlScreen::sfmlColors[nextPolygonColorIndex_];

	if (nextColor == sf::Color::Black || nextColor == sf::Color::Red)
	{
		nextPolygonColorIndex_ = (nextPolygonColorIndex_ + 1) % SfmlScreen::sfmlColors.size();
		nextColor = SfmlScreen::sfmlColors[nextPolygonColorIndex_];
	}

	pieceId2PolygonColor_.insert({ piece.id_, nextColor });
}


bool SfmlScreen::drawSprite(std::string pieceId, const b2Transform& trans)
{

	// If the piece does not have a sprite
	if (pieceId2Sprite_.find(pieceId) == pieceId2Sprite_.end() || pieceId2texture_.find(pieceId) == pieceId2texture_.end())
	{
		return false;
	}

	sf::Sprite& sprite = pieceId2Sprite_.at(pieceId);
	sprite.setTexture(pieceId2texture_.at(pieceId));
	double rotateRadians = trans.q.GetAngle();
	double rotateDegrees = rotateRadians * 180.0 / M_PI;
	sprite.setRotation(rotateDegrees);

	auto& position = trans.p;
	sprite.setPosition(widthScale_ * position.x, heightScale_ * position.y);

	window_.draw(sprite);

	return true;
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

void SfmlScreen::drawPolygon(std::string pieceId, const b2Transform& trans)
{
	sf::ConvexShape& convex = pieceId2Polygon_.at(pieceId);
	double rotateRadians = trans.q.GetAngle();
	double rotateDegrees = rotateRadians * 180.0 / M_PI;
	convex.setRotation(rotateDegrees);

	auto& position = trans.p;
	convex.setPosition(widthScale_ * position.x, heightScale_ * position.y);
	convex.setFillColor(pieceId2PolygonColor_.at(pieceId));
	window_.draw(convex);
}

void SfmlScreen::initPolygonCoordsDots(Piece& piece, float radius, sf::Color& color)
{
	std::vector<sf::CircleShape> dots;

	for (auto& cord : piece.globalCoordinates_)
	{
		sf::CircleShape dot(radius);
		dot.setOrigin(radius / 2, radius / 2);
		dot.setPosition(cord.x * widthScale_, cord.y * widthScale_);
		dot.setScale(sf::Vector2f(widthScale_, heightScale_));
		dot.setFillColor(color);
		dots.push_back(dot);
	}

	pieceId2PolygonsCoords_.insert({ piece.id_,dots });
}

void SfmlScreen::drawPolygonDots(std::string pieceId, std::vector<b2Vec2>& coordinates)
{
	std::vector<sf::CircleShape>& dots = pieceId2PolygonsCoords_.at(pieceId);

	int i = 0;
	for (auto &dot :dots)
	{
		dot.setPosition(coordinates[i].x * widthScale_, coordinates[i].y * widthScale_);
		window_.draw(dot);
		++i;
	}
}

void initPolygonCenter(Piece& piece, float radius, sf::Color& color)
{

}

sf::CircleShape SfmlScreen::initCircle(const b2Vec2& center, float radius, sf::Color color)
{
	sf::CircleShape circle(radius);
	circle.setOrigin(radius / 2, radius / 2);
	circle.setPosition(center.x * widthScale_, center.y * widthScale_);
	circle.setScale(sf::Vector2f(widthScale_, heightScale_));
	circle.setFillColor(color);
	return circle;
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

void SfmlScreen::drawCircle(const b2Vec2& center, float radius, sf::Color color)
{
	sf::CircleShape circle(radius);
	circle.setOrigin(radius / 2, radius / 2);
	circle.setPosition(center.x * widthScale_, center.y * widthScale_);
	circle.setScale(sf::Vector2f(widthScale_, heightScale_));
	circle.setFillColor(color);
	window_.draw(circle);
}

void SfmlScreen::screenShotToFile(std::string fileName)
{
	sf::Image image;
	image = window_.capture();
	image.saveToFile(fileName);
}


const std::vector<sf::Color> SfmlScreen::sfmlColors = {
		sf::Color::Black,
		sf::Color::White,
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Yellow,
		sf::Color::Magenta,
		sf::Color::Cyan
};