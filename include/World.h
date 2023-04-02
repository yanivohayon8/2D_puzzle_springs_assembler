#pragma once
#include <box2d/box2d.h>
#include <Piece.h>

class World
{
public:
	World();
	b2World world_ = b2World(b2Vec2(0, 0));
	std::vector<b2Body*> bodies_;
	b2Body* createBody(Piece& piece);
	void insert_pieces(std::vector<Piece> &pieces);
	void buckle_springs();
	void manual_simulation();
};

