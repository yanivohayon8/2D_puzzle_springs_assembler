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

nlohmann::json SilentReconstructor::snapshotTransformations(const b2Vec2& translateCenter, float coordinatesScale)
{
	nlohmann::json output = nlohmann::json::array();

	for (auto& piece : activePieces_)
	{
		b2Vec2 position;
		//piece.getBodyPosition(position);
		//position = piece.refb2Body_->GetTransform().p;
		////auto& position = transform.p;
		//position = piece.refb2Body_->GetPosition();
		piece.getBodyWorldCenterPosition(position);

		float x = position.x - translateCenter.x;
		x /= coordinatesScale;
		float y = position.y - translateCenter.y;
		y /= coordinatesScale;
		auto angle = piece.getBodyRotationRadians();

		nlohmann::json pieceJson;
		pieceJson["pieceId"] = piece.id_;
		pieceJson["translateVectorX"] = x;
		pieceJson["translateVectorY"] = y;
		pieceJson["rotationRadians"] = angle;
		output.push_back(pieceJson);
	}

	return output;
}



nlohmann::json SilentReconstructor::snapshotSpringsLength(std::vector<VertexMatingRePAIR*> &matings, 
															float coordinatesScale)
{
	nlohmann::json matingsJson = nlohmann::json::array();
	float sumLengths = 0;

	for (auto& mating : matings)
	{
		nlohmann::json matingJson;

		matingJson["firstPieceId"] = mating->firstPieceId_;
		double firstXscaled = mating->firstPieceLocalCoords_.x / coordinatesScale;
		double firstYscaled = mating->firstPieceLocalCoords_.y / coordinatesScale;
		matingJson["firstPieceLocalCoords"] = nlohmann::json::array({ firstXscaled,firstYscaled });

		matingJson["secondPieceId"] = mating->secondPieceId_;
		double secondXscaled = mating->secondPieceLocalCoords_.x / coordinatesScale;
		double secondYscaled = mating->secondPieceLocalCoords_.y / coordinatesScale;
		matingJson["secondPieceLocalCoords"] = nlohmann::json::array({ secondXscaled,secondYscaled });

		auto length = mating->jointRef_->GetCurrentLength();
		matingJson["jointLength"] = length;
		sumLengths += length;

		matingsJson.push_back(matingJson);
	}

	nlohmann::json output;
	output["springs"] = matingsJson;
	output["sumSpringsLength"] = sumLengths;

	return output;
}


nlohmann::json SilentReconstructor::snapshotPiecesCoords(const b2Vec2& translateCenter,float coordinatesScale)
{
	nlohmann::json output = nlohmann::json::array();

	for (auto& pieceIt : activePieces_)
	{
		nlohmann::json pieceJson;
		pieceJson["pieceId"] = pieceIt.id_;
		nlohmann::json coords = nlohmann::json::array();

		for (auto& coord : pieceIt.globalCoordinates_)
		{
			float x = coord.x - translateCenter.x;
			x = x / coordinatesScale;
			float y = coord.y - translateCenter.y;
			y = y / coordinatesScale;
			coords.push_back(nlohmann::json::array({ x,y }));
		}

		pieceJson["coordinates"] = coords;
		output.push_back(pieceJson);
	}

	return output;
}


nlohmann::json SilentReconstructor::reconstruct(float coordinatesScale)
{
	nlohmann::json output;
	//applyImpulseOnBodies(initPowerMagnitude_);

	//if (currentRequest_.has_param("collideOff"))
	//{
	//    output = silentReconstructor_->RunOffCollide(coordinatesScale);
	//}
	//else
	//{
	//    output = silentReconstructor_->RunOffOnCollide(coordinatesScale);
	//}

	//silentReconstructor_->screen_->closeWindow();
	//silentReconstructor_->closeRun();
	return output;
}



void SilentReconstructor::initRunNew(httplib::Request currentRequest, std::vector<Piece> activePieces,
	std::vector<VertexMatingRePAIR*> activeMatings)
{
	Reconstructor::initRunNew(currentRequest,activePieces,activeMatings);

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
	output["jointsAfterEnableCollision"] = snapshotSpringsLength(activeMatings_, coordinatesScale);
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