#pragma once
//#include <World.h>
#include <Piece.h>
#include <box2d/box2d.h>
#include <VertexMating.h>
#include "SfmlScreen.h"
#include <numeric>
#include "Utils.h"
#include <iostream>
#include <fstream>

class Reconstructor
{
private:
	b2Body* createPieceBody(Piece& piece, b2BodyDef& bodyDef, b2FixtureDef& fixture);
	Piece* getMaxMatingsPiece();
	void initStaticBody(Piece& piece, b2Vec2& position);
	void initMovingBody(Piece& piece, b2Vec2& initialPosition);
	void connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB,
		b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB,
		float frequencyHertz = 1.0f, float dampingRatio = 0.01);
	void putMatingSprings(VertexMating& mating);

public:
	b2World world_ = b2World(b2Vec2(0, 0));
	std::vector<Piece> activePieces_;
	std::vector<VertexMating> activeMatings_;
	std::vector< b2DistanceJoint*> joints_;
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;
	float boardHeight_;//20; 
	float boardWidth_;//20;//note also the recommondation of static bodies (no more than 50!)

	SfmlScreen* screen_; // valid also for silent mode for the last image
	int screenHeight_;
	int screenWidth_;

	double timeStep_ = 1 / 60.0F; //1.0F / 60.0F;
	int velocityIterations_ = 6;
	int positionIterations_ = 2;

	Reconstructor(float boardWidth = 10,float boardHeight=10, int screenWidth_ = 1380, int screenHeight_ = 1380);
	void init();
	void initRun(std::vector<Piece>& activePieces, std::vector<VertexMating>& activeMatings, int positionSeed = 0, int positionPadding = 2);
	virtual void Run(std::string resultScreenshotPath = "")=0;
	void closeRun(); // delete the matings and pieces...
};

