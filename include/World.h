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
	World();
	b2World world_ = b2World(b2Vec2(0, 0));
	//b2World world_ = b2World(b2Vec2(0, -2.0f)); // for first debugging
	std::vector<Piece> pieces_;
	Screen* screen_;
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;
	std::vector< b2DistanceJoint*> joints_;

	void InitPieces(std::vector<Piece> &pieces);
	b2Body* createPieceBody(Piece& piece, b2Vec2& initialPosition);
	void InitMatings(std::vector<EdgeMating>& matings);
	void connectSpringsToPieces(b2Body* bodyA, b2Body* bodyB, b2Vec2* globalCoordsAnchorA, b2Vec2* globalCoordsAnchorB);
	void Simulation();
	void initBounds(float height, float width, float wallWidth);
	void explode(int MaxPower, int seed);
	void switchColide(b2Body* body);
	void setDamping(b2Body* body, double linearDamping, double angularDamping);
};