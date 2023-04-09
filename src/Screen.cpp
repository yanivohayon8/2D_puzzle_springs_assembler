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
}

void Screen::finishDisplay()
{
	cv::destroyWindow(windowName_);
}

int Screen::castToImageX(double x)
{
	return static_cast<int>(scale_ * x);
}

int Screen::castToImageY(double y)
{
	int padding = 10;// Well this is empiracly helps
	return static_cast<int>(height_ - padding - scale_ * y);
}

void Screen::drawCircle(const b2Vec2& point,int radius,cv::Scalar& color)
{
	auto castPoint = cv::Point(castToImageX(point.x), castToImageY(point.y));
	cv::circle(frame_, castPoint, radius, color);
}

void Screen::drawPolygon(std::vector<b2Vec2>& coordinates, cv::Scalar& color)
{
	std::vector<cv::Point2i> polygon;

	for (int iCoord = 0; iCoord < coordinates.size(); iCoord++)
	{
		int x = static_cast<int>(castToImageX(coordinates.at(iCoord)(0)));	
		int y = static_cast<int>(castToImageY(coordinates.at(iCoord)(1)));
		cv::Point2i point(x, y);
		polygon.push_back(point);
	}

	cv::fillPoly(frame_, polygon, color);
}

void Screen::drawBounds(std::vector<std::vector<b2Vec2>>* boundsBodyCoordinates = nullptr)
{
	
	std::vector<cv::Scalar> colors = {
		{255,255,255},
		{0,0,255},
		{255,0,255},
		{0,255,0},
		{255,0,0},
	};
	
	auto& colorIt = colors.begin();

	for (auto coordIt = boundsBodyCoordinates->begin(); coordIt != boundsBodyCoordinates->end(); ++coordIt)
	{
		//this->drawPolygon(*coordIt, *colorIt); // for debug
		this->drawPolygon(*coordIt, colors[0]);
		colorIt++;
	}
}