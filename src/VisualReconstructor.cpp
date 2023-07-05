#include "VisualReconstructor.h"

void VisualReconstructor::Run()
{
	
	screen_->initDisplay();
	screen_->initBounds(boundsCoordinates_);
	auto redColor = sf::Color::Red;

	for (auto& piece : activePieces_)
	{
		screen_->initSprite(piece);
		screen_->initPolygon(piece);
		screen_->initPolygonCoordsDots(piece, 0.01, sf::Color(0, 255, 0));
	}

	while (screen_->isWindowOpen())
	{
		
		screen_->clearDisplay();
		screen_->drawBounds();
		world_.Step(timeStep_, velocityIterations_, positionIterations_);

		for (auto pieceIt = activePieces_.begin(); pieceIt != activePieces_.end(); pieceIt++)
		{
			pieceIt->translate();

			const b2Transform& transform = pieceIt->refb2Body_->GetTransform();

			//// FOR DEBUG
			if (isDebugDrawPolygons_)
			{
				screen_->drawPolygon(pieceIt->id_, transform);
				screen_->drawPolygonDots(pieceIt->id_, pieceIt->globalCoordinates_);
				screen_->drawCircle(pieceIt->refb2Body_->GetWorldCenter(), 0.05, sf::Color(0, 0, 255));
				screen_->drawCircle(transform.p, 0.05, sf::Color(255, 0, 0));
			}


			if (isDrawSprites_)
			{
				screen_->drawSprite(pieceIt->id_, transform);
			}

		}

		for (auto& joint : joints_)
		{
			auto& anchorA = joint->GetAnchorA();
			auto& anchorB = joint->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, -1);
		}

		screen_->updateDisplay();
	}

	screen_->closeWindow();
}
