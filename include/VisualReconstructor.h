//#pragma once
#include <reconstruction.h>
#include <json.hpp>


class VisualReconstructor: public Reconstructor
{

public:
	VisualReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight);
	bool isDebugDrawPolygons_ = true;
	bool isDrawSprites_ = true;
	void Run(std::string screenshotPathBeforeCollide = "", std::string screenshotPathAfterCollide = "");

	nlohmann::json reconstruct(float coordinatesScale);
	void initRunNew(httplib::Request currentRequest, std::vector<Piece> activePieces,
		std::vector<VertexMatingRePAIR*> activeMatings);
};

