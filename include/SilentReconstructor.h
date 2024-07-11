#pragma once
#include <reconstruction.h>
#include <map>
#include <string>

class SilentReconstructor: public Reconstructor
{
private:
	
protected:	
	void progress(int numIteration);
	int iterationToConvergeBeforeCollidePerPiece_ = 1000;
	int iterationToConvergeAfterCollidePerPiece_ = 500;	
	int nextPolygonColorIndex_ = 0;

public:
	SilentReconstructor();
	SilentReconstructor(float boardWidth, float boardHeight, int screenWidth_, int screenHeight_);
	
	bool isScreenInitiated();
	void setDebugScreenVisibility(bool isVisible);
	void setIterToConvAfterCollide(int numIterationPerPiece);
	void setIterToConvBeforeCollide(int numIterationPerPiece);
	nlohmann::json reconstruct(float coordinatesScale);

	void initRunNew(httplib::Request currentRequest, std::vector<Piece> activePieces,
		std::vector<VertexMating*> activeMatings);
};


class OffCollideSilentReconstructor : public SilentReconstructor
{
public:
	OffCollideSilentReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight);
	nlohmann::json reconstruct(float coordinatesScale);
};

class OffOnCollideSilentReconstructor : public SilentReconstructor
{
public:
	OffOnCollideSilentReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight);
	nlohmann::json reconstruct(float coordinatesScale);
};
