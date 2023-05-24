//#include "Screen.h"
//
//ScreenOpencv::ScreenOpencv(int height, int width, double scale)
//{
//	height_ = height;
//	width_ = width;
//	scale_ = scale;
//	frame_ = cv::Mat(height_, width_, CV_8UC3);
//	windowName_ = "Vika"; // What does it matter?
//	BOUNDS_COLOR_ = cv::Scalar(0, 0, 255);
//	BOUNDS_THICKNESS_ = 5;
//}
//void ScreenOpencv::initDisplay()
//{
//	cv::namedWindow(windowName_, cv::WINDOW_NORMAL);
//	cv::setWindowProperty(windowName_, cv::WND_PROP_FULLSCREEN, cv::WND_PROP_FULLSCREEN);
//}
//
//int ScreenOpencv::updateDisplay()
//{
//	cv::imshow(windowName_, frame_);
//	return cv::waitKey(5);
//}
//
//void ScreenOpencv::clearDisplay()
//{
//	frame_.setTo(cv::Scalar(0, 0, 0));
//}
//
//void ScreenOpencv::finishDisplay()
//{
//	cv::destroyWindow(windowName_);
//}
//
//int ScreenOpencv::castToImageX(double x)
//{
//	//return static_cast<int>(scale_ * x);
//	int padding = 10;// Well this is empiracly helps
//	return static_cast<int>(width_ -padding -scale_ * x);
//}
//
//int ScreenOpencv::castToImageY(double y)
//{
//	int padding = 10;// Well this is empiracly helps
//	return static_cast<int>(height_ - padding - scale_ * y);
//}
//
//void ScreenOpencv::drawCircle(const b2Vec2& point,int radius,cv::Scalar& color)
//{
//	auto castPoint = cv::Point(castToImageX(point.x), castToImageY(point.y));
//	cv::circle(frame_, castPoint, radius, color);
//}
//
//void ScreenOpencv::drawPolygon(std::vector<b2Vec2>& coordinates, cv::Scalar& color)
//{
//	std::vector<cv::Point2i> polygon;
//
//	for (int iCoord = 0; iCoord < coordinates.size(); iCoord++)
//	{
//		int x = static_cast<int>(castToImageX(coordinates.at(iCoord)(0)));	
//		int y = static_cast<int>(castToImageY(coordinates.at(iCoord)(1)));
//		cv::Point2i point(x, y);
//		polygon.push_back(point);
//	}
//
//	cv::fillPoly(frame_, polygon, color);
//}
//
//
//void ScreenOpencv::drawBounds(std::vector<std::vector<b2Vec2>>* boundsBodyCoordinates = nullptr)
//{
//	
//	std::vector<cv::Scalar> colors = {
//		{255,255,255},
//		{0,0,255},
//		{255,0,255},
//		{0,255,0},
//		{255,0,0},
//	};
//	
//	auto& colorIt = colors.begin();
//
//	for (auto coordIt = boundsBodyCoordinates->begin(); coordIt != boundsBodyCoordinates->end(); ++coordIt)
//	{
//		//this->drawPolygon(*coordIt, *colorIt); // for debug
//		this->drawPolygon(*coordIt, colors[0]);
//		colorIt++;
//	}
//}
//
//void ScreenOpencv::drawLine(b2Vec2& point1, b2Vec2& point2, cv::Scalar& color, int thickness)
//{
//	int x1 = static_cast<int>(castToImageX(point1.x));
//	int y1 = static_cast<int>(castToImageY(point1.y));
//	cv::Point2i cvPoint1(x1, y1);
//	
//	int x2 = static_cast<int>(castToImageX(point2.x));
//	int y2 = static_cast<int>(castToImageY(point2.y));
//	cv::Point2i cvPoint2(x2, y2);
//	
//	cv::line(frame_, cvPoint1, cvPoint2, color, thickness);
//}
//
//
//void ScreenOpencv::writeText(const std::string& text, const b2Vec2& position)
//{
//	// Convert number to string
//	//std::string numberStr = std::to_string(str);
//
//	// Define the text properties
//	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
//	double fontScale = 0.5;
//	int thickness = 1;
//	int baseline = 0;
//	int padding = 5; // Padding around the text
//
//	// Get the size of the text
//	cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
//
//	// Calculate the position to write the text
//	cv::Point textPosition(castToImageX(position.x), castToImageY(position.y));
//
//	// Write the text on the image
//	cv::putText(frame_, text, textPosition, fontFace, fontScale, cv::Scalar(255, 0, 0), thickness);
//}
//
//
////
////void Screen::pasteImage(int xPos,int yPos, int recCols,int recRows)
////{
////	cv::Mat img = cv::imread("../data/ofir/RePAIR/group_39/images/Picture1.png");
////	//int xPos = 100;
////	//int yPos = 200;
////	cv::Mat destRoi;
////	try
////	{
////		//destRoi = frame_(cv::Rect(xPos, yPos, img.cols, img.rows));
////		frame_ = frame_(cv::Rect(xPos, yPos, 50, 50));
////	}
////	catch (const std::exception&)
////	{
////
////	}
////
////	img.copyTo(frame_);
////}