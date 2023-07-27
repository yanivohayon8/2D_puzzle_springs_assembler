#include "VisualReconstructor.h"

void VisualReconstructor::Run(std::string screenshotPathBeforeCollide, std::string screenshotPathAfterCollide)
{
	
	screen_->initDisplay();
	screen_->initBounds(boundsCoordinates_);
	auto redColor = sf::Color::Red;
	float linearDamping = 0;

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

		for (auto& mating : activeMatings_)
		{
			auto& anchorA = mating.jointRef_->GetAnchorA();
			auto& anchorB = mating.jointRef_->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, -1);
		}

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
							int power = sampleIntUniformly(5, -5, -1);
							piece.applyLinearImpulse(power, power);
						}
						break;
					case sf::Keyboard::R:
						linearDamping += 0.1;
						for (auto& piece : activePieces_)
						{
							piece.setLinearDamping(linearDamping);
						}
						break;
					case sf::Keyboard::T:
						linearDamping -= 0.1;
						if (linearDamping < 0)
						{
							linearDamping = 0;
						}
						else {
							for (auto& piece : activePieces_)
							{
								piece.setLinearDamping(linearDamping);
							}
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

	screen_->closeWindow();
}
