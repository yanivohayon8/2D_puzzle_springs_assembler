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

	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);

	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	piece2CoordsBeforeEnableCollision_.clear();
	snapshotPiecesCoords(piece2CoordsBeforeEnableCollision_, centerOfBoard);

	setPiecesCollisionOn();
	
	// Apply impulse on bodies
	int impulseIndex = 0;
	//float powerMagnitude = 0.2;//2
	std::vector<b2Vec2> initialImpulses = {
		{initPowerMagnitude_,initPowerMagnitude_},
		{-initPowerMagnitude_,initPowerMagnitude_},
		{-initPowerMagnitude_,-initPowerMagnitude_},
		{-initPowerMagnitude_,initPowerMagnitude_}
	};
	int numInitialImpulses = initialImpulses.size();

	for (auto& piece : activePieces_)
	{
		auto& power = initialImpulses[++impulseIndex % numInitialImpulses];
		piece.applyLinearImpulse(power.x, power.y);
	}


	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);

	piece2FinalCoords_.clear();
	snapshotPiecesCoords(piece2FinalCoords_, centerOfBoard);
	
	piece2FinalTransformation_.clear();
	snapshotPiecesTransformation(piece2FinalTransformation_, centerOfBoard);
	
	for (auto& mating : activeMatings_)
	{
		mating.snapshotJointLength();
	}

	if (isDebugScreenVisible_)
	{
		screen_->closeWindow();
		isDebugScreenVisible_ = false;
	}
}
