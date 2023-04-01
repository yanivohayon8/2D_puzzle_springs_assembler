#pragma once
#include <Eigen/Dense>

class Piece
{
public:
	int id_;
	Eigen::MatrixX2d coordinates_;
	Piece(int pieceId,Eigen::MatrixX2d coordinates);
};

