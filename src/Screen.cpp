#include "Screen.h"

Screen::Screen(int height, int width, int scale)
{
	height_ = height;
	width_ = width;
	scale_ = scale;
	frame_ = cv::Mat(height_, width_, CV_8UC3);
}
