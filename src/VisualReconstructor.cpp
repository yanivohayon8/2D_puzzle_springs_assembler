#include "VisualReconstructor.h"



VisualReconstructor::VisualReconstructor(float boardWidth, float boardHeight, int screenWidth, int screenHeight):Reconstructor(boardWidth,boardHeight,screenWidth,screenHeight)
{
}

void VisualReconstructor::Run(std::string screenshotPathBeforeCollide, std::string screenshotPathAfterCollide)
{
	
	screen_->initDisplay();
	screen_->initBounds(boundsCoordinates_);
	auto redColor = sf::Color::Red;
	float linearDamping = 0;
	bool draw_joints = true;
	bool is_recreated_joints = false;

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

		for (auto& mating : activeMatings_)
		{
			if (draw_joints)
			{
				auto& anchorA = mating->jointRef_->GetAnchorA();
				auto& anchorB = mating->jointRef_->GetAnchorB();
				screen_->drawLine(anchorA, anchorB, redColor, -1);
			}
			
		}

		screen_->updateDisplay();

		// pressed F
		if (is_recreated_joints)
		{
			for (b2Contact* contact = world_.GetContactList(); contact; contact = contact->GetNext())
			{
				std::cout << "collide" << contact->GetFixtureA() << std::endl;
				/*for (auto& mating : activeMatings_)
				{
					
					draw_joints = false;
					world_.DestroyJoint(mating.jointRef_);
				}*/
			}
			
			for (auto& mating : activeMatings_)
			{
				auto bodyA = mating->jointRef_->GetBodyA();
				auto bodyB = mating->jointRef_->GetBodyB();

				for (b2Fixture* fixtureA = bodyA->GetFixtureList(); fixtureA; fixtureA = fixtureA->GetNext()) {
					for (b2Fixture* fixtureB = bodyB->GetFixtureList(); fixtureB; fixtureB = fixtureB->GetNext()) {
						if (b2TestOverlap(fixtureA->GetAABB(0), fixtureB->GetAABB(0))) {


							float inverseTimeStep = 1 / timeStep_;
							auto force = mating->jointRef_->GetReactionForce(inverseTimeStep);
							activePieces_[1].refb2Body_->ApplyForce(0.0001*force, mating->jointRef_->GetAnchorB(), true);
							fixtureB->SetRestitution(0);
							fixtureA->SetRestitution(0);
							
							/*for (auto& piece : activePieces_)
							{
								piece.setLinearDamping(2);
							}*/
							//activePieces_[1].refb2Body_->ApplyLinearImpulseToCenter(10 * force, true);

							draw_joints = false;
							world_.DestroyJoint(mating->jointRef_);
							setPiecesCollisionOn();
							is_recreated_joints = false; // to prevent from enter here again
						}
					}
				}
			}


		}

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
							auto force = mating->jointRef_->GetReactionForce(inverseTimeStep);
							std::string strForce = "(" + std::to_string(force.x) + "," + std::to_string(force.y) + ")";
							std::cout << "Force of mating is " + strForce << std::endl;

							auto torque = mating->jointRef_->GetReactionTorque(inverseTimeStep);
							std::cout << "Torque of mating is " + std::to_string(torque) << std::endl;
							
							std::cout << "piece 1 " << mating->firstPieceId_ << " piece2 " << mating->secondPieceId_ << std::endl;
							std::cout << "AnchorA " << std::to_string(mating->jointRef_->GetAnchorA().x) << "," << std::to_string(mating->jointRef_->GetAnchorA().y) << std::endl;
							std::cout << "AnchorB " << std::to_string(mating->jointRef_->GetAnchorB().x) << "," << std::to_string(mating->jointRef_->GetAnchorB().y) << std::endl;
							
							//activePieces_[1].setAngularDamping(0.15);
							//activePieces_[1].refb2Body_->ApplyLinearImpulse(2*force, mating.jointRef_->GetAnchorB(), true);

							//activePieces_[1].refb2Body_->ApplyLinearImpulseToCenter(10 * force, true);
							
							//world_.DestroyJoint(mating.jointRef_);


							world_.DestroyJoint(mating->jointRef_);
							disableJointsCollide();
							setJointRestLength(0.05);
							setJointFrequency(jointFrequencyHertz_ * 1.5);
							setJointDamping(jointDampingRatio_ * 2);
							putMatingSprings(mating);
							//mating.jointRef_->GetBodyA()->GetFixtureList()->SetRestitution(0);
							//mating.jointRef_->GetBodyB()->GetFixtureList()->SetRestitution(0);


						}

						is_recreated_joints = true;
						setPiecesCollisionOn();
						
						

						break;
					case sf::Keyboard::S:
						//std::cout << "Extend the springs length in a few cm" << std::endl;

						for (auto& mating : activeMatings_)
						{
							float inverseTimeStep = 1 / timeStep_;
							auto force = mating->jointRef_->GetReactionForce(inverseTimeStep);

							b2Vec2 perperndicular(-force.y*5, -force.x*5);

							activePieces_[1].refb2Body_->ApplyLinearImpulse(perperndicular, mating->jointRef_->GetAnchorB(), true);
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

							float maxLength = mating->jointRef_->GetMaxLength();
							mating->jointRef_->SetMaxLength(maxLength - 0.1);

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




nlohmann::json VisualReconstructor::reconstruct()
{
	nlohmann::json output;

	return output;
}