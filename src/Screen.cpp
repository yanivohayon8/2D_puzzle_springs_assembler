#include "Screen.h"

Screen::Screen(int height, int width, int scale)
{
	height_ = height;
	width_ = width;
	scale_ = scale;
	frame_ = cv::Mat(height_, width_, CV_8UC3);
	windowName_ = "Vika"; // What does it matter?
	BOUNDS_COLOR_ = cv::Scalar(0, 0, 255);
}
void Screen::initDisplay()
{
	cv::namedWindow(windowName_, cv::WINDOW_NORMAL);
	cv::setWindowProperty(windowName_, cv::WND_PROP_FULLSCREEN, cv::WND_PROP_FULLSCREEN);
}

int Screen::updateDisplay()
{
	cv::imshow(windowName_, frame_);
	return cv::waitKey(5);
}

void Screen::clearDisplay()
{
	frame_.setTo(cv::Scalar(0, 0, 0));
	//frame_ = cv::Mat(height_, width_, CV_8UC3);
	//cv::circle(frame_, cv::Point(0, 0), 500, cv::Scalar(0, 0, 255));// for debug
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

void Screen::drawBounds()
{
	/*std::vector<cv::Point2i> polygon;

	for (auto& coordinates : boundsCoordinates)
	{
		for (int iCoord = 0; iCoord < coordinates.rows(); iCoord++)
		{
			int x = static_cast<int>(coordinates.row(iCoord)(0));
			int y = static_cast<int>(coordinates.row(iCoord)(1));
			cv::Point2i point(x, y);
			polygon.push_back(point);
		}

		cv::fillPoly(frame_, polygon, BOUNDS_COLOR_);
	}*/

	cv::rectangle(frame_,cv::Point2i(0, 0), cv::Point2i(width_, height_), BOUNDS_COLOR_,10);
}