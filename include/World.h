#pragma once
#include <box2d/box2d.h>
#include <Piece.h>
#include <EdgeMating.h>
#include <VertexMating.h>
#include "Screen.h"
#include "SfmlScreen.h"
#include <numeric>
#include "Utils.h"
#include "DebugVisualize.h"
#include <iostream>
#include <fstream>

class World
{
public:
	World(std::vector<Piece>& pieces,std::vector<VertexMating> &matings);
	b2World world_ = b2World(b2Vec2(0, 0));
	//b2World world_ = b2World(b2Vec2(0, -2.0f)); // for first debugging
	//b2World world_ = b2World(b2Vec2(0, -12.0f)); // for first debugging
	std::vector<Piece> pieces_;
	std::vector<Piece> rawPieces_;
	//ScreenOpencv* screen_;
	SfmlScreen* screen_;
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;
	std::vector< b2DistanceJoint*> joints_;
	std::vector<VertexMating> matings_; // might be unnessary
	std::vector<SpringEdgeMating> springs_;
	int connectedSpringIndex_ = 0; // from the start

	//double screenScale_ = 50; //10; //50;
	int screenHeight_ = 1024; //880;
	int screenWidth_ = 1024;
	//float wallWidth = 0.1;
	float boardHeight_ = 17; //This fit my screeen...
	float boardWidth_ = 28.5; //This fit my screeen... note also the recommondation of static bodies (no more than 50!)

	b2Body* createPieceBody(Piece& piece, b2Vec2& initialPosition);
	void connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB, b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB,
		float minLength = 0.05f, float maxLength = 2.0f, float damping = 0.3, float stiffness = 0.5f);
	void putMatingSprings(VertexMating& mating);
	void orderSpringsConnection();

	void InitPieces();
	void initBounds();
	void Init();
	void preProcess();
	void Simulation(bool isAuto=true);
	void explode(int MaxPower, int seed);
	void switchColide(b2Body* body);
	void setCollideOff(b2Body* body);
	void setCollideOn(b2Body* body);
	void setDamping(b2Body* body, double linearDamping, double angularDamping);

	b2Vec2 getCenterOfMass(std::vector<Piece>& pieces);

	void saveFinalCoordinates(const std::string& filename);
	void saveFinalTransforms(const std::string& filename);

};