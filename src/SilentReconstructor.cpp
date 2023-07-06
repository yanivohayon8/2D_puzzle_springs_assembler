#include "SilentReconstructor.h"

void SilentReconstructor::Run(std::string resultScreenshotPath)
{
	int iteration = 0;

	if (iteration < 1000)
	{
		world_.Step(timeStep_, velocityIterations_, positionIterations_);

		for (auto pieceIt = activePieces_.begin(); pieceIt != activePieces_.end(); pieceIt++)
		{
			pieceIt->translate();
		}
	}

	if (resultScreenshotPath !="")
	{
		screen_->initDisplay();
		for (auto& piece : activePieces_)
		{
			screen_->initSprite(piece);
			screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
		}

		screen_->screenShot(resultScreenshotPath);
		screen_->closeWindow();
	}

}

//void SilentReconstructor::Run(std::string resultScreenShotPath)
//{
//}
