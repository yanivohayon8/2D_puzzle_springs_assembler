#include "SilentReconstructor.h"

void SilentReconstructor::progress(int numIteration)
{
	int iteration = 0;

	while (++iteration < numIteration)
	{
		world_.Step(timeStep_, velocityIterations_, positionIterations_);

		for (auto pieceIt = activePieces_.begin(); pieceIt != activePieces_.end(); pieceIt++)
		{
			pieceIt->translate();
		}
	}
}

void SilentReconstructor::Run(std::string resultScreenshotPath)
{
	bool isScreenVisible = false;
	screen_->initDisplay(isScreenVisible);
	screen_->clearDisplay();

	for (auto& piece : activePieces_)
	{
		screen_->initSprite(piece);
	}

	int iteration = 0;
	int iterationToConvergePerPiece = 1000;
	int iterationToConverge = activePieces_.size() * iterationToConvergePerPiece;
	progress(iterationToConverge);
	piecesOverlappingArea_ = computePiecesOverlapAreaPercentage();
	
	for (auto& piece : activePieces_)
	{
		screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
	}

	screen_->screenShot("../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0/disable_collide.png");
	screen_->clearDisplay();

	int iterationToSecondConverage = iterationToConverge / 2;
	for (auto& piece : activePieces_)
	{
		piece.setCollideOn();
	}

	progress(iterationToSecondConverage);

	for (auto& piece : activePieces_)
	{
		screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
	}

	float overlapDebug = computePiecesOverlapAreaPercentage();
	std::cout << "Overlapping area after enable collision is " << overlapDebug << std::endl;

	screen_->screenShot("../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0/enable_collide.png");
	screen_->clearDisplay();

	screen_->closeWindow();

	/*if (resultScreenshotPath !="")
	{
		bool isScreenVisible = false;
		screen_->initDisplay(isScreenVisible);
		screen_->clearDisplay();

		for (auto& piece : activePieces_)
		{
			screen_->initSprite(piece);
			screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
		}

		screen_->screenShot(resultScreenshotPath);
		screen_->closeWindow();
	}*/

	/// Todo: compute the distances between ground truth vertices
}
