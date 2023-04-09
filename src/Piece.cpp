#include "Piece.h"
#include <iostream>

Piece::Piece(int pieceId, Eigen::MatrixX2d coordinates)
{
	id_ = pieceId;
	localCoordinates_ = coordinates;
	
	cv::eigen2cv(coordinates, cvCoords_);
	cvCoords_.convertTo(cvCoords_, CV_32F);
}

void Piece::printCoords()
{
	for (int ii = 0; ii < localCoordinates_.rows(); ii++)
	{
		std::cout << "(" << localCoordinates_.coeff(ii, 0) << "," << localCoordinates_(ii, 1) << ")" << std::endl;
	}
}

std::pair<double, double> Piece::getVertexCoord(int iVertex)
{
	std::pair<double, double> xy;
	xy.first = localCoordinates_.coeff(iVertex, 0);
	xy.second = localCoordinates_.coeff(iVertex, 1);
	return xy;
}

int Piece::getNumCoords()
{
	return localCoordinates_.rows();
}

double Piece::getArea()
{
	return cv::contourArea(cvCoords_);
}

void Piece::rotate(const b2Rot& rot)
{
	// This is inspired from pelegs code: check it! (remember in Passover about the traslate method)
	Eigen::Matrix2d rotationEigen;
	rotationEigen << rot.c, -rot.s, rot.s, rot.c;
	localCoordinates_ = localCoordinates_ * rotationEigen.transpose();
}

void Piece::translate()
{
	for (int i = 0; i < globalCoordinates_.size(); i++)
	{
		globalCoordinates_[i] = refb2Body_->GetWorldPoint(localCoordsAsVecs_.at(i));
	}
}