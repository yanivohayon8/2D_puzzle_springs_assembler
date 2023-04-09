#include "Screen.h"

Screen::Screen(int height, int width, double scale)
{
	height_ = height;
	width_ = width;
	scale_ = scale;
	frame_ = cv::Mat(height_, width_, CV_8UC3);
	windowName_ = "Vika"; // What does it matter?
	BOUNDS_COLOR_ = cv::Scalar(0, 0, 255);
	BOUNDS_THICKNESS_ = 5;
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



void Screen::drawPolygon(std::vector<b2Vec2>& coordinates, cv::Scalar& color)
{
	std::vector<cv::Point2i> polygon;

	for (int iCoord = 0; iCoord < coordinates.size(); iCoord++)
	{
		/*int x = static_cast<int>(width_ / 2 + scale_ * coordinates.at(iCoord)(0));
		int y = static_cast<int>(height_ / 2 - scale_ * coordinates.at(iCoord)(1));*/
		int x = static_cast<int>(scale_* coordinates.at(iCoord)(0));
		int y = static_cast<int>(height_ - scale_*coordinates.at(iCoord)(1));
		// 
		//int y = static_cast<int>(scale_ * coordinates.at(iCoord)(1));
		cv::Point2i point(x, y);
		polygon.push_back(point);
	}

	cv::fillPoly(frame_, polygon, color);
}

void Screen::drawBounds(std::vector<std::vector<b2Vec2>>* boundsBodyCoordinates = nullptr)
{
	
	// when colliding is complete - return this
	/*cv::rectangle(frame_,cv::Point2i(0, 0), cv::Point2i(width_, height_), BOUNDS_COLOR_,BOUNDS_THICKNESS_);

	for (auto coordIt = boundsBodyCoordinates->begin();coordIt!=boundsBodyCoordinates->end();++coordIt)
	{
		this->drawPolygon(*coordIt, cv::Scalar(255, 255, 255));
	}*/

	std::vector<cv::Scalar> colors = {
		{255,255,255},
		{255,0,255},
		{0,0,255},
		{0,255,0},
		{255,0,0},
	};
	
	auto& colorIt = colors.begin();

	for (auto coordIt = boundsBodyCoordinates->begin(); coordIt != boundsBodyCoordinates->end(); ++coordIt)
	{
		this->drawPolygon(*coordIt, *colorIt);
		colorIt++;
	}
}