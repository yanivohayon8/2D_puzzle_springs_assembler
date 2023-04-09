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
	//b2World world_ = b2World(b2Vec2(0, 0));
	b2World world_ = b2World(b2Vec2(0, -0.1f));
	std::vector<Piece> pieces_;
	Screen* screen_;
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;

	void Init(std::vector<Piece> &pieces);
	b2Body* createPieceBody(Piece& piece);
	void connectSpringsToPieces(const EdgeMating& edgeMating);
	void Simulation();
	void initBounds(float height, float width, float scale, float padding);
};