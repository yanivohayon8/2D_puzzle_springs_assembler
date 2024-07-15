#include "piece.h"
#include <iostream>


Piece::Piece(std::string pieceId, std::vector<b2Vec2> &localCoordsAsVecs, std::string imagePath)
{
	id_ = pieceId;
	localCoordsAsVecs_ = localCoordsAsVecs;
	imagePath_ = imagePath;
}

void Piece::DestroyBody()
{
	refb2Body_->GetWorld()->DestroyBody(refb2Body_);
}


void Piece::translate()
{
	for (int i = 0; i < globalCoordinates_.size(); i++)
	{
		globalCoordinates_[i] = refb2Body_->GetWorldPoint(localCoordsAsVecs_.at(i));
	}
}

void Piece::getVeterxGlobalCoords(b2Vec2 &oCoords,int iVertex)
{
	oCoords = globalCoordinates_[iVertex];
}

void Piece::getGlobalCoords(b2Vec2& oCoords,b2Vec2& localCoord)
{
	oCoords = refb2Body_->GetWorldPoint(localCoord);
}

void Piece::computeBoundingBox()
{
	const b2Fixture* fixture = refb2Body_->GetFixtureList();
	const b2Shape* shape = fixture->GetShape();

	for (int childIndex = 0; childIndex < shape->GetChildCount(); ++childIndex)
	{
		b2AABB shapeAABB;
		shape->ComputeAABB(&shapeAABB, refb2Body_->GetTransform(), childIndex);

		if (childIndex == 0)
		{
			aabb_ = shapeAABB;
		}
		else
		{
			aabb_.lowerBound.x = std::min(aabb_.lowerBound.x, shapeAABB.lowerBound.x);
			aabb_.lowerBound.y = std::min(aabb_.lowerBound.y, shapeAABB.lowerBound.y);
			aabb_.upperBound.x = std::max(aabb_.upperBound.x, shapeAABB.upperBound.x);
			aabb_.upperBound.y = std::max(aabb_.upperBound.y, shapeAABB.upperBound.y);
		}
	}
}

float Piece::getBodyBoundingBoxWidth()
{
	return aabb_.upperBound.x - aabb_.lowerBound.x;
}

float Piece::getBodyBoundingBoxHeight()
{
	return std::abs(aabb_.upperBound.y - aabb_.lowerBound.y);
}

void Piece::setCollideOff()
{
	b2Filter filter = refb2Body_->GetFixtureList()->GetFilterData();
	if (filter.groupIndex > 0)
	{
		filter.groupIndex = filter.groupIndex * -1;
	}
	refb2Body_->GetFixtureList()->SetFilterData(filter);
}

void Piece::setCollideOn()
{
	b2Filter filter = refb2Body_->GetFixtureList()->GetFilterData();
	if (filter.groupIndex < 0)
	{
		filter.groupIndex = filter.groupIndex * -1;
	}
	refb2Body_->GetFixtureList()->SetFilterData(filter);
}

void Piece::switchColide()
{
	b2Filter filter = refb2Body_->GetFixtureList()->GetFilterData();
	filter.groupIndex = filter.groupIndex * -1;
	refb2Body_->GetFixtureList()->SetFilterData(filter);
}

void Piece::setLinearDamping(double linearDamping)
{
	refb2Body_->SetLinearDamping(linearDamping);
}

void Piece::setAngularDamping(double angularDamping)
{
	refb2Body_->SetAngularDamping(angularDamping);
}

void Piece::applyLinearImpulse(int powerX,int powerY)
{
	b2Vec2 impulse(powerX, powerY);
	refb2Body_->ApplyLinearImpulseToCenter(impulse, true);
}

float Piece::getBodyRotationRadians()
{
	return refb2Body_->GetAngle();
}

void Piece::getBodyPosition(b2Vec2& position)
{
	position = refb2Body_->GetWorldCenter();
}

void Piece::getBodyWorldCenterPosition(b2Vec2& position)
{
	auto& localCenter = refb2Body_->GetLocalCenter();
	position = refb2Body_->GetWorldPoint(localCenter);
}

void Piece::setIsRotationFixed(bool flag)
{
	isRotationFixed = flag;
}

void Piece::setInitialAngle(float radians)
{
	initialAngle = radians;
}


