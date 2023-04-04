#pragma once
#include <Eigen/Dense>
#include <box2d/box2d.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>

class Piece
{
public:
	int id_;
	Eigen::MatrixX2d coordinates_;
	b2Body* refb2Body_;
	cv::Mat cvCoords_;
	cv::Scalar color_;
	Piece(int pieceId,Eigen::MatrixX2d coordinates);
	void printCoords();
	std::pair<double, double> getVertexCoord(int iVertex);
	int getNumCoords();
	double getArea();
	void rotate(const b2Rot& rot);
	void translate(const b2Vec2& translateVector);
};

