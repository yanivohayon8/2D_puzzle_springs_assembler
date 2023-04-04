#pragma once
#include <random>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>

void generateColors(std::vector<cv::Scalar>& oColors);

void getScreenSize(int& oHeight, int& oWidth);
