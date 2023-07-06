#include "reconstruction.h"


Reconstructor::Reconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight)
{
	boardWidth_ = boardWidth;
	boardHeight_ = boardHeight;
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;
	screen_ = new SfmlScreen(screenWidth_, screenHeight_, screenWidth_ / boardWidth_, screenHeight_ / boardHeight_);
}

b2Body* Reconstructor::createPieceBody(Piece& piece, b2BodyDef& bodyDef, b2FixtureDef& fixture)
{
	b2PolygonShape shape;
	shape.Set(piece.localCoordsAsVecs_.data(), piece.localCoordsAsVecs_.size());
	fixture.shape = &shape;
	b2Body* oBody = world_.CreateBody(&bodyDef);
	oBody->CreateFixture(&fixture);

	for (int i = 0; i < piece.localCoordsAsVecs_.size(); i++)
	{
		b2Vec2& localPoint = piece.localCoordsAsVecs_.at(i);
		b2Vec2& globalPoint = oBody->GetWorldPoint(localPoint);
		piece.globalCoordinates_.push_back(globalPoint);
	}

	return oBody;
}

void Reconstructor::initStaticBody(Piece& piece, b2Vec2 &position)
{
	b2BodyDef fixedPieceBodyDef;
	fixedPieceBodyDef.type = b2_staticBody;
	fixedPieceBodyDef.position = position;//b2Vec2(boardWidth_ / 2, boardHeight_ / 2);

	b2FixtureDef fixedPieceFixture;
	fixedPieceFixture.density = 1.0f;
	fixedPieceFixture.friction = 0.5f;
	fixedPieceFixture.filter.groupIndex = 2; //-2; // Don't collide

	b2Body* fixedPieceBody;
	fixedPieceBody = this->createPieceBody(piece, fixedPieceBodyDef, fixedPieceFixture);
	piece.refb2Body_ = fixedPieceBody;
}

void Reconstructor::initMovingBody(Piece& piece, b2Vec2 &initialPosition)
{
	b2Body* body;
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = initialPosition;
	bodyDef.fixedRotation = false;

	b2FixtureDef fixture;
	fixture.density = 1.0f;
	fixture.friction = 0.5f;
	fixture.filter.groupIndex = 2; //-2; // Don't collide

	body = this->createPieceBody(piece, bodyDef, fixture);
	piece.refb2Body_ = body;
}

void Reconstructor::connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB, b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB, float frequencyHertz, float dampingRatio)
{
	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, *globalCoordsAnchorA, *globalCoordsAnchorB);
	jointDef.collideConnected = false;
	jointDef.minLength = 0;//0.05;// 0.1f;
	jointDef.maxLength = boardWidth_;//we have here implicit assumption that the board is squared
	jointDef.length = 0.01;// 0.05;

	b2LinearStiffness(jointDef.stiffness, jointDef.damping, frequencyHertz, dampingRatio, bodyA, bodyB);

	b2DistanceJoint* joint = (b2DistanceJoint*)world_.CreateJoint(&jointDef);
	joints_.push_back(joint);
}

void Reconstructor::putMatingSprings(VertexMating& mating)
{
	Piece* pieceA;
	Piece* pieceB;

	for (auto& piece : activePieces_)
	{
		if (piece.id_ == mating.firstPieceId_)
		{
			pieceA = &piece;
		}

		if (piece.id_ == mating.secondPieceId_)
		{
			pieceB = &piece;
		}
	}

	b2Body* bodyA = pieceA->refb2Body_;
	b2Body* bodyB = pieceB->refb2Body_;

	b2Vec2 vertexGlobalA;
	pieceA->getVeterxGlobalCoords(vertexGlobalA, mating.firstPieceVertex_);
	b2Vec2 vertexGlobalB;
	pieceB->getVeterxGlobalCoords(vertexGlobalB, mating.secondPieceVertex_);
	connectSpringsToPieces(bodyA, bodyB, &vertexGlobalA, &vertexGlobalB);
}

Piece* Reconstructor::getMaxMatingsPiece()
{
	int maxMatings = 0;
	Piece* maxPiece = &activePieces_[0];
	int countMatings = 0;
	int i = 0;

	for (auto& piece : activePieces_)
	{
		countMatings = 0;

		for (auto& mating : activeMatings_)
		{
			if (mating.firstPieceId_ == piece.id_ || mating.secondPieceId_ == piece.id_)
			{
				countMatings++;
			}
		}

		if (countMatings > maxMatings)
		{
			maxPiece = &piece;
			maxMatings = countMatings;
		}

	}

	return maxPiece;
}

void Reconstructor::init()
{
	float wallWidth = 0.1;
	float padding = wallWidth;

	float originX = 0;
	float originY = 0;

	// X_origin,Y_origin,width,height
	const std::vector<std::vector<float>> boundaries{
		{originX,originY, boardWidth_, wallWidth}, // from bottom left to the horizontal line
		{boardWidth_ - padding,originY,wallWidth,boardHeight_}, // from bottom right along the vertical line
		{originX,originY,wallWidth,boardHeight_}, // from bottom left along the vertical line
		{originX,boardHeight_ - padding,boardWidth_,wallWidth} // from top left along the horizontal line
	};

	for (auto& bound = boundaries.begin(); bound != boundaries.end(); bound++)
	{
		float x = bound->at(0);
		float y = bound->at(1);
		b2BodyDef  bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(x, y);
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

void Reconstructor::initRun(std::vector<Piece>& activePieces, std::vector<VertexMating>& activeMatings, int positionSeed, int positionPadding)
{
	// Init Bodies
	activePieces_ = activePieces;
	activeMatings_ = activeMatings;
	Piece* fixedPiece = getMaxMatingsPiece();
	initStaticBody(*fixedPiece, b2Vec2(boardWidth_ / 2, boardHeight_ / 2));

	std::vector<b2Vec2> positions;
	generate2DVectors(positions, activePieces_.size() - 1, boardWidth_, boardHeight_, positionPadding, positionSeed);
	std::sort(positions.begin(), positions.end(), [](const b2Vec2& a, const b2Vec2& b)->bool {
		return a.y > b.y || (a.y == b.y && a.x > b.x);
	});
	auto& initialPosIt = positions.begin();

	for (auto& piece:activePieces_)
	{
		if (piece.id_ != fixedPiece->id_)
		{
			initMovingBody(piece, *initialPosIt);
			++initialPosIt;
		}
	}

	// Apply impulse on bodies
	int impulseIndex = 0;
	float powerMagnitude = 0.2;
	std::vector<b2Vec2> initialImpulses = {
		{powerMagnitude,powerMagnitude},
		{-powerMagnitude,powerMagnitude},
		{-powerMagnitude,-powerMagnitude},
		{-powerMagnitude,powerMagnitude}
	};
	int numInitialImpulses = initialImpulses.size();

	for (auto& piece : activePieces_)
	{
		piece.setCollideOff();
		piece.setAngularDamping(0.01);
		auto& power = initialImpulses[++impulseIndex % numInitialImpulses];
		piece.applyLinearImpulse(power.x,power.y);
	}

	for (auto &mating:activeMatings_)
	{
		putMatingSprings(mating);
	}
}

void Reconstructor::closeRun()
{
	for (auto& joint:joints_)
	{
		world_.DestroyJoint(joint);
	}

	for (int i=0;i<activePieces_.size();++i)
	{
		activePieces_[i].DestroyBody();
	}

	activePieces_.clear();
	joints_.clear();
	activeMatings_.clear();
}

