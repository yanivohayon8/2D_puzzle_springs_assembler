#include "RePAIRSilentReconstructor.h"


void RePAIRSilentReconstructor::Run(std::string screenshotPathBeforeCollide = "", std::string screenshotPathAfterCollide = "")
{
	if (isDebugScreenVisible_ || screenshotPathBeforeCollide != "" || screenshotPathAfterCollide != "")
	{
		initScreen();
	}

	int iteration = 0;
	//int iterationToConvergePerPiece = 1000;
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);

	for (auto& piece : activePieces_)
	{
		piece.setCollideOn();
	}

	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);


	// Debug
	if (screenshotPathAfterCollide != "")
	{
		screen_->clearDisplay();
		auto redColor = sf::Color::Red;

		for (auto& mating : activeMatings_)
		{
			auto& anchorA = mating.jointRef_->GetAnchorA();
			auto& anchorB = mating.jointRef_->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, -1);
		}

		for (auto& piece : activePieces_)
		{
			screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
		}

		screen_->screenShotToFile(screenshotPathAfterCollide);
	}


	if (screenshotPathBeforeCollide != "" || screenshotPathAfterCollide != "")
	{
		screen_->closeWindow();
	}

	if (isDebugScreenVisible_)
	{
		screen_->closeWindow();
		isDebugScreenVisible_ = false;
	}
}
