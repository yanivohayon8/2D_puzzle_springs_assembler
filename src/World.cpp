#include "World.h"

World::World()
{
	//world_ = &(std::make_unique<b2World>(b2Vec2(0, 0)));
	//world_ = &b2World(b2Vec2(0, 0));

	//cvDebugDraw cvVis;
	//world_.SetDebugDraw(&cvVis);
}

b2Body* World::createPieceBody(Piece& piece)
{

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2{ 3, 2.5};
	//bodyDef.linearVelocity.Set(1, 0); // debug

	b2PolygonShape shape;
	std::vector<b2Vec2>& b2Poly = piece.localCoordsAsVecs_;
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
	//width /= scale;
	//height /= scale;

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
	
	//// working
	//const std::vector<std::vector<float>> boundaries{
	//	{0,20, width - (padding + noOverlap), 20}
	//};

	float gameWidth = 5;
	float startX = 0;
	float startY = 0;
	float wallWidth = 0.1;

	const std::vector<std::vector<float>> boundaries{ 
		{startX,startY, gameWidth, wallWidth}, // from bottom left to the horizontal line
		{gameWidth,startY,wallWidth,gameWidth}
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
		bodyDef.position.Set(x,y);
		//bodyDef.position
		//bodyDef.linearVelocity.Set(0, 0);
		//bodyDef.awake = false;

		auto w = bound->at(2);
		auto h = bound->at(3);

		b2PolygonShape shape;
		std::vector<b2Vec2> b2Poly = {
			{0, 0},
			{w, 0},
			{w, h},
			{0, h}
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

		double xMean = 0;
		double yMean = 0;

		// This is should be in global coords?
		for (int i = 0; i < b2Poly.size(); i++)
		{
			auto& point = b2Poly.at(i);
			xMean += point.x;
			yMean += point.y;
		}

		xMean /= b2Poly.size();
		yMean /= b2Poly.size();
		b2Vec2 mean(xMean, yMean);
		std::vector<b2Vec2> globalCoords;

		for (int i = 0; i < 4; i++)
		{
			b2Vec2& worldPoint = body->GetWorldPoint(b2Poly.at(i));
			//globalCoords.push_back(worldPoint - mean);
			globalCoords.push_back(worldPoint);
		}

		boundsCoordinates_.push_back(globalCoords);
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



	//double puzzleArea = 0;
	//for (auto& piece:pieces_ )
	//{
	//	puzzleArea += piece.getArea();
	//}
	//const int dim = static_cast<int>(3.0 * std::sqrt(puzzleArea));
	//double scale = 0.5; //0.8;
	//int height = dim; //dim * scale;
	//int screenHeight=0;
	//int screenWidth=0;
	//getScreenSize(screenHeight, screenWidth);
	//double screenRatio = screenWidth / (double)screenHeight;
	//int width = dim * screenRatio; //dim * scale;
	
	double scale = 50; //0.8;
	int height = 880; //dim * scale;
	int width = 1440; //dim * scale;

	//double scale = 50; //0.8;
	//int height = 800; //dim * scale;
	//int width = 800; //dim * scale;
	
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
	cv::Scalar redColor = { 0,0,255 };

	screen_->initDisplay();


	while (!isFinished)
	{
		screen_->clearDisplay();
		screen_->drawBounds(&boundsCoordinates_);
		
		world_.Step(timeStep, velocityIterations, positionIterations);

		for (auto pieceIt = pieces_.begin(); pieceIt != pieces_.end(); pieceIt++)
		{
			pieceIt->translate();
			screen_->drawPolygon(pieceIt->globalCoordinates_, pieceIt->color_);
			const b2Transform &transform = pieceIt->refb2Body_->GetTransform();
			screen_->drawCircle(transform.p, 3, redColor);
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
