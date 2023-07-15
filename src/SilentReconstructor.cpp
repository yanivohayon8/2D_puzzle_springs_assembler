#include "SilentReconstructor.h"

std::map<std::string, std::vector<b2Vec2>>* SilentReconstructor::getPiece2CoordsBeforeEnableCollision()
{
	return &piece2CoordsBeforeEnableCollision_;
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

	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	snapshotPiecesCoords(piece2CoordsBeforeEnableCollision_, centerOfBoard);

	/*for (auto& piece : activePieces_)
	{
		screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
	}

	screen_->screenShot("../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0/disable_collide.png");
	screen_->clearDisplay();*/

	int iterationToSecondConverage = iterationToConverge / 2;
	for (auto& piece : activePieces_)
	{
		piece.setCollideOn();
	}

	progress(iterationToSecondConverage);
	
	for (auto& mating: activeMatings_)
	{
		mating.snapshotJointLength();
	}

	/*const b2Vec2& centerOfAssembly = fixedPiece_->refb2Body_->GetTransform().p;
	saveFinalTransforms("../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0/final_transform_debug.csv", centerOfAssembly);*/

		/*computePiecesBoostPolygons();
		piecesOverlappingArea_ = computeOverlapAreaDiceCoeff();*/

		/*int iterationToSecondConverage = iterationToConverge / 2;
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

		screen_->closeWindow();*/

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
