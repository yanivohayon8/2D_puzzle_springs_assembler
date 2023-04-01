#include "Piece.h"
#include <iostream>

Piece::Piece(int pieceId, Eigen::MatrixX2d coordinates)
{
	id_ = pieceId;
	coordinates_ = coordinates;
}

void Piece::printCoords()
{
	for (int ii = 0; ii < coordinates_.rows(); ii++)
	{
		std::cout << "(" << coordinates_.coeff(ii, 0) << "," << coordinates_(ii, 1) << ")" << std::endl;
	}
}