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

void sortVerticesCCW(std::vector<std::pair<double, double>>& coords, std::map<int, int>& indexMap) {
	// Calculate the center of the polygon
	double centerX = 0.0;
	double centerY = 0.0;
	for (const auto& coord : coords) {
		centerX += coord.first;
		centerY += coord.second;
	}
	centerX /= coords.size();
	centerY /= coords.size();

	// Calculate angles of the vertices with respect to the center
	std::vector<std::pair<double, int>> angles;
	for (int i = 0; i < coords.size(); i++) {
		double dx = coords[i].first - centerX;
		double dy = coords[i].second - centerY;
		double angle = std::atan2(dy, dx);
		angles.push_back(std::make_pair(angle, i));
	}

	// Sort the vertices by angle
	std::sort(angles.begin(), angles.end());

	// Build the new "coords" vector and the index mapping
	std::vector<std::pair<double, double>> newCoords;
	for (int i = 0; i < angles.size(); i++) {
		newCoords.push_back(coords[angles[i].second]);
		indexMap[angles[i].second] = i;
	}
	coords = newCoords;
}