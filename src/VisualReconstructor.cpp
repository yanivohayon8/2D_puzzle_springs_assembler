#include "VisualReconstructor.h"



VisualReconstructor::VisualReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight):Reconstructor(boardWidth,boardHeight,screenWidth,screenHeight)
{
}

nlohmann::json VisualReconstructor::reconstruct(float coordinatesScale)
{
	nlohmann::json output;


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
				//screen_->drawPolygonDots(pieceIt->id_, pieceIt->globalCoordinates_);

				/*auto& world_center_debug = pieceIt->refb2Body_->GetWorldCenter();
				std::cout << "World center: " << world_center_debug.x << "," << world_center_debug.y << std::endl;*/

				screen_->drawCircle(pieceIt->refb2Body_->GetWorldCenter(), 0.05, sf::Color(0, 0, 255));
				screen_->drawCircle(transform.p, 0.05, sf::Color(255, 0, 255));
			}

			if (isDrawSprites_)
			{
				screen_->drawSprite(pieceIt->id_, transform);
			}

		}

		drawJoints();

		screen_->updateDisplay();

		sf::Event nextEvent;
		while (screen_->pollEvent(nextEvent))
		{

			if (nextEvent.type == sf::Event::Closed)
				screen_->closeWindow();
			else {

				if (nextEvent.type == sf::Event::KeyPressed)
				{
					switch (nextEvent.key.code)
					{
					case sf::Keyboard::P:
						isDebugDrawPolygons_ = !isDebugDrawPolygons_;
						break;
					case sf::Keyboard::O:
						isDrawSprites_ = !isDrawSprites_;
						break;
					case sf::Keyboard::E:
						std::cout << "Exploding" << std::endl;
						for (auto& piece : activePieces_)
						{
							piece.applyLinearImpulse(10, 10);
						}
						break;
					case sf::Keyboard::W:
						std::cout << "Exploding" << std::endl;
						for (auto& piece : activePieces_)
						{
							piece.applyLinearImpulse(-100, -100);
						}
						break;
					case sf::Keyboard::C:
						std::cout << "Set collide on" << std::endl;
						for (auto& piece : activePieces_)
						{
							piece.setCollideOn();
						}
						break;
					case sf::Keyboard::X:
						std::cout << "Set collide off" << std::endl;
						for (auto& piece : activePieces_)
						{
							piece.setCollideOff();
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	output["joints"] = snapshotSpringsLength(activeMatings_, coordinatesScale);
	const b2Vec2& centerOfAssemblyAfter = fixedPiece_->refb2Body_->GetTransform().p;
	output["piecesFinalCoords"] = snapshotPiecesCoords(centerOfAssemblyAfter, coordinatesScale);
	output["piecesFinalTransformations"] = snapshotTransformations(centerOfAssemblyAfter, coordinatesScale);


	return output;
}

void VisualReconstructor::initRunNew(httplib::Request currentRequest, std::vector<Piece> activePieces,
	std::vector<VertexMating*> activeMatings)
{
	Reconstructor::initRunNew(currentRequest, activePieces, activeMatings);
	initScreenNew(true);
}