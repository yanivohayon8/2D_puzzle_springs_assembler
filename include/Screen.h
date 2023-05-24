//#pragma once
//#include <Eigen/Dense>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <string>
//#include <box2d/box2d.h>
//
//
//class ScreenOpencv
//{
//public:
//	ScreenOpencv(int height, int width, double scale);
//	cv::Mat frame_;
//	int height_;
//	int width_;
//	double scale_; 
//	std::string windowName_;
//	cv::Scalar BOUNDS_COLOR_;
//	float BOUNDS_THICKNESS_;
//
//	void initDisplay();
//	int updateDisplay();
//	void clearDisplay();
//	void finishDisplay();
//	void drawPolygon(std::vector<b2Vec2>& coordinates, cv::Scalar& color);
//	void drawBounds(std::vector<std::vector<b2Vec2>>* boundsBodyCoordinates);
//	void drawCircle(const b2Vec2& point, int radius, cv::Scalar& color);
//	int castToImageX(double x);
//	int castToImageY(double y);
//	void drawBounds();
//	void drawLine(b2Vec2& point1, b2Vec2& point2, cv::Scalar& color, int thickness);
//
//	void writeText(const std::string& str, const b2Vec2& position);
//
//	void pasteImage(int xPos, int yPos, int recCols, int recRows);
//};
