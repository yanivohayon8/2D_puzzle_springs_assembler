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

void Reconstructor::putMatingSprings(VertexMatingRePAIR* &mating)
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

	pieceA->getGlobalCoords(vertexGlobalA, mating->firstPieceLocalCoords_);//*debug_coord_A);
	pieceB->getGlobalCoords(vertexGlobalB, mating->secondPieceLocalCoords_);
	

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

void Reconstructor::initBoundaryWallBodies()
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
		boundsWallBodies_.push_back(body);
	}
}

void Reconstructor::closeRun()
{
	for (auto& body: boundsWallBodies_)
	{
		world_.DestroyBody(body);
	}

	boundsWallBodies_.clear();

	for (auto& mating:activeMatings_)
	{
		world_.DestroyJoint(mating->jointRef_);
		delete mating;
	}
	
	activeMatings_.clear();

	for (int i=0;i<activePieces_.size();++i)
	{
		activePieces_[i].DestroyBody();
	}

	activePieces_.clear();
	

	if (isScreenInitiated_)
	{
		screen_->closeWindow();
		isScreenInitiated_ = false;
	}

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




/// From here new functions of refactor
void Reconstructor::setBoardWidth(float boardWidth) { boardWidth_ = boardWidth; }
void Reconstructor::setBoardHeight(float boardHeight) { boardHeight_ = boardHeight; }
void Reconstructor::setScreenWidth(float screenWidth) { screenWidth_ = screenWidth; }
void Reconstructor::setScreenHeight(float screenHeight) { screenHeight_ = screenHeight; }

void Reconstructor::updateScreen()
{
	delete screen_;
	screen_ = new SfmlScreen(screenWidth_, screenHeight_, screenWidth_ / boardWidth_, screenHeight_ / boardHeight_);
}

void Reconstructor::initPiecesBodies(std::vector<Piece>& activePieces, std::string fixedPieceId, std::vector<b2Vec2>& positions)
{

	fixedPiece_ = &activePieces_[0];

	for (int i = 0; i < activePieces_.size(); i++)
	{

		if (activePieces_[i].id_ == fixedPieceId)
		{
			initStaticBody(activePieces_[i], b2Vec2(boardWidth_ / 2, boardHeight_ / 2));
			fixedPiece_ = &activePieces_[i]; // useless? 
		}
		else
		{
			initMovingBody(activePieces_[i], positions[i]);
		}
	}
}

void Reconstructor::initMatingsJoints(std::vector<VertexMatingRePAIR*>& activeMatings)
{
	activeMatings_.clear();

	for (auto& mating : activeMatings)
	{
		activeMatings_.push_back(mating);
		putMatingSprings(mating);
	}
}


void Reconstructor::initScreenNew(bool isScreenVisible)
{
	// The screen is not visible if we want to take a screen shot at the end
	screen_->initDisplay(isScreenVisible);

	for (auto& piece : activePieces_)
	{
		if (piece.imagePath_ != "")
		{
			screen_->initSprite(piece);
		}

		screen_->initPolygon(piece);
	}

	nextScreenPolygonColorIndex_ = 0;

	screen_->clearDisplay();
	isScreenInitiated_ = true;
}


void Reconstructor::initRunNew(httplib::Request currentRequest, std::vector<Piece> activePieces, std::vector<VertexMatingRePAIR*> activeMatings)
{
	initBoundaryWallBodies();

	activePieces_ = activePieces;
	std::string fixedPieceId = "";

	if (currentRequest.has_param("fixPiece"))
	{
		Piece* fixedPiece = getMaxMatingsPiece();
		fixedPieceId = fixedPiece->id_;
	}

	std::vector<b2Vec2> positions;

	if (currentRequest.has_param("seedInitialPositions"))
	{
		int positionPadding = 2;
		int seedPositions = std::stoi(currentRequest.get_param_value("seedInitialPositions"));

		generate2DVectors(positions, activePieces_.size(), boardWidth_, boardHeight_,
			positionPadding, seedPositions);

		std::sort(positions.begin(), positions.end(),
			[](const b2Vec2& a, const b2Vec2& b)->bool {
				return a.y > b.y || (a.y == b.y && a.x > b.x);
			});
	}
	else
	{
		for (int i = 0; i < activePieces_.size(); i++)
		{
			positions.push_back(b2Vec2(boardWidth_ / 2, boardHeight_ / 2));
		}
	}

	initPiecesBodies(activePieces_, fixedPieceId, positions);
	applyImpulseOnBodies(initPowerMagnitude_);

	enableJointsCollide();

	if (currentRequest.has_param("disableJointsCollide"))
	{
		disableJointsCollide();
	}

	initMatingsJoints(activeMatings);

	if (currentRequest.has_param("isDrawOnlyPolygons"))
	{
		isDrawOnlyPolygons = true;
	}
}


void Reconstructor::progress(int numIteration)
{
	int iteration = 0;

	while (++iteration < numIteration)
	{
		world_.Step(timeStep_, velocityIterations_, positionIterations_);

		for (auto pieceIt = activePieces_.begin(); pieceIt != activePieces_.end(); pieceIt++)
		{
			pieceIt->translate();
		}

		if (isDebugScreenVisible_)
		{
			screen_->clearDisplay();

			drawPieces();
			drawJoints();

			screen_->updateDisplay();
		}
	}
}


void Reconstructor::drawJoints()
{
	for (auto& mating : activeMatings_)
	{
		auto& anchorA = mating->jointRef_->GetAnchorA();
		auto& anchorB = mating->jointRef_->GetAnchorB();
		screen_->drawLine(anchorA, anchorB, jointColor_, -1);
	}
}

void Reconstructor::drawPieces()
{
	for (auto pieceIt = activePieces_.begin(); pieceIt != activePieces_.end(); pieceIt++)
	{
		const b2Transform& transform = pieceIt->refb2Body_->GetTransform();

		if (isDrawOnlyPolygons)
		{
			screen_->drawPolygon(pieceIt->id_, transform);
		}
		else
		{
			bool isSpriteAvailable = screen_->drawSprite(pieceIt->id_, transform);

			if (!isSpriteAvailable)
			{
				screen_->drawPolygon(pieceIt->id_, transform);
			}
		}
	}
}

void Reconstructor::saveScreenShot(std::string screenshotPath)
{
	screen_->clearDisplay();

	drawJoints();
	drawPieces();

	screen_->screenShotToFile(screenshotPath);
}