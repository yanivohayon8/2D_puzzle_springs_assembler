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
	/*auto redColor = sf::Color::Red;
	bool isScreenVisible = false;
	screen_->initDisplay(isScreenVisible);
	screen_->clearDisplay();

	for (auto& piece : activePieces_)
	{
		screen_->initSprite(piece);
	}*/

	int iteration = 0;
	int iterationToConvergePerPiece = 1000;
	int iterationToConverge = activePieces_.size() * iterationToConvergePerPiece;
	progress(iterationToConverge);

	const b2Vec2& centerOfBoard = fixedPiece_->refb2Body_->GetTransform().p;
	snapshotPiecesCoords(piece2CoordsBeforeEnableCollision_, centerOfBoard);

	//debug
	for (auto& mating : activeMatings_)
	{
		mating.snapshotJointLength();
	}

	// debug
	/*for (auto& piece : activePieces_)
	{
		screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
	}

	screen_->screenShot("../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0/disable_collide.png");
	screen_->clearDisplay();*/

	for (auto& piece : activePieces_)
	{
		piece.setCollideOn();
	}

	int iterationToSecondConverage = iterationToConverge / 2;
	progress(iterationToSecondConverage);
	
	for (auto& mating: activeMatings_)
	{
		mating.snapshotJointLength();
	}

	// Debug
	/*for (auto& mating : activeMatings_)
	{
		auto& anchorA = mating.jointRef_->GetAnchorA();
		auto& anchorB = mating.jointRef_->GetAnchorB();
		screen_->drawLine(anchorA, anchorB, redColor, -1);
	}

	for (auto& piece : activePieces_)
	{
		screen_->drawSprite(piece.id_, piece.refb2Body_->GetTransform());
	}

	screen_->screenShot("../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0/enable_collide.png");
	screen_->closeWindow();*/
}
