#pragma once
#include <reconstruction.h>

class VisualReconstructor: public Reconstructor
{

public:
	VisualReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight);
	bool isDebugDrawPolygons_ = false;
	bool isDrawSprites_ = true;
	void Run(std::string screenshotPathBeforeCollide = "", std::string screenshotPathAfterCollide = "");
};

