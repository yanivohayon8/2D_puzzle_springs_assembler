#include "RePAIRSilentReconstructor.h"


RePAIRSilentReconstructor::RePAIRSilentReconstructor():SilentReconstructor()
{
	isEnableJointsCollide_ = true;
	jointRestLength_ = 0.05;
}

RePAIRSilentReconstructor::RePAIRSilentReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight):SilentReconstructor(boardWidth,boardHeight,screenWidth,screenHeight)
{
	/*isEnableJointsCollide_ = true;
	jointRestLength_ = 0.05;*/
}

void RePAIRSilentReconstructor::Run(std::string screenshotPathBeforeCollide , std::string screenshotPathAfterCollide)
{
	
}

void RePAIRSilentReconstructor::RunCollisionOff()
{
	/*
		Set collision off the entire running
	*/
	if (isDebugScreenVisible_)
	{
		initScreen();
	}

	setPiecesCollisionOff();
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);

	piece2FinalCoords_.clear();
	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	snapshotPiecesCoords(piece2FinalCoords_, centerOfBoard);

	//piece2FinalTransformation_.clear();
	//snapshotPiecesTransformation(piece2FinalTransformation_, centerOfBoard);

	for (auto& mating : activeMatings_)
	{
		mating->snapshotJointLength();
	}

	if (isDebugScreenVisible_)
	{
		screen_->closeWindow();
		isDebugScreenVisible_ = false;
	}
}

void RePAIRSilentReconstructor::RunCollisionOffThenOn()
{
	/*
		In the beginning, set collisionOff and then set collision on
	*/

	if (isDebugScreenVisible_)
	{
		initScreen();
	}

	setPiecesCollisionOff();
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);

	setPiecesCollisionOn();
	applyImpulseOnBodies(0.5);

	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);
	
	piece2FinalCoords_.clear();
	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	snapshotPiecesCoords(piece2FinalCoords_, centerOfBoard);

	piece2FinalTransformation_.clear();
	snapshotPiecesTransformation(piece2FinalTransformation_, centerOfBoard);

	for (auto& mating : activeMatings_)
	{
		mating->snapshotJointLength();
	}

	if (isDebugScreenVisible_)
	{
		screen_->closeWindow();
		isDebugScreenVisible_ = false;
	}
}

void RePAIRSilentReconstructor::RunCollisionOn()
{
	/*
		Set collision on the entire running
	*/
	if (isDebugScreenVisible_)
	{
		initScreen();
	}

	setPiecesCollisionOn();
	applyImpulseOnBodies(5);
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);

	piece2FinalCoords_.clear();
	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	snapshotPiecesCoords(piece2FinalCoords_, centerOfBoard);

	for (auto& mating : activeMatings_)
	{
		mating->snapshotJointLength();
	}

	if (isDebugScreenVisible_)
	{
		screen_->closeWindow();
		isDebugScreenVisible_ = false;
	}
}
