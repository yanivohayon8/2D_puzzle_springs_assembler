#pragma once
//#include <World.h>

#include <string>
#include "cpp-httplib/httplib.h"
#include <Piece.h>
#include <box2d/box2d.h>
#include <VertexMating.h>
#include "SfmlScreen.h"
#include <numeric>
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <json.hpp>

class Reconstructor
{
private:
	b2Body* createPieceBody(Piece& piece, b2BodyDef& bodyDef, b2FixtureDef& fixture);
	void initStaticBody(Piece& piece, b2Vec2& position);
	void initMovingBody(Piece& piece, b2Vec2& initialPosition);

protected:
	void putMatingSprings(VertexMating* &mating);
	bool isEnableJointsCollide_ = false;
	float jointRestLength_ = 0.01;
	float jointMinLength_ = 0;
	float jointFrequencyHertz_ = 1.0f;
	float jointDampingRatio_ = 0.01;


	int nextScreenPolygonColorIndex_ = 0;
	bool isScreenInitiated_ = false;
	bool isDebugScreenVisible_ = false;
	sf::Color jointColor_ = sf::Color::Red;

	bool isDrawOnlyPolygons = false;

public:
	float initPowerMagnitude_ = 0.2;
	Piece* getMaxMatingsPiece();
	float pieceAngularDamping_ = 0.01;
	b2World world_ = b2World(b2Vec2(0, 0));
	std::vector<Piece> activePieces_;
	std::vector<VertexMating*> activeMatings_;
	Piece* fixedPiece_;
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;

	std::vector<b2Body*> boundsWallBodies_;

	float boardHeight_;//20; 
	float boardWidth_;//20;//note also the recommondation of static bodies (no more than 50!)

	SfmlScreen* screen_; // valid also for silent mode for the last image
	int screenHeight_;
	int screenWidth_;
//#pragma message("Please convert to double");
	//float timeStep_ = 1 / 60; //1.0F / 60.0F;
	double timeStep_ = 1.0 / 60.0F; //1.0F / 60.0F;
	int velocityIterations_ = 6;
	int positionIterations_ = 2;

	Reconstructor(float boardWidth, float boardHeight, int screenWidth_, int screenHeight_);
	void initBoundaryWallBodies();
	void initRun(std::vector<Piece>& activePieces, std::vector<VertexMating*>& activeMatings, 
		int positionSeed = 1, int positionPadding = 5);
	virtual void Run(std::string screenshotPathBeforeCollide = "", std::string screenshotPathAfterCollide = "")=0;
	void closeRun(); // delete the matings and pieces...

	void disableJointsCollide();
	void enableJointsCollide();
	void setJointRestLength(float length);
	void setJointMinLength(float minlength);
	void setJointFrequency(float herz);
	void setJointDamping(float ratio);
	void setInitPowerMagnitude(float magnitude);

	void setPiecesLinearDamping(float damping);
	void setPiecesAngularDamping(float damping);
	void setPiecesCollisionOn();
	void setPiecesCollisionOff();

	void applyImpulseOnBodies(float powerMagnitude);


	// from here functions of refactor
	void setBoardWidth(float boardWidth);
	void setBoardHeight(float boardHeight);
	void setScreenWidth(float screenWidth);
	void setScreenHeight(float screenHeight);
	void updateScreen();

	void initPiecesBodies(std::vector<Piece>& activePieces, std::string fixedPieceId, std::vector<b2Vec2>& positions);
	void initMatingsJoints(std::vector<VertexMating*>& activeMatings);
	virtual nlohmann::json reconstruct(float coordinatesScale)=0;
	
	void initScreenNew(bool isScreenVisible);

	virtual void initRunNew(httplib::Request currentRequest, std::vector<Piece> activePieces, 
		std::vector<VertexMating*> activeMatings);

	void progress(int numIteration);
};

