#include "World.h"

World::World(std::vector<Piece>& pieces,std::vector<VertexMating>& matings, std::vector<VertexMating>& groundTruthMatings)
{
	matings_ = matings;
	groundTruthMatings_ = groundTruthMatings;
	rawPieces_ = pieces;
	screen_ = new SfmlScreen(screenWidth_,screenHeight_, screenWidth_ / boardWidth_,screenHeight_ / boardHeight_);
}

b2Body* World::createPieceBody(Piece& piece, b2BodyDef& bodyDef, b2FixtureDef& fixture)
{

	
	b2PolygonShape shape;
	std::vector<b2Vec2> localCoords;

	for (int i = 0; i < piece.localCoordsAsVecs_.size(); i++)
	{
		float xFactored = piece.localCoordsAsVecs_[i].x;
		float yFactored = piece.localCoordsAsVecs_[i].y;
		localCoords.push_back(b2Vec2(xFactored, yFactored));
	}

	shape.Set(localCoords.data(), localCoords.size());

	/*if (is_polygon_degenerate(points)) {
		s.SetAsBox(b2_linearSlop * 2,
			b2_linearSlop * 2);
	}
	else {
		s.Set(points.data(), points.size());
	}*/

	fixture.shape = &shape;
	b2Body* oBody = world_.CreateBody(&bodyDef);
	oBody->CreateFixture(&fixture);

	for (int i = 0; i < localCoords.size(); i++)
	{
		b2Vec2& localPoint = localCoords.at(i);
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
	initBounds();
	InitPieces();
	orderSpringsConnection();
}

void World::initBounds()
{
	float wallWidth = 0.1;
	float padding = wallWidth;

	float originX = 0;
	float originY = 0;
	
	// X_origin,Y_origin,width,height
	const std::vector<std::vector<float>> boundaries{ 
		{originX,originY, boardWidth_, wallWidth}, // from bottom left to the horizontal line
		{boardWidth_-padding,originY,wallWidth,boardHeight_}, // from bottom right along the vertical line
		{originX,originY,wallWidth,boardHeight_}, // from bottom left along the vertical line
		{originX,boardHeight_-padding,boardWidth_,wallWidth} // from top left along the horizontal line
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

	std::sort(positions.begin(), positions.end(), [](const b2Vec2& a, const b2Vec2& b)->bool {
		return a.y > b.y || (a.y==b.y && a.x > b.x);
		});

	auto& initialPosIt = positions.begin();

	for (auto pieceIt = rawPieces_.begin(); pieceIt != rawPieces_.end(); pieceIt++)
	{
		b2Body* body;
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = *initialPosIt;
		bodyDef.fixedRotation = false;//piece.isRotationFixed;

		b2FixtureDef fixture;
		fixture.density = 1.0f;
		fixture.friction = 0.5f;
		fixture.filter.groupIndex = 2; //-2; // Don't collide

		body = this->createPieceBody(*pieceIt, bodyDef,fixture);
		//body = this->createPieceBody(*pieceIt, b2Vec2(5,5));
		pieceIt->refb2Body_ = body;
		
		pieceIt->computeBoundingBox();


		pieces_.push_back(*pieceIt);
		initialPosIt++;
	}
}

void World::connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB,
	b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB,
	float minLength,float maxLength, float damping, float stiffness)
{
	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, *globalCoordsAnchorA, *globalCoordsAnchorB);
	//jointDef.collideConnected = true;
	jointDef.collideConnected = true;
	jointDef.minLength = 0;//0.05;// 0.1f;
	jointDef.maxLength = boardWidth_;//we have here implicit assumption that the board is squared
	jointDef.length = 0.01;// 0.05;
	
	// the stifness correponds to the score of the pairwise?
	//jointDef.stiffness = stiffness;
	//jointDef.damping = damping;
	
	// more natural springs

	float frequencyHertz = 0.01;//1.5;//0.5f;//5;//0.5f; // "Speed of oscillation" 1-5 typical. if it is lower then it is stiffer?
	float dampingRatio = 0.01; //1;//0.1f; // typical 0-1, at 1 all oscillation vanish
	b2LinearStiffness(jointDef.stiffness, jointDef.damping, frequencyHertz, dampingRatio, bodyA, bodyB);
	
	b2DistanceJoint* joint = (b2DistanceJoint*)world_.CreateJoint(&jointDef);
	joints_.push_back(joint);
}

void World::switchJointCollide(b2DistanceJoint& joint)
{
	//joint.
	//joint.
}

void World::putMatingSprings(VertexMating& mating)
{

	Piece* pieceA;
	Piece* pieceB;

	for (auto& piece: pieces_)
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
	int i = 0;
	for (auto& piece: pieces_)
	{
		int power = sampleIntUniformly(MaxPower, -MaxPower, i);
		b2Vec2 impulse(power, power);
		piece.refb2Body_->ApplyLinearImpulseToCenter(impulse,true);
		i++;
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
	double timeStep = 1 / 60.0F; //1.0F / 60.0F;
	int velocityIterations = 6;
	int positionIterations = 2;
	bool isFinished = false;
	float damping = 0;
	auto redColor = sf::Color::Red;
	int nIteration = -1;
	bool isMovedToOrigin = false;

	screen_->initDisplay();
	screen_->initBounds(boundsCoordinates_);

	int impulseIndex = 0;
	float power = 0.2;
	std::vector<b2Vec2> initialImpulses = {
		{power,power},
		{-power,power},
		{-power,-power},
		{-power,power}
	};
	int numInitialImpulses = initialImpulses.size();

	for (auto&piece:pieces_)
	{
		screen_->initSprite(piece);
		screen_->initPolygon(piece);
		screen_->initPolygonCoordsDots(piece, 0.01, sf::Color(0, 255,0 ));
		setCollideOff(piece.refb2Body_);
		//setDamping(piece.refb2Body_, 0, 0.05); // to prevent the bodies spining like centrifugot
		piece.refb2Body_->ApplyLinearImpulseToCenter(initialImpulses[++impulseIndex%numInitialImpulses], true);
	}

	/*if (isAuto)
	{*/
		while (connectedSpringIndex_ < int(matings_.size()))
		{
			putMatingSprings(matings_[connectedSpringIndex_]);
			++connectedSpringIndex_;
		}
	//}

	//while ((!isFinished || !isAuto) && screen_->isWindowOpen())
	while (screen_->isWindowOpen())
	{
		nIteration++;

		screen_->clearDisplay();
		screen_->drawBounds();
		world_.Step(timeStep, velocityIterations, positionIterations);

		for (auto pieceIt = pieces_.begin(); pieceIt != pieces_.end(); pieceIt++)
		{
			pieceIt->translate();

			const b2Transform &transform = pieceIt->refb2Body_->GetTransform();

			// FOR DEBUG
			
			if (isDrawPolygons_)
			{
				screen_->drawPolygon(pieceIt->id_, transform);
				screen_->drawPolygonDots(pieceIt->id_, pieceIt->globalCoordinates_);
			}


			if (isDrawSprites_)
			{
				screen_->drawSprite(pieceIt->id_, transform);
			}

			// Debug Graphics
			//screen_->drawCircle(pieceIt->refb2Body_->GetWorldCenter(), 0.05, sf::Color(0, 0, 255));
			//screen_->drawCircle(transform.p, 0.05, sf::Color(255, 0, 0));
		}

		for (auto& joint : joints_)
		{
			auto& anchorA = joint->GetAnchorA();
			auto& anchorB = joint->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, -1);
		}

		screen_->updateDisplay();

		if (isAuto)
		{
			// Hand made
			if (nIteration == pieces_.size()*200)
			{
				for (auto& piece : pieces_)
				{
					//setDamping(piece.refb2Body_, 0.025, 0.05);
					setCollideOn(piece.refb2Body_);
				}

			}
			
			

		}


		sf::Event nextEvent;
		while (screen_->pollEvent(nextEvent))
		{

			if (nextEvent.type == sf::Event::Closed)
				screen_->closeWindow();
			else {

				if (nextEvent.type == sf::Event::KeyPressed)
				{
					switch (nextEvent.key.code)
					{
					case sf::Keyboard::P:
						isDrawPolygons_ = !isDrawPolygons_;
						break;
					case sf::Keyboard::O:
						isDrawSprites_ = !isDrawSprites_;
						break;
					case sf::Keyboard::E:
						explode(5, -1);
						break;
					case sf::Keyboard::R:
						damping += 0.1;
						for (auto& piece : pieces_)
						{
							setDamping(piece.refb2Body_, damping, damping);
						}
						break;
					case sf::Keyboard::T:
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
					case sf::Keyboard::M:
						if (connectedSpringIndex_ < int(matings_.size()))
						{
							putMatingSprings(matings_[connectedSpringIndex_]);
							++connectedSpringIndex_;
						}
						break;
					case sf::Keyboard::C:
						for (auto& piece : pieces_)
						{
							switchColide(piece.refb2Body_);
						}
					default:
						break;
					}
				}
			}
		}
	}

	//screen_->screenShot("../data/ofir/RePAIR/group_39/assembly.png");
	float sumErr = sumDistancesGroundTruthVertices();
	screen_->closeWindow();
}

void World::moveAssemblyToOrigin(b2Vec2 & centerPosition)
{
	b2Vec2 centerOfMass = getCenterOfMass(pieces_);
	b2Vec2 finalTranslate = centerOfMass - centerPosition;

	for (auto& piece:pieces_ )
	{
		piece.finalCoordinates_ = piece.localCoordinates_;
		const b2Transform& transform = piece.refb2Body_->GetTransform();
		//piece.refb2Body_->SetTransform(transform.p - finalTranslate, 0);
		piece.refb2Body_->SetLinearVelocity(0.1*finalTranslate);
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
		file << piece.id_ << "," << piece.finalTranslate_.x << "," << piece.finalTranslate_.y << "," << piece.finalRot_.s << "," << piece.finalRot_.c << std::endl;
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
			file << piece.id_ <<"," << piece.finalCoordinates_(i, 0) << "," << piece.finalCoordinates_(i, 1) << std::endl;
		}
	}

	// Close the file
	file.close();

	std::cout << "Matrix data written to CSV file: " << filename << std::endl;
}

float World::sumDistancesGroundTruthVertices()
{
	float sum = 0;
	for (auto& mating:groundTruthMatings_)
	{
		for (auto& pieceA:pieces_)
		{
			for (auto& pieceB : pieces_)
			{
				if (pieceA.id_ == mating.firstPieceId_ && pieceB.id_ == mating.secondPieceId_)
				{
					b2Vec2 vertexA;
					pieceA.getVeterxGlobalCoords(vertexA, mating.firstPieceVertex_);
					b2Vec2 vertexB;
					pieceA.getVeterxGlobalCoords(vertexB, mating.secondPieceVertex_);

					float xSquared = (vertexA.x - vertexB.x) * (vertexA.x - vertexB.x);
					float ySquared = (vertexA.y - vertexB.y) * (vertexA.y - vertexB.y);
					sum+= std::sqrt(xSquared + ySquared);
				}
			}
		}
	}

	return sum;
}