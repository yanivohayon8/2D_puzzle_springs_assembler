#pragma once
#include <string>
#include <box2d/box2d.h>
#include <vector>
#include <json.hpp>
#include "Global_Parameters.h"

class VertexMatingRePAIR
{
public:
	std::string firstPieceId_; // The id of the first piece in the mating
	b2Vec2 firstPieceLocalCoords_; // the id of the edge of the first piece
	std::string secondPieceId_; // The id of the second piece in the mating
	b2Vec2 secondPieceLocalCoords_; // the id of the edge of the second piece
	b2DistanceJoint* jointRef_;

	VertexMatingRePAIR(std::string firstPieceId,b2Vec2 &firstPieceLocalCoords,
		std::string secondPieceId, b2Vec2 &secondPieceLocalCoords);

	bool operator==(const VertexMatingRePAIR& other) const;
};

