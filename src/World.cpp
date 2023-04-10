#include "World.h"

World::World()
{
}

b2Body* World::createPieceBody(Piece& piece,b2Vec2& initialPosition)
{

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = initialPosition; //b2Vec2{ 3, 2.5};
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
	//fixture.filter.groupIndex = -2; // Don't collide

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

void World::initBounds(float height, float width, float wallWidth)
{

	float originX = 0;
	float originY = 0;
	
	const std::vector<std::vector<float>> boundaries{ 
		{originX,originY, width, wallWidth}, // from bottom left to the horizontal line
		{width,originY,wallWidth,height}, // from bottom right along the vertical line
		{originX,originY,wallWidth,height}, // from bottom left along the vertical line
		{originX,height,width,wallWidth} // from top left along the horizontal line
	};

	for (auto& bound = boundaries.begin(); bound != boundaries.end(); bound++)
	{
		float x = bound->at(0);
		float y = bound->at(1);
		b2BodyDef  bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(x,y);
		bodyDef.awake = false;

		auto w = bound->at(2);
		auto h = bound->at(3);

		b2PolygonShape shape;
		std::vector<b2Vec2> b2Poly = {
			{0, 0},
			{w, 0},
			{w, h},
			{0, h}
		};

		shape.Set(b2Poly.data(), b2Poly.size());

		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.restitution = 0.75;

		auto* body = world_.CreateBody(&bodyDef);
		body->CreateFixture(&fixture);

		std::vector<b2Vec2> globalCoords;

		for (int i = 0; i < 4; i++)
		{
			b2Vec2& worldPoint = body->GetWorldPoint(b2Poly.at(i));
			globalCoords.push_back(worldPoint);
		}

		boundsCoordinates_.push_back(globalCoords);
	}
}

void World::InitPieces(std::vector<Piece>& pieces)
{
	double scale = 50;
	int height = 880; 
	int width = 1440; 
	screen_ = new Screen(height, width, scale);

	float wallWidth = 0.1;
	float boardHeight = 15;
	float boardWidth = 21;
	initBounds(boardHeight,boardWidth,wallWidth);

	std::vector<b2Vec2> positions;
	int seed = 0;
	int padding = 1;
	generate2DVectors(positions, pieces.size(), boardWidth, boardHeight,padding, seed);
	auto& initialPosIt = positions.begin();

	for (auto pieceIt = pieces.begin(); pieceIt != pieces.end(); pieceIt++)
	{
		b2Body* body = this->createPieceBody(*pieceIt,*initialPosIt);
		pieceIt->refb2Body_ = body;
		pieces_.push_back(*pieceIt);
		initialPosIt++;
	}

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

	// TODO call connectSpringsToPieces by the pairs

}

void World::connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB, b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB)
{
	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, *globalCoordsAnchorA, *globalCoordsAnchorB);
	jointDef.collideConnected = true;
	jointDef.minLength = 0.1f; //0.05f;
	jointDef.maxLength = 1.0f; //0.5f;
	jointDef.damping = 0.7f; //1.0f;
	b2DistanceJoint* joint = (b2DistanceJoint*)world_.CreateJoint(&jointDef);
	joints_.push_back(joint);
}

void World::InitMatings(std::vector<EdgeMating>& matings)
{

	for (auto& matingIt: matings)
	{
		Piece* pieceA = &pieces_.at(matingIt.firstPieceId_);
		Piece* pieceB = &pieces_.at(matingIt.secondPieceId_);

		b2Body* bodyA = pieceA->refb2Body_;
		b2Body* bodyB = pieceB->refb2Body_;

		std::pair<int, int> vertsPieceA = pieceA->getEdgeVertexIndexes(matingIt.firstPieceEdge_);
		b2Vec2* firstVertexGlobalA = pieceA->getVeterxGlobalCoords(vertsPieceA.first);
		b2Vec2* secondVertexGlobalA = pieceA->getVeterxGlobalCoords(vertsPieceA.second);
		
		//a test
		/*b2Vec2* firstVertexLocalA = pieceA->getVeterxLocalCoords(vertsPieceA.first);
		b2Vec2* secondVertexLocalA = pieceA->getVeterxLocalCoords(vertsPieceA.second);*/


		std::pair<int, int> vertsPieceB = pieceB->getEdgeVertexIndexes(matingIt.secondPieceEdge_);
		b2Vec2* firstVertexGlobalB = pieceB->getVeterxGlobalCoords(vertsPieceB.first);
		b2Vec2* secondVertexGlobalB = pieceB->getVeterxGlobalCoords(vertsPieceB.second);

		//a test
		/*b2Vec2* firstVertexLocalB = pieceB->getVeterxLocalCoords(vertsPieceB.first);
		b2Vec2* secondVertexLocalB = pieceB->getVeterxLocalCoords(vertsPieceB.second);*/


		connectSpringsToPieces(bodyA, bodyB, firstVertexGlobalA, firstVertexGlobalB);
		connectSpringsToPieces(bodyA, bodyB, secondVertexGlobalA, secondVertexGlobalB);
		
		/*connectSpringsToPieces(bodyA, bodyB, firstVertexLocalA, firstVertexLocalB);
		connectSpringsToPieces(bodyA, bodyB, secondVertexLocalA, secondVertexLocalB);*/


	}
}

void World::explode(int MaxPower, int seed)
{
	int power = sampleIntUniformly(MaxPower, -MaxPower, seed);
	b2Vec2 impulse(power, power);

	for (auto& piece: pieces_)
	{
		piece.refb2Body_->ApplyLinearImpulseToCenter(impulse,true);
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

		for (auto& joint: joints_)
		{
			auto& anchorA = joint->GetAnchorA();
			auto& anchorB = joint->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, 1);
		}

		// for debug
		/*for (b2Contact* contact = world_.GetContactList(); contact; contact = contact->GetNext())
		{
			std::cout << "collide" << contact->GetFixtureA() << std::endl;
		}*/

		int pressedKey = screen_->updateDisplay();

		switch (pressedKey)
		{
		case 'q':
			isFinished = true;
			break;
		case 'e':
			explode(5, -1);
			break;
		default:
			break;
		}

	}

	screen_->finishDisplay();

}
