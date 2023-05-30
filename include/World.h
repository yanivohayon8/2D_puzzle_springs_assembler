#pragma once
#include <box2d/box2d.h>
#include <Piece.h>
#include <EdgeMating.h>
#include <VertexMating.h>
#include "SfmlScreen.h"
#include <numeric>
#include "Utils.h"
#include <iostream>
#include <fstream>

class World
{
public:
	World(std::vector<Piece>& pieces,std::vector<VertexMating> &matings);
	//b2World world_ = b2World(b2Vec2(0, 0.05));
	b2World world_ = b2World(b2Vec2(0, 0));
	std::vector<Piece> pieces_;
	std::vector<Piece> rawPieces_;
	SfmlScreen* screen_;
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;
	std::vector< b2DistanceJoint*> joints_;
	std::vector<VertexMating> matings_; 
	int connectedSpringIndex_ = 0; 
	int screenHeight_ = 1380;
	int screenWidth_ = 1380;
	float boardHeight_ = 10;//20; 
	float boardWidth_ = 10;//20;//note also the recommondation of static bodies (no more than 50!)

	bool isDrawPolygons_ = false;
	bool isDrawSprites_ = true;
	
	b2Body* createPieceBody(Piece& piece, b2Vec2& initialPosition);
	void connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB, b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB,
		float minLength = 0, float maxLength = 2.5f, float damping = 1, float stiffness = 1);//stif=0.5
	void putMatingSprings(VertexMating& mating);
	void orderSpringsConnection();

	void InitPieces();
	void initBounds();
	void Init();
	void preProcess();
	void Simulation(bool isAuto=false);
	void explode(int MaxPower, int seed);
	void switchColide(b2Body* body);
	void setCollideOff(b2Body* body);
	void setCollideOn(b2Body* body);
	void setDamping(b2Body* body, double linearDamping, double angularDamping);

	b2Vec2 getCenterOfMass(std::vector<Piece>& pieces);

	void saveFinalCoordinates(const std::string& filename);
	void saveFinalTransforms(const std::string& filename);
	void moveAssemblyToOrigin();
};