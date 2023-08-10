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

		/*for (auto& mating : activeMatings_)
		{
			auto& anchorA = mating.jointRef_->GetAnchorA();
			auto& anchorB = mating.jointRef_->GetAnchorB();
			screen_->drawLine(anchorA, anchorB, redColor, -1);
		}*/

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
							/*int power = sampleIntUniformly(5, -5, -1);
							piece.applyLinearImpulse(power, power);*/
							
							piece.applyLinearImpulse(10, 10);
						}
						break;
					case sf::Keyboard::W:
						std::cout << "Exploding" << std::endl;
						for (auto& piece : activePieces_)
						{
							/*int power = sampleIntUniformly(5, -5, -1);
							piece.applyLinearImpulse(power, power);*/

							piece.applyLinearImpulse(-100, -100);
						}
						break;
					case sf::Keyboard::R:
						std::cout << "Increase damping" << std::endl;
						linearDamping += 0.1;
						for (auto& piece : activePieces_)
						{
							piece.setLinearDamping(linearDamping);
						}
						break;
					case sf::Keyboard::T:
						std::cout << "Decrease damping" << std::endl;
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
					case sf::Keyboard::F:
						// For tests...
						for (auto& piece: activePieces_)
						{
							auto velocity = piece.refb2Body_->GetLinearVelocity();
							std::string strVelocity = "(" + std::to_string(velocity.x) + "," + std::to_string(velocity.y) + ")";
							std::cout << "Velocity of piece " << piece.id_ << " is " + strVelocity << std::endl;
						}

						for (auto& mating : activeMatings_)
						{

							float inverseTimeStep = 1/timeStep_;
							auto force = mating.jointRef_->GetReactionForce(inverseTimeStep);
							std::string strForce = "(" + std::to_string(force.x) + "," + std::to_string(force.y) + ")";
							std::cout << "Force of mating is " + strForce << std::endl;

							auto torque = mating.jointRef_->GetReactionTorque(inverseTimeStep);
							std::cout << "Torque of mating is " + std::to_string(torque) << std::endl;
							
							std::cout << "piece 1 " << mating.firstPieceId_ << " piece2 " << mating.secondPieceId_ << std::endl;
							std::cout << "AnchorA " << std::to_string(mating.jointRef_->GetAnchorA().x) << "," << std::to_string(mating.jointRef_->GetAnchorA().y) << std::endl;
							std::cout << "AnchorB " << std::to_string(mating.jointRef_->GetAnchorB().x) << "," << std::to_string(mating.jointRef_->GetAnchorB().y) << std::endl;
							
							activePieces_[1].setAngularDamping(0.15);
							activePieces_[1].refb2Body_->ApplyLinearImpulse(5 * force, mating.jointRef_->GetAnchorB(), true);

							//activePieces_[1].refb2Body_->ApplyLinearImpulseToCenter(10 * force, true);
							
							world_.DestroyJoint(mating.jointRef_);
						}

						/*auto hardcodedForce = b2Vec2(0.029518, -0.169097);
						activePieces_[1].refb2Body_->ApplyLinearImpulseToCenter(-hardcodedForce, true);
						activePieces_[0].refb2Body_->ApplyLinearImpulseToCenter(hardcodedForce, true);*/
						
						
						

						break;
					case sf::Keyboard::S:
						//std::cout << "Extend the springs length in a few cm" << std::endl;

						for (auto& mating : activeMatings_)
						{
							float inverseTimeStep = 1 / timeStep_;
							auto force = mating.jointRef_->GetReactionForce(inverseTimeStep);

							b2Vec2 perperndicular(-force.y*5, -force.x*5);

							activePieces_[1].refb2Body_->ApplyLinearImpulse(perperndicular, mating.jointRef_->GetAnchorB(), true);
							//activePieces_[1].refb2Body_->ApplyLinearImpulseToCenter(perperndicular,true);*/

							//float length = mating.jointRef_->GetCurrentLength();
							//mating.jointRef_->SetLength(length + 0.005);
							
							/*float stif, damp;
							b2LinearStiffness(stif, damp, 0.6f, 0.05f, mating.jointRef_->GetBodyA(), mating.jointRef_->GetBodyB());
							mating.jointRef_->SetStiffness(stif);
							mating.jointRef_->SetDamping(damp);
							*/
							
						}
						break;
					case sf::Keyboard::A:
						std::cout << "Short the springs length in a few cm" << std::endl;

						for (auto& mating : activeMatings_)
						{
							//float length = mating.jointRef_->GetCurrentLength();
							//mating.jointRef_->SetLength(length - 0.005);

							float maxLength = mating.jointRef_->GetMaxLength();
							mating.jointRef_->SetMaxLength(maxLength - 0.1);

							/*float stif, damp;
							b2LinearStiffness(stif, damp, 0.6f, 0.05f, mating.jointRef_->GetBodyA(), mating.jointRef_->GetBodyB());
							mating.jointRef_->SetStiffness(stif);
							mating.jointRef_->SetDamping(damp);
							*/

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
