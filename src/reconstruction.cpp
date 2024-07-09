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
	fixedPieceBodyDef.angle = piece.initialAngle;

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
	bodyDef.fixedRotation = piece.isRotationFixed; // = false;
	bodyDef.angle = piece.initialAngle;

	b2FixtureDef fixture;
	fixture.density = 1.0f;
	fixture.friction = 0.5f;
	fixture.filter.groupIndex = 2; //-2; // Don't collide

	body = this->createPieceBody(piece, bodyDef, fixture);
	piece.refb2Body_ = body;
}

void Reconstructor::putMatingSprings(VertexMating* &mating)
{
	Piece* pieceA;
	Piece* pieceB;

	for (auto& piece : activePieces_)
	{
		if (piece.id_ == mating->firstPieceId_)
		{
			pieceA = &piece;
		}

		if (piece.id_ == mating->secondPieceId_)
		{
			pieceB = &piece;
		}
	}

	b2Body* bodyA = pieceA->refb2Body_;
	b2Body* bodyB = pieceB->refb2Body_;
	b2Vec2 vertexGlobalA;
	b2Vec2 vertexGlobalB;

	VertexMatingRePAIR* matingRepair_ptr = dynamic_cast<VertexMatingRePAIR*>(mating);

	if (matingRepair_ptr!=nullptr)
	{
		//REPAIR
		pieceA->getGlobalCoords(vertexGlobalA, matingRepair_ptr->firstPieceLocalCoords_);//*debug_coord_A);
		pieceB->getGlobalCoords(vertexGlobalB, matingRepair_ptr->secondPieceLocalCoords_);
	}
	else
	{
		// Convex Drawing
		pieceA->getVeterxGlobalCoords(vertexGlobalA, mating->firstPieceVertex_);	
		pieceB->getVeterxGlobalCoords(vertexGlobalB, mating->secondPieceVertex_);
	}


	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, vertexGlobalA, vertexGlobalB);
	jointDef.collideConnected = isEnableJointsCollide_;//true;//false; //true;//false; // true makes the correct matings "jitter"?
	jointDef.minLength = jointMinLength_; //0;
	jointDef.maxLength = boardWidth_;//we have here implicit assumption that the board is squared
	jointDef.length = jointRestLength_;//0.01;

	b2LinearStiffness(jointDef.stiffness, jointDef.damping, jointFrequencyHertz_, jointDampingRatio_, bodyA, bodyB);

	mating->jointRef_ = (b2DistanceJoint*)world_.CreateJoint(&jointDef);
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
			if (mating->firstPieceId_ == piece.id_ || mating->secondPieceId_ == piece.id_)
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

void Reconstructor::initRun(std::vector<Piece>& activePieces, std::vector<VertexMating*>& activeMatings, int positionSeed, int positionPadding)
{
	// Init Bodies
	activePieces_ = activePieces;
	//activeMatings_ = activeMatings;

	for (auto& mating:activeMatings)
	{
		activeMatings_.push_back(mating);
	}

	fixedPiece_ = getMaxMatingsPiece();
	initStaticBody(*fixedPiece_, b2Vec2(boardWidth_ / 2, boardHeight_ / 2));
	//initMovingBody(*fixedPiece_, b2Vec2(boardWidth_ / 2, boardHeight_ / 2));

	std::vector<b2Vec2> positions;
	generate2DVectors(positions, activePieces_.size() - 1, boardWidth_, boardHeight_, positionPadding, positionSeed);
	std::sort(positions.begin(), positions.end(), [](const b2Vec2& a, const b2Vec2& b)->bool {
		return a.y > b.y || (a.y == b.y && a.x > b.x);
	});
	auto& initialPosIt = positions.begin();

	for (auto& piece:activePieces_)
	{
		if (piece.id_ != fixedPiece_->id_)
		{
			//initMovingBody(piece, *initialPosIt);
			initMovingBody(piece, b2Vec2(boardWidth_ / 2, boardHeight_ / 2));
			//initMovingBody(piece, b2Vec2(8, 5));
			//initMovingBody(piece, b2Vec2(8, 8));

			//initMovingBody(piece, b2Vec2(1, 1));
			++initialPosIt;
		}
	}

	// Apply impulse on bodies
	int impulseIndex = 0;
	//float powerMagnitude = 0.2;//2
	std::vector<b2Vec2> initialImpulses = {
		{initPowerMagnitude_,initPowerMagnitude_},
		{-initPowerMagnitude_,initPowerMagnitude_},
		{-initPowerMagnitude_,-initPowerMagnitude_},
		{-initPowerMagnitude_,initPowerMagnitude_}
	};
	int numInitialImpulses = initialImpulses.size();

	for (auto& piece : activePieces_)
	{
		piece.setCollideOff();
		//piece.setAngularDamping(pieceAngularDamping_);
		auto& power = initialImpulses[++impulseIndex % numInitialImpulses];
		piece.applyLinearImpulse(power.x,power.y);
	}

	setPiecesAngularDamping(pieceAngularDamping_);

	for (auto &mating:activeMatings_)
	{
		putMatingSprings(mating);
	}

	piecesOverlappingArea_ = -1;
}

void Reconstructor::closeRun()
{
	/*for (auto& joint:joints_)
	{
		world_.DestroyJoint(joint);
	}*/

	for (auto& mating:activeMatings_)
	{
		world_.DestroyJoint(mating->jointRef_);
		delete mating;
	}

	

	for (int i=0;i<activePieces_.size();++i)
	{
		activePieces_[i].DestroyBody();
	}

	activePieces_.clear();
	//joints_.clear();
	
	/*for (auto& mating: activeMatings_)
	{
		delete& mating;
	}*/
	
	activeMatings_.clear();

	std::cout << "Run closed" << std::endl;
}



void Reconstructor::disableJointsCollide()
{
	isEnableJointsCollide_ = false;
}

void Reconstructor::enableJointsCollide()
{
	isEnableJointsCollide_ = true;

}

void Reconstructor::setPiecesLinearDamping(float damping)
{
	for (auto& piece : activePieces_)
	{
		piece.setLinearDamping(damping);
	}
}

void Reconstructor::setPiecesAngularDamping(float damping)
{
	for (auto& piece : activePieces_)
	{
		piece.setLinearDamping(damping);
	}
}

void Reconstructor::setPiecesCollisionOn()
{
	for (auto& piece : activePieces_)
	{
		piece.setCollideOn();
	}
}
void Reconstructor::setPiecesCollisionOff()
{
	for (auto& piece : activePieces_)
	{
		piece.setCollideOff();
	}
}

void Reconstructor::applyImpulseOnBodies(float powerMagnitude)
{

	std::vector<b2Vec2> initialImpulses = {
		{powerMagnitude,-powerMagnitude},
		{powerMagnitude,powerMagnitude},
		{-powerMagnitude,-powerMagnitude},
		{-powerMagnitude,powerMagnitude}
	};

	int numInitialImpulses = initialImpulses.size();
	int impulseIndex = 0;

	for (auto& piece : activePieces_)
	{
		auto& power = initialImpulses[++impulseIndex % numInitialImpulses];
		piece.applyLinearImpulse(power.x, power.y);
	}
}

void Reconstructor::setJointRestLength(float length)
{
	jointRestLength_ = length;
}

void Reconstructor::setJointMinLength(float minlength)
{
	jointMinLength_ = minlength;
}

void Reconstructor::setInitPowerMagnitude(float magnitude)
{
	initPowerMagnitude_ = magnitude;
}

void Reconstructor::setJointFrequency(float herz)
{
	jointFrequencyHertz_ = herz;
}

void Reconstructor::setJointDamping(float ratio)
{
	jointDampingRatio_ = ratio;
}