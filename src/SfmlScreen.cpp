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
	window_.create(sf::VideoMode(width_, height_), "Physical Optimization");
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

	//float bodyHeight = piece.getBodyBoundingBoxHeight();
	float bodyWidth = piece.getBodyBoundingBoxWidth();
	float boundWidth = sprite.getGlobalBounds().width/1000.f;//sprite.getLocalBounds().width;
	//float boundHeight = sprite.getLocalBounds().height;
	float tmp = bodyWidth / boundWidth;

	float hand_made_scale = widthScale_ / 1000 * 2.8;//widthScale_ / 1000 * 3; //0.165;//2*widthScale_ * tmp/1000;///1000; //widthScale_/1000; //0.125;

	//sprite.setScale(widthScale_*0.01,heightScale_* 0.01); // Divided by 1000 because we divide it as the dataloader
	sprite.setScale(hand_made_scale, hand_made_scale); // Divided by 1000 because we divide it as the dataloader

	float debugX = texture.getSize().x/2;
	float debugY = texture.getSize().y/2;
	sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);

	pieceId2Sprite_.insert({ piece.id_,sprite });
	pieceId2texture_.insert({ piece.id_, texture });
	//window_.draw(sprite);
}

void SfmlScreen::initPolygon(Piece& piece)
{

	const b2Transform& trans = piece.refb2Body_->GetTransform();
	// create an empty shape
	sf::ConvexShape convex;

	// resize it to 5 points
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
	convex.setPosition(sf::Vector2f(widthScale_ * position.x, heightScale_ * position.y));
	convex.setScale(widthScale_, heightScale_);


	//sf::RectangleShape debugBoundingBox(sf::Vector2f(convex.getGlobalBounds().width, convex.getGlobalBounds().height));
	//debugBoundingBox.rotate(rotateDegrees);
	//debugBoundingBox.setPosition(sf::Vector2f(widthScale_ * position.x, heightScale_ * position.y));
	//debugBoundingBox.setOrigin(debugBoundingBox.getSize().x / 2, debugBoundingBox.getSize().y / 2);
	////debugBoundingBox.setScale(widthScale_, heightScale_);
	//debugBoundingBox.setFillColor(sf::Color(0, 0, 0));
	//debugBoundingBox.setOutlineThickness(2.f);
	//debugBoundingBox.setOutlineColor(sf::Color(sf::Color::Red));
	//window_.draw(debugBoundingBox);

	pieceId2Polygon_.insert({ piece.id_,convex });
}

void SfmlScreen::drawSprite(std::string pieceId, const b2Transform& trans)
{
	sf::Sprite& sprite = pieceId2Sprite_.at(pieceId);
	sprite.setTexture(pieceId2texture_.at(pieceId));
	double rotateRadians = trans.q.GetAngle();
	double rotateDegrees = rotateRadians * 180.0 / M_PI;
	//sprite.rotate(rotateDegrees);
	sprite.setRotation(rotateDegrees);

	auto& position = trans.p;
	sprite.setPosition(sf::Vector2f(widthScale_ * position.x, heightScale_ * position.y));

	/*sf::RectangleShape debugBoundingBox(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height));
	debugBoundingBox.rotate(rotateDegrees);
	debugBoundingBox.setPosition(sf::Vector2f(widthScale_ * position.x, heightScale_ * position.y));
	debugBoundingBox.setOrigin(debugBoundingBox.getSize().x / 2, debugBoundingBox.getSize().y / 2);
	debugBoundingBox.setScale(widthScale_, heightScale_);
	debugBoundingBox.setFillColor(sf::Color(0, 0, 0));
	debugBoundingBox.setOutlineThickness(2.f);
	debugBoundingBox.setOutlineColor(sf::Color(sf::Color::Red));*/
	//window_.draw(debugBoundingBox);
	window_.draw(sprite);
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
	//sprite.rotate(rotateDegrees);
	convex.setRotation(rotateDegrees);

	auto& position = trans.p;
	convex.setPosition(sf::Vector2f(widthScale_ * position.x, heightScale_ * position.y));
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

