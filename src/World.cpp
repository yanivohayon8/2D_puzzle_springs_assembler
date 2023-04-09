#include "World.h"

World::World()
{
	//world_ = &(std::make_unique<b2World>(b2Vec2(0, 0)));
	//world_ = &b2World(b2Vec2(0, 0));
}

b2Body* World::createPieceBody(Piece& piece)
{

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2{ 1000, 600 };
	//bodyDef.linearVelocity.Set(0, -10); // debug

	b2PolygonShape shape;
	std::vector<b2Vec2> b2Poly;
	int numCoords = piece.getNumCoords();

	for (int i = 0; i < numCoords; i++)
	{
		auto [x, y] = piece.getVertexCoord(i);
		float x_ = static_cast<float>(x);
		float y_ = static_cast<float>(y);
		b2Poly.push_back(b2Vec2{ x_,y_});
	}
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
	fixture.density = 1.0f;
	fixture.friction = 0.3f;
	//fixture.filter.groupIndex = 2;

	b2Body* oBody = world_.CreateBody(&bodyDef);
	oBody->CreateFixture(&fixture);

	for (int i = 0; i < numCoords; i++)
	{
		b2Vec2& localPoint = b2Poly.at(i);
		b2Vec2& globalPoint = oBody->GetWorldPoint(localPoint);
		piece.globalCoordinates_.push_back(globalPoint);
	}

	return oBody;
}



void World::connectSpringsToPieces(const EdgeMating &edgeMating)
{

}

void World::initBounds(float height, float width, float scale, float padding)
{
	width /= scale;
	height /= scale;

	float noOverlap = 2;

	//const std::vector<std::vector<float>> boundaries{
	//	{0, 0, width - (padding + noOverlap), 20} // the box stops from falling
	//};

	

	//const std::vector<std::vector<float>> boundaries{ 
	//	{150, 0, 20, 20},
	//	{-width / 2, -height / 2 + padding, width - (padding+ noOverlap), 1}, // from bottom left to the horizontal line
	//	{-width / 2 +padding, height/2 - padding , width - (padding + noOverlap), 1}, // from top left along the horizontal line
	//	{-width / 2 + padding, -height / 2, 1, height - (padding + noOverlap)}, // from bottom left along the vertical line
	//	{width / 2 - padding, -height / 2, 1, height - (padding + noOverlap)} // from bottom right up to the vertical line
	//};
	
	const std::vector<std::vector<float>> boundaries{ 
		{0,0 + padding, width - (padding+ noOverlap), 1} // from bottom left to the horizontal line
		//{0, 0, width - (padding + noOverlap), 1}, // from top left along the horizontal line
		//{0, 0, 1, height - (padding + noOverlap)}, // from bottom left along the vertical line
		//{0, 0, 1, height - (padding + noOverlap)} // from bottom right up to the vertical line
	};

	for (auto& bound = boundaries.begin(); bound != boundaries.end(); bound++)
	{
		float x = bound->at(0);
		float y = bound->at(1);
		b2BodyDef  bodyDef;
		bodyDef.type = b2_staticBody; // b2_staticBody; // is this necessary? 
		bodyDef.position.Set(0, 0);
		//bodyDef.position
		//bodyDef.linearVelocity.Set(0, 0);
		//bodyDef.awake = false;

		auto w = bound->at(2);
		auto h = bound->at(3);

		b2PolygonShape shape;
		std::vector<b2Vec2> b2Poly = {
			{x, y},
			{x + w, y},
			{x + w, y + h},
			{x, y + h}
		};

		//shape.SetAsBox(w/2, h/2);
		shape.Set(b2Poly.data(), b2Poly.size());

		b2FixtureDef fixture;
		fixture.shape = &shape;
		//fixture.density = 1.0f;
		//fixture.friction = 0.3f;
		//fixture.filter.groupIndex = 2;
		fixture.restitution = 0.75;

		auto* body = world_.CreateBody(&bodyDef);
		body->CreateFixture(&fixture);
		//body->SetAwake()
		//body->CreateFixture(&shape, 0);

		std::vector<b2Vec2> globalCoords;
		for (int i = 0; i < 4; i++)
		{
			globalCoords.push_back(body->GetWorldPoint(b2Poly.at(i)));
		}

		boundsCoordinates_.push_back(globalCoords);

		/*Piece debugP(89, coords);
		debugP.refb2Body_ = body;
		pieces_.push_back(debugP);*/
	}
}


void World::Init(std::vector<Piece>& pieces)
{
	
	for (auto pieceIt = pieces.begin(); pieceIt!=pieces.end(); pieceIt++)
	{
		b2Body* body = this->createPieceBody(*pieceIt);
		pieceIt->refb2Body_ = body;
		pieces_.push_back(*pieceIt);
	}

	// TODO call connectSpringsToPieces by the pairs

	/*const int dim = static_cast<int>(1.2 * std::sqrt(puzzleArea));*/
	int scale = 1;
	int height; // = dim * scale;
	int width; // = dim * scale;
	
	getScreenSize(height, width);
	
	screen_ = new Screen(height, width, scale);

	initBounds(height,width,scale,screen_->BOUNDS_THICKNESS_);

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
		screen_->drawBounds(&boundsCoordinates_);
		
		world_.Step(timeStep, velocityIterations, positionIterations);

		for (auto pieceIt = pieces_.begin(); pieceIt != pieces_.end(); pieceIt++)
		{
			const b2Transform &transform = pieceIt->refb2Body_->GetTransform();

			//debug
			if (transform.p.y < 10)
			{
				std::cout << "here" << std::endl;
			}

			pieceIt->rotate(transform.q);
			auto& velocity = pieceIt->refb2Body_->GetLinearVelocity();
			pieceIt->translate(velocity);
			screen_->drawPolygon(pieceIt->globalCoordinates_, pieceIt->color_);
		}

		for (b2Contact* contact = world_.GetContactList(); contact; contact = contact->GetNext())
		{
			std::cout << "collide" << contact->GetFixtureA() << std::endl;
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
