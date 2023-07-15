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

void Reconstructor::putMatingSprings(VertexMating& mating, float frequencyHertz, float dampingRatio)
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

	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, vertexGlobalA, vertexGlobalB);
	jointDef.collideConnected = false; //true;//false; // true makes the correct matings "jitter"?
	jointDef.minLength = 0;//0.05;// 0.1f;
	jointDef.maxLength = boardWidth_;//we have here implicit assumption that the board is squared
	jointDef.length = 0.01;// 0.05;

	b2LinearStiffness(jointDef.stiffness, jointDef.damping, frequencyHertz, dampingRatio, bodyA, bodyB);

	mating.jointRef_ = (b2DistanceJoint*)world_.CreateJoint(&jointDef);
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
	fixedPiece_ = getMaxMatingsPiece();
	initStaticBody(*fixedPiece_, b2Vec2(boardWidth_ / 2, boardHeight_ / 2));

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
		world_.DestroyJoint(mating.jointRef_);
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
}

void Reconstructor::computePiecesBoostPolygons()
{
	for (auto& piece : activePieces_)
	{
		piece.initBoostPolygon();
		//totalArea += piece.computeArea();
	}
}

//float Reconstructor::computeOverlapAreaDiceCoeff()
//{
//	/// <summary>
//	/// This is a variant to the dice coeff 
//	/// </summary>
//	/// <returns></returns>
//	
//	std::vector<Piece> piecesToUnion;
//	std::vector<BoostPolygon> unionedPolys;
//	float sum = 0;
//	float overlapArea;
//	float currPieceArea;
//
//	for (int i = 0; i < activePieces_.size(); i++)
//	{
//
//		piecesToUnion.clear();
//
//		for (int j = 0; j < activePieces_.size(); j++)
//		{
//			if (i!=j)
//			{
//				piecesToUnion.push_back(activePieces_[j]);
//			}
//		}
//
//		unionedPolys.clear();
//		computeUnionBoostPolygon(unionedPolys, piecesToUnion);
//		overlapArea = 0;//
//
//		for (auto& poly: unionedPolys)
//		{
//			overlapArea += activePieces_[i].computeOverlappingArea(poly);
//		}
//
//		currPieceArea = activePieces_[i].computeArea();
//
//		sum += overlapArea / (currPieceArea+1e-5);
//
//	}
//
//	return sum;
//}
//
//float Reconstructor::computePiecesOverlapAreaPercentage()
//{
//	float totalArea = 0;
//
//	for (int i = 0; i < activePieces_.size(); i++)
//	{
//		totalArea += activePieces_[i].computeArea();
//	}
//
//	float overlappingArea = 0;
//
//	for (int i = 0; i < activePieces_.size(); i++)
//	{
//		for (int j = i + 1; j < activePieces_.size(); j++)
//		{
//			overlappingArea += activePieces_[i].computeOverlappingArea(activePieces_[j].boostPolygonGlobalCoords_);
//		}
//	}
//
//	return overlappingArea / totalArea * 100;
//}

float Reconstructor::getPiecesOverlappingArea()
{
	return piecesOverlappingArea_;
}



void Reconstructor::saveFinalTransforms(const std::string& filename,const b2Vec2& translateCenter)
{
	// Open the CSV file for writing
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	// Write the header
	//file << "piece,t_x,t_y,r_sin,r_cos,r_angle" << std::endl;
	file << "piece,translate_x,translate_y,rotation_angle" << std::endl;

	// Write each row of the matrix as a separate line in the CSV file
	for (auto& piece : activePieces_)
	{
		auto& trans = piece.refb2Body_->GetTransform();
		float x = trans.p.x - translateCenter.x;
		float y = trans.p.y - translateCenter.y;
		float radians = piece.refb2Body_->GetAngle();

		//file << piece.id_ << "," << trans.p.x << "," << trans.p.y << "," << trans.q.s << "," << trans.q.c<<  << std::endl;
		file << piece.id_ << "," << x << "," << y << "," << radians << std::endl;
	}

	// Close the file
	file.close();

	std::cout << "Matrix data written to CSV file: " << filename << std::endl;
}