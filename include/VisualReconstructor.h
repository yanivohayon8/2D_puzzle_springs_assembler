#pragma once
#include <reconstruction.h>

class VisualReconstructor: public Reconstructor
{

public:
	bool isDebugDrawPolygons_ = false;
	bool isDrawSprites_ = true;
	void Run(std::string resultScreenshotPath = "");
};

