#include "Screen.h"

Screen::Screen(int height, int width, int scale)
{
	height_ = height;
	width_ = width;
	scale_ = scale;
	frame_ = cv::Mat(height_, width_, CV_8UC3);
	windowName_ = "Vika"; // What does it matter?
}
void Screen::initDisplay()
{
	cv::namedWindow(windowName_);
}

int Screen::updateDisplay()
{
	cv::imshow(windowName_, frame_);
	return cv::waitKey(5);
}

void Screen::finishDisplay()
{
	cv::destroyWindow(windowName_);
}

void Screen::drawPolygon(Eigen::MatrixX2d& coordinates, cv::Scalar& color)
{
	std::vector<cv::Point2i> polygon;

	for (int iCoord = 0; iCoord < coordinates.rows(); iCoord++)
	{
		int x = static_cast<int>(width_ / 2 + scale_ * coordinates.row(iCoord)(0));
		int y = static_cast<int>(height_ / 2 - scale_ * coordinates.row(iCoord)(1));
		cv::Point2i point(x, y);
		polygon.push_back(point);
	}

	cv::fillPoly(frame_, polygon, color);
}