#pragma once
#include <box2d/box2d.h>
#include <Piece.h>
#include <EdgeMating.h>
#include "Screen.h"
#include <numeric>
#include "Utils.h"

class World
{
public:
	World();
	//b2World world_ = b2World(b2Vec2(0, 0));
	b2World world_ = b2World(b2Vec2(0, -10.0f));
	std::vector<Piece> pieces_;
	Screen* screen_;

	void Init(std::vector<Piece> &pieces);
	b2Body* createBody(Piece& piece);
	void connectSpringsToPieces(const EdgeMating& edgeMating);
	void Simulation();
	void initBounds(int height, int width, int scale);
};


//struct SimulationParams
//{
//	
//};
