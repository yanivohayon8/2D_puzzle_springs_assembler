#pragma once
#include <Eigen/Dense>
#include <box2d/box2d.h>
#include <vector>
#include <algorithm>

class Piece
{
public:
	std::string id_;
	b2Body* refb2Body_;
	Eigen::MatrixX2d localCoordinates_;
	Eigen::MatrixX2d finalCoordinates_;
	std::vector<b2Vec2> localCoordsAsVecs_;
	std::vector<b2Vec2> globalCoordinates_;
	b2Vec2 finalTranslate_;
	b2Rot finalRot_;
	bool isRotationFixed = false;
	std::string imagePath_;
	b2AABB aabb_;

	Piece(std::string pieceId,Eigen::MatrixX2d coordinates, std::string imagePath);
	void printCoords();
	std::pair<double, double> getVertexCoord(int iVertex);
	int getNumCoords();
	void rotate(const b2Rot& rot);
	void translate();
	std::pair<int, int> getEdgeVertexIndexes(int iEdge);
	b2Vec2* getVeterxLocalCoords(int iVertex);
	void getVeterxGlobalCoords(b2Vec2& oCoords, int iVertex);
	void sortVerticesCCW(Eigen::MatrixX2d& coords, std::vector<int>& index_map);
	void getGlobalCoordsMoved(Eigen::MatrixX2d &oCoords, b2Vec2 translate);
	void getVertexGlobalCoordsAsEigen(Eigen::MatrixX2d& oCoords);
	void computeBoundingBox();
	float getBodyBoundingBoxWidth();
	float getBodyBoundingBoxHeight();
};

