#include "SilentReconstructor.h"


SilentReconstructor::SilentReconstructor(float boardWidth, float boardHeight, int screenWidth_, int screenHeight_):Reconstructor(boardWidth,boardHeight,screenWidth_,screenHeight_)
{

}

SilentReconstructor::SilentReconstructor() :Reconstructor(60, 60, 1380, 1380)
{

}

bool SilentReconstructor::isScreenInitiated()
{
	return isScreenInitiated_;
}

void SilentReconstructor::setIterToConvBeforeCollide(int numIterationPerPiece)
{
	iterationToConvergeBeforeCollidePerPiece_ = numIterationPerPiece;
}

void SilentReconstructor::setIterToConvAfterCollide(int numIterationPerPiece)
{
	iterationToConvergeAfterCollidePerPiece_ = numIterationPerPiece;
}

void SilentReconstructor::setDebugScreenVisibility(bool isVisible)
{
	isDebugScreenVisible_ = isVisible;
}



//// new functions from here
void SilentReconstructor::progress(int numIteration)
{
	Reconstructor::progress(numIteration);
}



nlohmann::json SilentReconstructor::reconstruct(float coordinatesScale)
{
	nlohmann::json output;
	return output;
}



void SilentReconstructor::initRun(httplib::Request currentRequest, std::vector<Piece> activePieces,
	std::vector<VertexMating*> activeMatings)
{
	Reconstructor::initRun(currentRequest,activePieces,activeMatings);

	setDebugScreenVisibility(false);

	if (currentRequest.has_param("visibilityOn"))
	{
	    setDebugScreenVisibility(true);
		initScreenNew(true);
	}
}


OffCollideSilentReconstructor::OffCollideSilentReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight) :SilentReconstructor(boardWidth, boardHeight, screenWidth, screenHeight)
{
}


nlohmann::json OffCollideSilentReconstructor::reconstruct(float coordinatesScale)
{
	nlohmann::json output;

	setPiecesCollisionOff();
	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);
	output["joints"] = snapshotSpringsLength(activeMatings_, coordinatesScale);
	const b2Vec2& centerOfAssemblyAfter = fixedPiece_->refb2Body_->GetTransform().p;
	output["piecesFinalCoords"] = snapshotPiecesCoords(centerOfAssemblyAfter, coordinatesScale);
	output["piecesFinalTransformations"] = snapshotTransformations(centerOfAssemblyAfter, coordinatesScale);

	return output;
}

OffOnCollideSilentReconstructor::OffOnCollideSilentReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight) :SilentReconstructor(boardWidth, boardHeight, screenWidth, screenHeight)
{
}

nlohmann::json OffOnCollideSilentReconstructor::reconstruct(float coordinatesScale)
{
	nlohmann::json output;

	setPiecesCollisionOff();
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);
	const b2Vec2& centerOfAssemblyBefore = fixedPiece_->refb2Body_->GetTransform().p;
	output["piecesBeforeEnableCollisionCoords"] = snapshotPiecesCoords(centerOfAssemblyBefore, coordinatesScale);

	applyImpulseOnBodies(0.5);
	setPiecesCollisionOn();
	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);
	output["jointsAfterEnableCollision"] = snapshotSpringsLength(activeMatings_, coordinatesScale);
	const b2Vec2& centerOfAssemblyAfter = fixedPiece_->refb2Body_->GetTransform().p;
	output["piecesFinalCoords"] = snapshotPiecesCoords(centerOfAssemblyAfter, coordinatesScale);
	output["piecesFinalTransformations"] = snapshotTransformations(centerOfAssemblyAfter, coordinatesScale);

	return output;
}