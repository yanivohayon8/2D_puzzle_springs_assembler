//#pragma once
//#include <box2d/box2d.h>
//#include <Piece.h>
////#include <EdgeMating.h>
//#include <VertexMating.h>
//#include "SfmlScreen.h"
//#include <numeric>
//#include "Utils.h"
//#include <iostream>
//#include <fstream>
//
////enum AutoAssemblyPhase { connectingSprings, delayConvergence, };
//
//class World
//{
//public:
//	World();
//	World(std::vector<Piece>& pieces,std::vector<VertexMating> &matings, std::vector<VertexMating>& groundTruthMatings);
//	//b2World world_ = b2World(b2Vec2(0, 0.05));
//	b2World world_ = b2World(b2Vec2(0, 0));
//	
//	std::vector<Piece> pieces_;
//	std::vector<Piece> rawPieces_;
//	std::vector<Piece> movingPieces_; //to reconstruction
//	std::vector<std::vector<b2Vec2>> boundsCoordinates_; 
//
//	std::vector<VertexMating> matings_; 
//	std::vector<VertexMating> groundTruthMatings_;
//	int connectedSpringIndex_ = 0; // to delete? since we connect all springs in one shot
//	float boardHeight_ = 10;//20; 
//	float boardWidth_ = 10;//20;//note also the recommondation of static bodies (no more than 50!)
//
//	b2Body* createPieceBody(Piece& piece, b2BodyDef &bodyDef, b2FixtureDef &fixture);
//	void putMatingSprings(VertexMating& mating);
//	void orderSpringsConnection();
//	void InitMovingPieces();
//	void initBounds();
//	void Init();
//	//void preProcess(); // deleted
//
//	// simulation functions
//	std::vector< b2DistanceJoint*> joints_;
//	void connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB, b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB,
//		float minLength = 0, float maxLength = 2.5f, float damping = 1, float stiffness = 1);//stif=0.5
//	void Simulation(bool isAuto=true); // need to transform as class
//	void explode(int MaxPower, int seed);
//	void switchColide(b2Body* body);
//	void setCollideOff(b2Body* body);
//	void setCollideOn(b2Body* body);
//	void setDamping(b2Body* body, double linearDamping, double angularDamping);
//	void switchJointCollide(b2DistanceJoint& joint);
//	Piece* getPieceWithHighestMatings();
//	float computeOverlappingAreaPercentageBetweenPIeces();
//	void moveAssemblyToOrigin(b2Vec2& centerPosition); // to delete? as we fixed one piece...
//	
//	// auto simulation
//
//	// manual simulation
//	int screenHeight_ = 1380;
//	int screenWidth_ = 1380;
//	bool isDrawPolygons_ = false;
//	bool isDrawSprites_ = true;
//	SfmlScreen* screen_;
//
//
//
//	// Keep in world? 
//	b2Vec2 getCenterOfMass(std::vector<Piece>& pieces);
//	void saveFinalCoordinates(const std::string& filename);
//	void saveFinalTransforms(const std::string& filename);
//	float sumDistancesGroundTruthVertices();
//};