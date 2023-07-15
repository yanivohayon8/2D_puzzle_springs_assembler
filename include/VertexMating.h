#pragma once
#include <string>
#include <box2d/box2d.h>
#include <vector>
#include <json.hpp>

class VertexMating
{
public:
	std::string firstPieceId_; // The id of the first piece in the mating
	int firstPieceVertex_; // the id of the edge of the first piece
	std::string secondPieceId_; // The id of the second piece in the mating
	int secondPieceVertex_; // the id of the edge of the second piece
	b2DistanceJoint* jointRef_;
	std::vector<float> jointLengthSnapshots_;

	VertexMating(std::string firstPieceId, int firstPieceEdge, std::string secondPieceId, int secondPieceEdge);

	bool operator==(const VertexMating& other) const;

	void snapshotJointLength();
	nlohmann::json toJson(float scale=1);


	~VertexMating();
};

