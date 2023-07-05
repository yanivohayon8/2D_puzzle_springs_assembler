#pragma once
//#include <World.h>
#include <Piece.h>
#include <box2d/box2d.h>
#include <VertexMating.h>
#include "SfmlScreen.h"
#include <numeric>
#include "Utils.h"
#include <iostream>
#include <fstream>

class Reconstructor
{
private:
	b2World world_ = b2World(b2Vec2(0, 0));
	std::vector<Piece> activePieces_;
	std::vector<VertexMating> activeMatings_;
	float boardHeight_ = 10;//20; 
	float boardWidth_ = 10;//20;//note also the recommondation of static bodies (no more than 50!)
	std::vector<std::vector<b2Vec2>> boundsCoordinates_;
	b2Body* createPieceBody(Piece& piece, b2BodyDef& bodyDef, b2FixtureDef& fixture);
	void initStaticBody(Piece& piece, b2Vec2& position);
	void initMovingBody(Piece& piece, b2Vec2& initialPosition);
	Piece* getMaxMatingsPiece();

	//void countMatingsPerPiece();

public:
	Reconstructor(float boardWidth = 10,float boardHeight=10);
	void init();
	void initRun(std::vector<Piece>& activePieces, std::vector<VertexMating>& activeMatings, int positionSeed = 0, int positionPadding = 0);
	virtual void Run()=0;

	//void initPieces();
	//void countMatings(std::vector<int>& oPiecesCounters);
	//void close(); // delete the matings and pieces...
};


class SilentReconstructor : public Reconstructor
{
	void Run();
};


class VisualReconstructor : public Reconstructor
{
	void Run();
};