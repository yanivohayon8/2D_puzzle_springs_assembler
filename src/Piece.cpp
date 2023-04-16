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

std::pair<int, int> Piece::getEdgeVertexIndexes(int iEdge)
{
	int indexFirstVertex = iEdge;
	int indexSecondVertex = (iEdge + 1) % getNumCoords();
	return std::pair<int, int>(indexFirstVertex, indexSecondVertex);
}

b2Vec2* Piece::getVeterxLocalCoords(int iVertex)
{
	return &localCoordsAsVecs_[iVertex];
}

void Piece::getVeterxGlobalCoords(b2Vec2 &oCoords,int iVertex)
{
	oCoords = globalCoordinates_[iVertex];
}

void Piece::sortVerticesCCW(Eigen::MatrixX2d& coords, std::vector<int>& index_map)
{
	// Compute centroid
	Eigen::Vector2d centroid(0, 0);
	for (int i = 0; i < coords.rows(); i++) {
		centroid += coords.row(i);
	}
	centroid /= coords.rows();

	// Compute angles with respect to centroid
	std::vector<std::pair<double, int>> angles;
	for (int i = 0; i < coords.rows(); i++) {
		double x = coords(i, 0) - centroid(0);
		double y = coords(i, 1) - centroid(1);
		double angle = atan2(y, x);
		angles.push_back(std::make_pair(angle, i));
	}

	// Sort angles
	std::sort(angles.begin(), angles.end());

	// Reorder coordinates and index map
	Eigen::MatrixX2d sorted_coords(coords.rows(), coords.cols());
	std::vector<int> sorted_index_map(coords.rows());
	for (int i = 0; i < coords.rows(); i++) {
		sorted_coords.row(i) = coords.row(angles[i].second);
		sorted_index_map[angles[i].second] = i;
	}

	coords = sorted_coords;
	index_map = sorted_index_map;
}