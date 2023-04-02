#include "Piece.h"
#include <iostream>

Piece::Piece(int pieceId, Eigen::MatrixX2d coordinates)
{
	id_ = pieceId;
	coordinates_ = coordinates;
	
	cv::eigen2cv(coordinates, cvCoords_);
	cvCoords_.convertTo(cvCoords_, CV_32F);
}

void Piece::printCoords()
{
	for (int ii = 0; ii < coordinates_.rows(); ii++)
	{
		std::cout << "(" << coordinates_.coeff(ii, 0) << "," << coordinates_(ii, 1) << ")" << std::endl;
	}
}

std::pair<double, double> Piece::getVertexCoord(int iVertex)
{
	std::pair<double, double> xy;
	xy.first = coordinates_.coeff(iVertex, 0);
	xy.second = coordinates_.coeff(iVertex, 1);
	return xy;
}

int Piece::getNumCoords()
{
	return coordinates_.rows();
}

double Piece::getArea()
{
	return cv::contourArea(cvCoords_);
}