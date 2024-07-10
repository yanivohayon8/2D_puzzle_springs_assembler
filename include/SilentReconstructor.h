#pragma once
#include <reconstruction.h>
#include <map>
#include <string>

class SilentReconstructor: public Reconstructor
{
private:
	
protected:	
	void snapshotPiecesCoords(std::map<std::string, std::vector<b2Vec2>>& oPiece2Coords, const b2Vec2 &translateCenter);
	void snapshotPiecesTransformation(std::map<std::string, std::pair<float, b2Vec2>>& oPiece2Transform, const b2Vec2& translateCenter);
	void progress(int numIteration);
	std::map<std::string, std::vector<b2Vec2>> piece2CoordsBeforeEnableCollision_;
	std::map<std::string, std::vector<b2Vec2>> piece2FinalCoords_;
	std::map<std::string, std::pair<float, b2Vec2>> piece2FinalTransformation_;
	bool isScreenInitiated_ = false;
	int iterationToConvergeBeforeCollidePerPiece_ = 1000;
	int iterationToConvergeAfterCollidePerPiece_ = 500;
	bool isDebugScreenVisible_ = false;
	sf::Color springColor_ = sf::Color::Red;
	int nextPolygonColorIndex_ = 0;

public:
	SilentReconstructor();
	SilentReconstructor(float boardWidth, float boardHeight, int screenWidth_, int screenHeight_);
	void Run(std::string screenshotPathBeforeCollide="", std::string screenshotPathAfterCollide="");
	std::map<std::string, std::vector<b2Vec2>>* getPiece2CoordsBeforeEnableCollision();
	std::map<std::string, std::vector<b2Vec2>>* getPiece2FinalCoords();
	void getPiece2FinalTransformation(std::map<std::string, std::pair<float, b2Vec2>>& piece2FinalTransformation);

	void initScreen();//std::vector<Piece>& pieces
	bool isScreenInitiated();
	void setDebugScreenVisibility(bool isVisible);
	void setIterToConvAfterCollide(int numIterationPerPiece);
	void setIterToConvBeforeCollide(int numIterationPerPiece);


	nlohmann::json snapshotTransformations(const b2Vec2& translateCenter, float coordinatesScale);
	nlohmann::json snapshotSpringsLength(std::vector<VertexMatingRePAIR*>& matings, float coordinatesScale);
	nlohmann::json snapshotPiecesCoords(const b2Vec2& translateCenter, float coordinatesScale);
	nlohmann::json RunOffCollide(float coordinatesScale);
	nlohmann::json RunOffOnCollide(float coordinatesScale);

	nlohmann::json reconstruct();
};

