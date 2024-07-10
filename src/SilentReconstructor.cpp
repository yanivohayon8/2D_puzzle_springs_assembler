#include "SilentReconstructor.h"


SilentReconstructor::SilentReconstructor(float boardWidth, float boardHeight, int screenWidth_, int screenHeight_):Reconstructor(boardWidth,boardHeight,screenWidth_,screenHeight_)
{

}

//SilentReconstructor::SilentReconstructor():Reconstructor(40,40,1380,1380)
//{
//
//}
SilentReconstructor::SilentReconstructor() :Reconstructor(60, 60, 1380, 1380)
{

}

bool SilentReconstructor::isScreenInitiated()
{
	return isScreenInitiated_;
}

void SilentReconstructor::initScreen()//std::vector<Piece>& pieces
{
	//bool isScreenVisible = false;
	screen_->initDisplay(isDebugScreenVisible_);

	for (auto& piece : activePieces_)
	{
		//screen_->initSprite(piece);
		screen_->initPolygon(piece);
	}

	nextPolygonColorIndex_ = 0;

	screen_->clearDisplay();
	isScreenInitiated_ = true;
}

std::map<std::string, std::vector<b2Vec2>>* SilentReconstructor::getPiece2CoordsBeforeEnableCollision()
{
	return &piece2CoordsBeforeEnableCollision_;
}

std::map<std::string, std::vector<b2Vec2>>* SilentReconstructor::getPiece2FinalCoords()
{
	return &piece2FinalCoords_;
}

void SilentReconstructor::snapshotPiecesCoords(std::map<std::string, std::vector<b2Vec2>>& oPiece2Coords, const b2Vec2& translateCenter)
{

	for (auto& piece: activePieces_)
	{
		std::vector<b2Vec2> coords;

		for (auto& coord: piece.globalCoordinates_)
		{
			float x = coord.x - translateCenter.x;
			float y = coord.y - translateCenter.y;
			coords.push_back(b2Vec2(x,y));
		}

		oPiece2Coords[piece.id_] = coords;
	}
}

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

		if (isDebugScreenVisible_)
		{
			screen_->clearDisplay();

			for (auto pieceIt = activePieces_.begin(); pieceIt != activePieces_.end(); pieceIt++)
			{
				const b2Transform& transform = pieceIt->refb2Body_->GetTransform();			
				screen_->drawPolygon(pieceIt->id_, transform);
				//screen_->drawSprite(pieceIt->id_, transform);
			}

			for (auto& mating : activeMatings_)
			{
				auto& anchorA = mating->jointRef_->GetAnchorA();
				auto& anchorB = mating->jointRef_->GetAnchorB();
				screen_->drawLine(anchorA, anchorB, springColor_, -1);
			}

			screen_->updateDisplay();
		}

		
	}
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

void SilentReconstructor::Run(std::string screenshotPathBeforeCollide, std::string screenshotPathAfterCollide)
{
	if (isDebugScreenVisible_ || screenshotPathBeforeCollide != "" || screenshotPathAfterCollide != "")
	{
		initScreen();
	}
	
	int iteration = 0;
	//int iterationToConvergePerPiece = 1000;
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);

	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	piece2CoordsBeforeEnableCollision_.clear();
	snapshotPiecesCoords(piece2CoordsBeforeEnableCollision_, centerOfBoard);

	//debug
	for (auto& mating : activeMatings_)
	{
		mating->snapshotJointLength();
	}

	
	for (auto& piece : activePieces_)
	{
		piece.setCollideOn();
	}

	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);
	
	for (auto& mating: activeMatings_)
	{
		mating->snapshotJointLength();
	}

	piece2FinalCoords_.clear();
	snapshotPiecesCoords(piece2FinalCoords_, centerOfBoard);

	piece2FinalTransformation_.clear();
	snapshotPiecesTransformation(piece2FinalTransformation_, centerOfBoard);

	// Debug
	if (screenshotPathAfterCollide!="")
	{
		screen_->clearDisplay();
		auto redColor = sf::Color::Red;

		for (auto& mating : activeMatings_)
		{
			auto& anchorA = mating->jointRef_->GetAnchorA();
			auto& anchorB = mating->jointRef_->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, -1);
		}

		for (auto& piece : activePieces_)
		{
			//screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
			screen_->drawPolygon(piece.id_, piece.refb2Body_->GetTransform());
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

void SilentReconstructor::snapshotPiecesTransformation(std::map<std::string, std::pair<float, b2Vec2>>& oPiece2Transform, const b2Vec2& translateCenter)
{
	for (auto& piece : activePieces_)
	{

		auto id_tmp = piece.id_;
		b2Vec2 worldCenter1;
		b2Vec2 worldCenter2;
		//piece.getBodyPosition(worldCenter1);
		piece.getBodyWorldCenterPosition(worldCenter2);
		//piece.getBodyPosition(worldCenter2);
		
		//const b2Transform& transform = piece.refb2Body_->GetTransform();
		////auto& position = transform.p;
		//b2Vec2 position = transform.p;

		//b2Vec2 position = piece.refb2Body_->GetPosition();
		//std::cout << get_position.x << " " << get_position.y << std::endl;
		//std::cout << position.x << " " << position.y << std::endl;

		//float x = position.x - translateCenter.x;
		//float y = position.y - translateCenter.y;
		/*float x = worldCenter1.x - translateCenter.x;
		float y = worldCenter1.y - translateCenter.y;*/

		float x = worldCenter2.x - translateCenter.x;
		float y = worldCenter2.y - translateCenter.y;

		auto angle = piece.getBodyRotationRadians();
		std::pair<float, b2Vec2> AngleAndTranslatevector(angle,b2Vec2(x, y));
		oPiece2Transform[piece.id_] = AngleAndTranslatevector;
	}
}

void SilentReconstructor::getPiece2FinalTransformation(std::map<std::string, std::pair<float, b2Vec2>>& piece2FinalTransformation)
{
	piece2FinalTransformation = piece2FinalTransformation_;
}






//// new functions from here


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



nlohmann::json SilentReconstructor::RunOnCollide()
{
	nlohmann::json output;
	
	return output;
}

nlohmann::json SilentReconstructor::RunOffCollide()
{
	nlohmann::json output;
	
	return output;
}

nlohmann::json SilentReconstructor::RunOffOnCollide(float coordinatesScale)
{
	nlohmann::json output;

	setPiecesCollisionOff();
	int iterationToConverge = activePieces_.size() * iterationToConvergeBeforeCollidePerPiece_;
	progress(iterationToConverge);
	const b2Vec2& centerOfAssemblyBefore = fixedPiece_->refb2Body_->GetTransform().p;
	output["piecesBeforeEnableCollision"] = snapshotPiecesCoords(centerOfAssemblyBefore, coordinatesScale);

	setPiecesCollisionOn();
	int iterationToSecondConverage = activePieces_.size() * iterationToConvergeAfterCollidePerPiece_;
	progress(iterationToSecondConverage);
	const b2Vec2& centerOfAssemblyAfter = fixedPiece_->refb2Body_->GetTransform().p;
	output["piecesAfterEnableCollision"] = snapshotPiecesCoords(centerOfAssemblyAfter, coordinatesScale);

	return output;
}