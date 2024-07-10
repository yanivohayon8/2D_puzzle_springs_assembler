#pragma once
#include <reconstruction.h>
#include <map>
#include <string>

class SilentReconstructor: public Reconstructor
{
private:
	
protected:	
	void progress(int numIteration);
	//bool isScreenInitiated_ = false;
	int iterationToConvergeBeforeCollidePerPiece_ = 1000;
	int iterationToConvergeAfterCollidePerPiece_ = 500;	
	int nextPolygonColorIndex_ = 0;

public:
	SilentReconstructor();
	SilentReconstructor(float boardWidth, float boardHeight, int screenWidth_, int screenHeight_);
	
	void initScreen();
	bool isScreenInitiated();
	void setDebugScreenVisibility(bool isVisible);
	void setIterToConvAfterCollide(int numIterationPerPiece);
	void setIterToConvBeforeCollide(int numIterationPerPiece);


	nlohmann::json snapshotTransformations(const b2Vec2& translateCenter, float coordinatesScale);
	nlohmann::json snapshotSpringsLength(std::vector<VertexMatingRePAIR*>& matings, float coordinatesScale);
	nlohmann::json snapshotPiecesCoords(const b2Vec2& translateCenter, float coordinatesScale);
	

	nlohmann::json reconstruct(float coordinatesScale);

	void initRunNew(httplib::Request currentRequest, std::vector<Piece> activePieces,
		std::vector<VertexMatingRePAIR*> activeMatings);
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
