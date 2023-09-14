#include "RePAIRSilentReconstructor.h"


RePAIRSilentReconstructor::RePAIRSilentReconstructor():SilentReconstructor()
{
	isEnableJointsCollide_ = true;
	jointRestLength_ = 0.05;
}

void RePAIRSilentReconstructor::Run(std::string screenshotPathBeforeCollide , std::string screenshotPathAfterCollide)
{
	if (isDebugScreenVisible_ || screenshotPathBeforeCollide != "" || screenshotPathAfterCollide != "")
	{
		initScreen();
	}

	int iteration = 0;
	//setPiecesCollisionOn();
	//int iterationToConvergePerPiece = 1000;
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);

	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	piece2CoordsBeforeEnableCollision_.clear();
	snapshotPiecesCoords(piece2CoordsBeforeEnableCollision_, centerOfBoard);

	setPiecesCollisionOn();

	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);

	piece2FinalCoords_.clear();
	snapshotPiecesCoords(piece2FinalCoords_, centerOfBoard);

	

	/*int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);*/


	

	if (isDebugScreenVisible_)
	{
		screen_->closeWindow();
		isDebugScreenVisible_ = false;
	}
}
