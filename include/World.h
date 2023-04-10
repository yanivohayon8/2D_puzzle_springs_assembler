#pragma once
#include <box2d/box2d.h>
#include <Piece.h>
#include <EdgeMating.h>
#include "Screen.h"
#include <numeric>
#include "Utils.h"
#include "DebugVisualize.h"

class World
{
public:
	World(std::vector<EdgeMating> &matings);
	b2World world_ = b2World(b2Vec2(0, 0));
	//b2World world_ = b2World(b2Vec2(0, -2.0f)); // for first debugging
	//b2World world_ = b2World(b2Vec2(0, -12.0f)); // for first debugging
	std::vector<Piece> pieces_;
	Screen* screen_;
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;
	std::vector< b2DistanceJoint*> joints_;
	std::vector<EdgeMating> matings_; // might be unnessary
	std::vector<SpringMating> springs_;
	int connectedSpringIndex_ = -1; // from the start


	void InitPieces(std::vector<Piece> &pieces);
	b2Body* createPieceBody(Piece& piece, b2Vec2& initialPosition);
	void connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB, b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB);
	void putMatingSprings(EdgeMating& mating);
	void Simulation();
	void initBounds(float height, float width, float wallWidth);
	void explode(int MaxPower, int seed);
	void switchColide(b2Body* body);
	void setDamping(b2Body* body, double linearDamping, double angularDamping);
};