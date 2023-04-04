#pragma once
#include <box2d/box2d.h>
#include <Piece.h>
#include <EdgeMating.h>

class World
{
public:
	World();
	b2World world_ = b2World(b2Vec2(0, 0));
	std::vector<Piece> pieces_;
	b2Body* screenBody_;
	b2Body* createBody(Piece& piece);
	void Init(std::vector<Piece> &pieces);
	void connectSpringsToPieces(const EdgeMating& edgeMating);
	void Simulation();
	void initFrame();
};


class ScreenSpecs
{
public:
	// All these data members should not be double? 
	int height_;
	int width_;
	int scale_;
	ScreenSpecs(int height, int width, int scale);
};

