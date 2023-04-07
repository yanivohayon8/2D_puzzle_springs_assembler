#pragma once
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>


class Screen
{
public:
	Screen(int height, int width, int scale);
	cv::Mat frame_;
	int height_;
	int width_;
	int scale_; 
	std::string windowName_;
	cv::Scalar BOUNDS_COLOR_;
	float BOUNDS_THICKNESS_;

	void initDisplay();
	int updateDisplay();
	void clearDisplay();
	void finishDisplay();
	void drawPolygon(Eigen::MatrixX2d& coordinates, cv::Scalar& color);
	//void drawBounds(std::vector<Eigen::MatrixX2d> boundsCoordinates);
	void drawBounds();
};
