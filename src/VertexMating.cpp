#include "VertexMating.h"

VertexMating::VertexMating(std::string firstPieceId, int firstPieceVertex, std::string secondPieceId, int secondPieceVertex)
{
	firstPieceId_ = firstPieceId;
	secondPieceId_ = secondPieceId;
	firstPieceVertex_ = firstPieceVertex;
	secondPieceVertex_ = secondPieceVertex;
	jointLengthSnapshots_.clear();
}

bool VertexMating::operator==(const VertexMating& other) const {
	return (firstPieceId_ == other.firstPieceId_ &&
		firstPieceVertex_ == other.firstPieceVertex_ &&
		secondPieceId_ == other.secondPieceId_ &&
		secondPieceVertex_ == other.secondPieceVertex_);
}

VertexMating::~VertexMating(){}

void VertexMating::snapshotJointLength()
{
	jointLengthSnapshots_.push_back(jointRef_->GetCurrentLength());
}

nlohmann::json VertexMating::toJson(float scale)
{
	nlohmann::json oJson;

	oJson["firstPieceId"] = firstPieceId_;
	oJson["firstPieceVertex"] = firstPieceVertex_;
	oJson["secondPieceId"] = secondPieceId_;
	oJson["secondPieceVertex"] = secondPieceVertex_;
	oJson["snapshotedLength"] = jointLengthSnapshots_[jointLengthSnapshots_.size()-1] / scale;

	return oJson;
}
