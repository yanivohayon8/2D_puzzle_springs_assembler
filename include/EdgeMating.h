#pragma once
#include <box2d/box2d.h>

class EdgeMating
{
public:
	int firstPieceId_; // The id of the first piece in the mating
	int firstPieceEdge_; // the id of the edge of the first piece
	int secondPieceId_; // The id of the second piece in the mating
	int secondPieceEdge_; // the id of the edge of the second piece

	EdgeMating(int firstPieceId, int firstPieceEdge, int secondPieceId, int secondPieceEdge);

	bool operator==(const EdgeMating& other) const;
};


class SpringEdgeMating
{
public:
	b2Body* bodyA_;
	b2Body* bodyB_;
	b2Vec2 firstAnchorsA_;
	b2Vec2 secondAnchorsA_;
	b2Vec2 firstAnchorsB_;
	b2Vec2 secondAnchorsB_;

	SpringEdgeMating(b2Body* bodyA, b2Body* bodyB, b2Vec2& firstAnchorsA, b2Vec2& secondAnchorsA, b2Vec2& firstAnchorsB, b2Vec2& secondAnchorsB);
};
