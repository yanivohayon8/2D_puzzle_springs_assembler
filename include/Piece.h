#pragma once
#include <Eigen/Dense>

class Piece
{
public:
	int id_;
	Eigen::MatrixX2d coordinates_;
	Piece(int pieceId,Eigen::MatrixX2d coordinates);
	void printCoords();
	std::pair<double, double> getVertexCoord(int iVertex);
	int getNumCoords();
};

