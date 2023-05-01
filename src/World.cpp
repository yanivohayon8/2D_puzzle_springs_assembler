#include "World.h"

World::World(std::vector<Piece>& pieces,std::vector<VertexMating>& matings)
{
	matings_ = matings;
	rawPieces_ = pieces;
}

b2Body* World::createPieceBody(Piece& piece,b2Vec2& initialPosition)
{

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = initialPosition; //b2Vec2{ 3, 2.5};
	bodyDef.fixedRotation = piece.isRotationFixed;
	//bodyDef.linearVelocity.Set(1, 0); // debug

	b2PolygonShape shape;
	/*std::vector<b2Vec2>& b2Poly = piece.localCoordsAsVecs_;
	int numCoords = piece.getNumCoords();

	for (int i = 0; i < numCoords; i++)
	{
		auto [x, y] = piece.getVertexCoord(i);
		float x_ = static_cast<float>(x);
		float y_ = static_cast<float>(y);
		b2Poly.push_back(b2Vec2{ x_,y_});
	}*/
	
	shape.Set(piece.localCoordsAsVecs_.data(), piece.localCoordsAsVecs_.size());

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
	fixture.friction = 0.5f;
	fixture.filter.groupIndex = 2; //-2; // Don't collide

	b2Body* oBody = world_.CreateBody(&bodyDef);
	oBody->CreateFixture(&fixture);

	for (int i = 0; i < piece.localCoordsAsVecs_.size(); i++)
	{
		b2Vec2& localPoint = piece.localCoordsAsVecs_.at(i);
		b2Vec2& globalPoint = oBody->GetWorldPoint(localPoint);
		piece.globalCoordinates_.push_back(globalPoint);
	}

	// Setting the mass of all the pieces to prevent overlapping of pieces
	// https://www.iforce2d.net/b2dtut/joints-overview (last paragraph)
	/*b2MassData mass = oBody->GetMassData();
	mass.mass = 2;
	oBody->SetMassData(&mass);*/

	return oBody;
}


void World::Init()
{
	//preProcess();
	initBounds();
	InitPieces();
	orderSpringsConnection();
}

void World::initBounds()
{
	float wallWidth = 0.1;
	screen_ = new Screen(screenHeight_, screenWidth_, screenScale_);

	float originX = 0;
	float originY = 0;
	
	const std::vector<std::vector<float>> boundaries{ 
		{originX,originY, boardWidth_, wallWidth}, // from bottom left to the horizontal line
		{boardWidth_,originY,wallWidth,boardHeight_}, // from bottom right along the vertical line
		{originX,originY,wallWidth,boardHeight_}, // from bottom left along the vertical line
		{originX,boardHeight_,boardWidth_,wallWidth} // from top left along the horizontal line
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

void World::InitPieces()
{
	
	std::vector<b2Vec2> positions;
	int seed = 0;
	int padding = 2;
	generate2DVectors(positions, rawPieces_.size(), boardWidth_, boardHeight_,padding, seed);
	auto& initialPosIt = positions.begin();

	for (auto pieceIt = rawPieces_.begin(); pieceIt != rawPieces_.end(); pieceIt++)
	{
		b2Body* body;

		//// Box2d limitations
		//if (pieceIt->getNumCoords()>7)
		//{
		//	std::vector<std::vector<b2Vec2>> triangles;
		//	pieceIt->triangulated(triangles);
		//	std::cout << "here" << std::endl;
		//}
		//else
		//{
		//	body = this->createPieceBody(*pieceIt, *initialPosIt);
		//}
		body = this->createPieceBody(*pieceIt, *initialPosIt);

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

}

void World::connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB,
	b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB,
	float minLength,float maxLength, float damping, float stiffness)
{
	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, *globalCoordsAnchorA, *globalCoordsAnchorB);
	jointDef.collideConnected = true;
	jointDef.minLength = minLength; //0.05f; //0.05f;
	jointDef.maxLength = maxLength;//0.2f; //0.5f;
	jointDef.damping = damping; //0.3f; //1.0f;
	jointDef.stiffness = stiffness; //0.5f;
	b2DistanceJoint* joint = (b2DistanceJoint*)world_.CreateJoint(&jointDef);
	joints_.push_back(joint);
}


void World::putMatingSprings(VertexMating& mating)
{
	Piece* pieceA = &pieces_.at(mating.firstPieceId_);
	Piece* pieceB = &pieces_.at(mating.secondPieceId_);

	b2Body* bodyA = pieceA->refb2Body_;
	b2Body* bodyB = pieceB->refb2Body_;

	b2Vec2 vertexGlobalA;
	pieceA->getVeterxGlobalCoords(vertexGlobalA, mating.firstPieceVertex_);
	b2Vec2 vertexGlobalB;
	pieceB->getVeterxGlobalCoords(vertexGlobalB, mating.secondPieceVertex_);
	connectSpringsToPieces(bodyA, bodyB, &vertexGlobalA, &vertexGlobalB);

}

void World::orderSpringsConnection()
{
	std::vector<VertexMating> orderedMatings;

	for (auto& piece:pieces_)
	{
		for (auto &matingIt :matings_)
		{
			if (matingIt.firstPieceId_ == piece.id_ || matingIt.secondPieceId_ == piece.id_)
			{

				if (std::find(orderedMatings.begin(), orderedMatings.end(),matingIt) == orderedMatings.end())
				{
					orderedMatings.push_back(matingIt);
				}
			}

		}
	}

	matings_ = orderedMatings;
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

void World::switchColide(b2Body* body)
{
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	filter.groupIndex = filter.groupIndex * -1;
	body->GetFixtureList()->SetFilterData(filter);
}

void World::setCollideOff(b2Body* body)
{
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	if (filter.groupIndex > 0)
	{
		filter.groupIndex = filter.groupIndex * -1;
	}
	body->GetFixtureList()->SetFilterData(filter);
}

void World::setCollideOn(b2Body* body)
{
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	if (filter.groupIndex < 0)
	{
		filter.groupIndex = filter.groupIndex * -1;
	}
	body->GetFixtureList()->SetFilterData(filter);
}


void World::setDamping(b2Body* body, double linearDamping,double angularDamping)
{
	body->SetLinearDamping(linearDamping);
	body->SetAngularDamping(angularDamping);
}

void World::Simulation(bool isAuto)
{

	// The following params make as parameters to the function
	double timeStep = 1.0F / 60.0F;//1.0F / 300.0F; //1.0F / 60.0F;
	int velocityIterations = 6;
	int positionIterations = 2;
	bool isFinished = false;
	float damping = 0;
	cv::Scalar redColor = { 0,0,255 };
	cv::Scalar greenColor = { 0,255,0 };
	//SpringMating* nextSpring;

	int nIteration = 0;
	bool isJointShorted = false;

	screen_->initDisplay();

	if (isAuto)
	{
		explode(1, 0);

	}


	while (!isFinished)
	{
		screen_->clearDisplay();
		screen_->drawBounds(&boundsCoordinates_);

		world_.Step(timeStep, velocityIterations, positionIterations);

		for (auto pieceIt = pieces_.begin(); pieceIt != pieces_.end(); pieceIt++)
		{
			pieceIt->translate();
			screen_->drawPolygon(pieceIt->globalCoordinates_, pieceIt->color_);
			const b2Transform& transform = pieceIt->refb2Body_->GetTransform();
			//screen_->drawCircle(transform.p, 3, redColor);
			screen_->writeText(std::to_string(pieceIt->id_), transform.p);
		}

		for (auto& joint : joints_)
		{
			auto& anchorA = joint->GetAnchorA();
			auto& anchorB = joint->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, 1);
		}

		//// for debug
		//b2Vec2 centerOfMass = getCenterOfMass(pieces_);
		//screen_->drawCircle(centerOfMass, 6, greenColor);


		// for debug
		/*for (b2Contact* contact = world_.GetContactList(); contact; contact = contact->GetNext())
		{
			std::cout << "collide" << contact->GetFixtureA() << std::endl;
		}*/

		int pressedKey = screen_->updateDisplay();

		if (isAuto)
		{

		

			if (nIteration % 60 == 0)
			{
				// Still connecting the springs
				if (connectedSpringIndex_ < int(matings_.size()))
				{
					putMatingSprings(matings_[connectedSpringIndex_]);
					++connectedSpringIndex_;
				}
				else {
					// Shorting the springs
					if (!isJointShorted)
					{
						for (auto& joint : joints_)
						{
							joint->SetMinLength(0.01);
							joint->SetMaxLength(0.05);
						}
						isJointShorted = true;
					}
					else {

						// start to slow down
						double AveragedSpeed = 0;

						for (auto& piece : pieces_)
						{
							AveragedSpeed += piece.refb2Body_->GetLinearVelocity().Length();
						}

						AveragedSpeed /= pieces_.size();
						double speedEpsilon = 0.1;

						if (AveragedSpeed < speedEpsilon)
						{
							isFinished = true;
							continue;
						}


						damping += 0.1;
						for (auto& piece : pieces_)
						{
							setDamping(piece.refb2Body_, damping, damping);
						}
					}
				}

			}
		}

		switch (pressedKey)
		{
		case 'c':
			for (auto& piece : pieces_)
			{
				switchColide(piece.refb2Body_);
			}
			break;
		case 'e':
			explode(5, 0);
			break;
		case 'E':
			explode(50, -1);
			break;
		case 'd':
			damping += 0.1;
			for (auto& piece : pieces_)
			{
				setDamping(piece.refb2Body_, damping, damping);
			}
			break;
		case 'D':
			damping -= 0.1;
			if (damping < 0)
			{
				damping = 0;
			}
			for (auto& piece : pieces_)
			{
				setDamping(piece.refb2Body_, damping, damping);
			}
			break;
		case 'm':
			if (connectedSpringIndex_ < int(matings_.size()))
			{
				putMatingSprings(matings_[connectedSpringIndex_]);
				++connectedSpringIndex_;
			}
			break;
		case 's':
			for (auto& joint : joints_)
			{
				auto length = joint->GetMinLength();
				if (length>0.05)
				{
					joint->SetMinLength(length-0.1);
				}
				//joint->SetMaxLength(0.05);
			}
			break;
		case 'S':
			for (auto& joint : joints_)
			{

				//joint->SetMinLength(joint->GetMinLength() - 0.1);
				auto length = joint->GetMaxLength();
				if (length>0.2f)
				{

					joint->SetMaxLength(length-0.1);
				}
			}
			break;
		case 'q':
			isFinished = true;
			break;
		default:
			break;
		}

		nIteration++;
	}

	screen_->finishDisplay();

	b2Vec2 centerOfMass = getCenterOfMass(pieces_);

	for (auto& piece:pieces_ )
	{
		piece.finalCoordinates_ = piece.localCoordinates_;
		const b2Transform& transform = piece.refb2Body_->GetTransform();
		Eigen::MatrixX2d rotation(2,2);
		b2Vec2 finalTranslate = transform.p - centerOfMass;

		piece.finalRot_ = transform.q;
		piece.finalTranslate_ = finalTranslate;
		
		getRoatationMatrix(rotation, transform.q);
		piece.finalCoordinates_*= rotation;
		piece.finalCoordinates_ = piece.finalCoordinates_.rowwise() + Eigen::RowVector2d(finalTranslate.x,finalTranslate.y);
	}
}


b2Vec2 World::getCenterOfMass(std::vector<Piece>& pieces)
{
	// Compute total mass
	float totalMass = 0.f;
	for (auto& piece : pieces) {
		b2Body* body = piece.refb2Body_;
		totalMass += body->GetMass();
	}

	b2Vec2 centreOfMass(0.f, 0.f);

	for (auto& piece : pieces) {
		b2Body* body = piece.refb2Body_;
		b2Vec2 r = body->GetWorldCenter();
		float m = body->GetMass();
		float weight = m / totalMass;
		centreOfMass += weight * r;
	}

	return centreOfMass;
}


void World::saveFinalTransforms(const std::string& filename)
{
	// Open the CSV file for writing
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	// Write the header
	file << "piece,t_x,t_y,r_sin,r_cos" << std::endl;

	// Write each row of the matrix as a separate line in the CSV file
	for (auto& piece : pieces_)
	{
		file << std::to_string(piece.id_) << "," << piece.finalTranslate_.x << "," << piece.finalTranslate_.y << "," << piece.finalRot_.s << "," << piece.finalRot_.c << std::endl;
	}

	// Close the file
	file.close();

	std::cout << "Matrix data written to CSV file: " << filename << std::endl;
}

void World::saveFinalCoordinates(const std::string& filename)
{
	// Open the CSV file for writing
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	// Write the header
	file << "piece,x,y" << std::endl;

	// Write each row of the matrix as a separate line in the CSV file
	for (auto& piece:pieces_)
	{
		for (int i = 0; i < piece.finalCoordinates_.rows(); ++i) {
			file << std::to_string(piece.id_) <<"," << piece.finalCoordinates_(i, 0) << "," << piece.finalCoordinates_(i, 1) << std::endl;
		}
	}

	// Close the file
	file.close();

	std::cout << "Matrix data written to CSV file: " << filename << std::endl;
}