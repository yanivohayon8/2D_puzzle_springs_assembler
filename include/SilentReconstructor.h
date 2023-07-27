#pragma once
#include <reconstruction.h>
#include <map>
#include <string>

class SilentReconstructor: public Reconstructor
{
private:
	void progress(int numIteration);
	void snapshotPiecesCoords(std::map<std::string, std::vector<b2Vec2>>& oPiece2Coords, const b2Vec2 &translateCenter);
	
protected:	
	std::map<std::string, std::vector<b2Vec2>> piece2CoordsBeforeEnableCollision_;
	std::map<std::string, std::vector<b2Vec2>> piece2FinalCoords_;
	bool isScreenInitiated_ = false;
	int iterationToConvergeBeforeCollidePerPiece_ = 1000;
	int iterationToConvergeAfterCollidePerPiece_ = 500;

public:
	void Run(std::string screenshotPathBeforeCollide="", std::string screenshotPathAfterCollide="");
	std::map<std::string, std::vector<b2Vec2>>* getPiece2CoordsBeforeEnableCollision();
	std::map<std::string, std::vector<b2Vec2>>* getPiece2FinalCoords();
	void initScreen();//std::vector<Piece>& pieces
	bool isScreenInitiated();
	void setIterToConvAfterCollide(int numIterationPerPiece);
	void setIterToConvBeforeCollide(int numIterationPerPiece);
};

