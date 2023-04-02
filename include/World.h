#pragma once
#include <box2d/box2d.h>
#include <Piece.h>
#include <EdgeMating.h>

class World
{
public:
	World();
	b2World world_ = b2World(b2Vec2(0, 0));
	std::vector<b2Body*> bodies_;
	b2Body* createBody(Piece& piece);
	void Init(std::vector<Piece> &pieces);
	void connectSpringsToPieces(const EdgeMating& edgeMating);
	void manual_simulation();
};

