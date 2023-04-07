#include "World.h"

World::World()
{
	//world_ = &(std::make_unique<b2World>(b2Vec2(0, 0)));
	//world_ = &b2World(b2Vec2(0, 0));
}

b2Body* World::createBody(Piece& piece)
{
	std::vector<b2Vec2> b2Poly;
	int numCoords = piece.getNumCoords();

	for (int i = 0; i < numCoords; i++)
	{
		auto [x, y] = piece.getVertexCoord(i);
		float x_ = static_cast<float>(x);
		float y_ = static_cast<float>(y);
		b2Poly.push_back(b2Vec2{ x_,y_});
	}

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2{0,0};
	//bodyDef.position.Set(0.0f, 4.0f);

	b2PolygonShape shape;
	shape.Set(b2Poly.data(), b2Poly.size());
	/*if (is_polygon_degenerate(points)) {
		s.SetAsBox(b2_linearSlop * 2,
			b2_linearSlop * 2);
	}
	else {
		s.Set(points.data(), points.size());
	}*/

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1;
	fixture.filter.groupIndex = -2;

	b2Body* oBody = world_.CreateBody(&bodyDef);
	oBody->CreateFixture(&fixture);
	return oBody;
}



void World::connectSpringsToPieces(const EdgeMating &edgeMating)
{

}

void World::initBounds(int height, int width, int scale)
{
	width /= scale;
	height /= scale;
	//const std::vector<std::vector<float>> boundaries{{0, 0, 1, 1}};
	const std::vector<std::vector<float>> boundaries{};
	
	//const std::array<std::array<float, 4>, 4> bs{ {{0, 0, width / 2, 1},
	//											  {0, 0, height / 2, 1}}
	//											  //{width / 2, 0, 1, height},
	//											  //{-width / 2, 0, 1, height}}
	//};

	for (auto& bound = boundaries.begin(); bound != boundaries.end(); bound++)
	{
		auto x = bound->at(0);
		auto y = bound->at(1);
		b2BodyDef  bodyDef;
		bodyDef.type = b2_staticBody; // is this necessary? 
		bodyDef.position.Set(x, y);
		
		auto w = bound->at(2);
		auto h = bound->at(3);
		b2PolygonShape shape;
		shape.SetAsBox(w, h);
		
		auto* body = world_.CreateBody(&bodyDef);
		body->CreateFixture(&shape, 0);

		Eigen::MatrixX2d coords;
		coords.resize(4, 2);
		coords.row(0) << x, y;
		coords.row(1) << x + width, y;
		coords.row(2) << x, y + height;
		coords.row(3) << x + width, y + height;
			
		boundsCoordinates_.push_back(coords);
	}
}

void World::Init(std::vector<Piece>& pieces)
{
	
	for (auto pieceIt = pieces.begin(); pieceIt!=pieces.end(); pieceIt++)
	{
		b2Body* body = this->createBody(*pieceIt);
		pieceIt->refb2Body_ = body;
		pieces_.push_back(*pieceIt);
	}

	// TODO call connectSpringsToPieces by the pairs
	

	double puzzleArea = 0;
	for (auto pieceIt = pieces_.begin(); pieceIt != pieces_.end(); pieceIt++) { puzzleArea += pieceIt->getArea(); };

	const int dim = static_cast<int>(1.2 * std::sqrt(puzzleArea));
	int scale = 1;
	int height; // = dim * scale;
	int width; // = dim * scale;
	
	getScreenSize(height, width);
	
	//height = 800; //  peleg's value //dim * scale;
	//width = 1440; //  peleg's value //dim * scale;

	initBounds(height,width,scale);

	screen_ = new Screen(height, width, scale);

	// Assign color for debuging or rendring
	std::vector<cv::Scalar> colors(std::size(pieces_));
	generateColors(colors);

	auto& pieceIt = pieces_.begin();
	auto colorIt = colors.begin();

	while(pieceIt!=pieces_.end())
	{
		pieceIt->color_ = *colorIt;
		++pieceIt;
		++colorIt;
	}
}

void World::Simulation()
{
	
	// The following params make as parameters to the function
	double timeStep = 1.0F / 60.0F;
	int velocityIterations = 6;
	int positionIterations = 2;
	bool isFinished = false;
	float damping = 0;

	screen_->initDisplay();

	while (!isFinished)
	{
		screen_->clearDisplay();
		/*for (int i = 0; i < boundsCoordinates_.size(); i++)
		{
			screen_->drawPolygon(boundsCoordinates_[i], cv::Scalar(255,255,255));
		}*/
		
		//screen_->drawBounds(boundsCoordinates_);
		
		screen_->drawBounds();
		
		world_.Step(timeStep, velocityIterations, positionIterations);

		for (auto pieceIt = pieces_.begin(); pieceIt != pieces_.end(); pieceIt++)
		{
			const b2Transform &transform = pieceIt->refb2Body_->GetTransform();
			pieceIt->rotate(transform.q);
			pieceIt->translate(transform.p);
			screen_->drawPolygon(pieceIt->coordinates_, pieceIt->color_);
		}

		int pressedKey = screen_->updateDisplay();

		switch (pressedKey)
		{
		case 'q':
			isFinished = true;
		default:
			break;
		}

	}

	screen_->finishDisplay();

}
