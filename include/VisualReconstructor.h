//#pragma once
#include <reconstruction.h>
#include <json.hpp>


class InteractiveReconstructor : public Reconstructor
{

public:
	InteractiveReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight);
	bool isDebugDrawPolygons_ = true;
	bool isDrawSprites_ = true;

	nlohmann::json reconstruct(float coordinatesScale);
	void initRun(httplib::Request currentRequest, std::vector<Piece> activePieces,
		std::vector<VertexMating*> activeMatings);
};

