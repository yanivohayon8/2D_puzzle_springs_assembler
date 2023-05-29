#pragma once
#include <random>
#include <iostream>
//#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>
#include <box2d/box2d.h>
#include <Eigen/Dense>

void generate2DVectors(std::vector<b2Vec2>& vectors, int n, int maxX, int maxY, int padding, int seed);

//void generateColors(std::vector<cv::Scalar>& oColors);

int sampleIntUniformly(double max, double min, int seed);

void getScreenSize(int& oHeight, int& oWidth);

void getRoatationMatrix(Eigen::MatrixX2d& oRotation, double angle);

void getRoatationMatrix(Eigen::MatrixX2d& oRotation, const b2Rot& rot);