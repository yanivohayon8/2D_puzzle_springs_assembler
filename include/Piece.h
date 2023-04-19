#pragma once
#include <Eigen/Dense>
#include <box2d/box2d.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <vector>
#include <algorithm>

class Piece
{
public:
	int id_;
	b2Body* refb2Body_;
	cv::Mat cvCoords_;
	cv::Scalar color_;
	Eigen::MatrixX2d localCoordinates_;
	Eigen::MatrixX2d finalCoordinates_;
	std::vector<b2Vec2> localCoordsAsVecs_;
	std::vector<b2Vec2> globalCoordinates_;
	b2Vec2 finalTranslate_;
	b2Rot finalRot_;

	Piece(int pieceId,Eigen::MatrixX2d coordinates);
	void printCoords();
	std::pair<double, double> getVertexCoord(int iVertex);
	int getNumCoords();
	double getArea();
	void rotate(const b2Rot& rot);
	void translate();
	std::pair<int, int> getEdgeVertexIndexes(int iEdge);
	b2Vec2* getVeterxLocalCoords(int iVertex);
	//b2Vec2* getVeterxGlobalCoords(int iVertex);
	void getVeterxGlobalCoords(b2Vec2& oCoords, int iVertex);

	void sortVerticesCCW(Eigen::MatrixX2d& coords, std::vector<int>& index_map);

	//void isOverlapAfterTranslation(Piece& otherPiece, Eigen::matrixx t, Eigen::MatrixX2d& R);
	//void coordsMoved(Eigen::MatrixX2d oCoordsCopy, b2Vec2 translate);
	void getGlobalCoordsMoved(Eigen::MatrixX2d &oCoords, b2Vec2 translate);
	void getVertexGlobalCoordsAsEigen(Eigen::MatrixX2d& oCoords);
};

