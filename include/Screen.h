#pragma once
#include <opencv2/highgui/highgui.hpp>

class Screen
{
public:
	Screen(int height, int width, int scale);
	cv::Mat frame_;
	int height_;
	int width_;
	int scale_; 
};
