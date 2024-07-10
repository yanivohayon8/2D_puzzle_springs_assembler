#include "VertexMating.h"

//VertexMating::VertexMating()
//{
//
//}
//
//VertexMating::VertexMating(std::string firstPieceId, int firstPieceVertex, std::string secondPieceId, int secondPieceVertex)
//{
//	firstPieceId_ = firstPieceId;
//	secondPieceId_ = secondPieceId;
//	firstPieceVertex_ = firstPieceVertex;
//	secondPieceVertex_ = secondPieceVertex;
//	jointLengthSnapshots_.clear();
//}
//
//bool VertexMating::operator==(const VertexMating& other) const {
//	return (firstPieceId_ == other.firstPieceId_ &&
//		firstPieceVertex_ == other.firstPieceVertex_ &&
//		secondPieceId_ == other.secondPieceId_ &&
//		secondPieceVertex_ == other.secondPieceVertex_);
//}
//
////VertexMating::~VertexMating()
////{
////}
//
//void VertexMating::snapshotJointLength()
//{
//	jointLengthSnapshots_.push_back(jointRef_->GetCurrentLength());
//}
//
//nlohmann::json VertexMating::toJson(float scale)
//{
//	nlohmann::json oJson;
//
//	oJson["firstPieceId"] = firstPieceId_;
//	oJson["firstPieceVertex"] = firstPieceVertex_;
//	oJson["secondPieceId"] = secondPieceId_;
//	oJson["secondPieceVertex"] = secondPieceVertex_;
//	auto numSnapshots = jointLengthSnapshots_.size();
//	oJson["snapshotedLength"] = jointLengthSnapshots_[numSnapshots-1] / scale;
//
//	return oJson;
//}


VertexMatingRePAIR::VertexMatingRePAIR(std::string firstPieceId, b2Vec2 &firstPieceLocalCoords,
	std::string secondPieceId, b2Vec2 &secondPieceLocalCoords)
{
	firstPieceId_ = firstPieceId;
	firstPieceLocalCoords_ = firstPieceLocalCoords; 
	secondPieceId_ = secondPieceId; 
	secondPieceLocalCoords_ = secondPieceLocalCoords; 
}

bool VertexMatingRePAIR::operator==(const VertexMatingRePAIR& other) const {
	return (firstPieceId_ == other.firstPieceId_ &&
		firstPieceLocalCoords_ == other.firstPieceLocalCoords_ &&
		secondPieceId_ == other.secondPieceId_ &&
		secondPieceLocalCoords_ == other.secondPieceLocalCoords_);
}

//nlohmann::json VertexMatingRePAIR::toJson(float scale)
//{
//	nlohmann::json oJson;
//
//	oJson["firstPieceId"] = firstPieceId_;
//	double firstXscaled = firstPieceLocalCoords_.x / SCALE_IMAGE_COORDINATES_TO_BOX2D;
//	double firstYscaled = firstPieceLocalCoords_.y / SCALE_IMAGE_COORDINATES_TO_BOX2D;
//	oJson["firstPieceLocalCoords"] = nlohmann::json::array({firstXscaled,firstYscaled });
//	oJson["secondPieceId"] = secondPieceId_;
//	double secondXscaled = secondPieceLocalCoords_.x / SCALE_IMAGE_COORDINATES_TO_BOX2D;
//	double secondYscaled = secondPieceLocalCoords_.y / SCALE_IMAGE_COORDINATES_TO_BOX2D;
//	oJson["secondPieceLocalCoords"] = nlohmann::json::array({secondXscaled,secondYscaled });
//	auto numSnapshots = jointLengthSnapshots_.size();
//	oJson["snapshotedLength"] = jointLengthSnapshots_[numSnapshots - 1] / scale;
//
//	return oJson;
//}

//VertexMatingRePAIR::~VertexMatingRePAIR()
//{
//}

