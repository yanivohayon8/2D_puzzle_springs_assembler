#include "Piece.h"

Piece::Piece(int pieceId, Eigen::MatrixX2d coordinates)
{
	id_ = pieceId;
	coordinates_ = coordinates;
}