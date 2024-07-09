#include "Piece.h"
#include <iostream>

Piece::Piece(std::string pieceId, Eigen::MatrixX2d coordinates, std::string imagePath)
{
	id_ = pieceId;

	for (int i = 0; i < coordinates.rows(); i++)
	{
		float x_ = static_cast<float>(coordinates.coeff(i, 0));
		float y_ = static_cast<float>(coordinates.coeff(i, 1));
		localCoordsAsVecs_.push_back(b2Vec2{ x_,y_ });
	}

	imagePath_ = imagePath;
}

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

void Piece::sortVerticesCCW(Eigen::MatrixX2d& coords, std::vector<int>& index_map)
{
	// Compute centroid
	Eigen::Vector2d centroid(0, 0);
	for (int i = 0; i < coords.rows(); i++) {
		centroid += coords.row(i);
	}
	centroid /= coords.rows();

	// Compute angles with respect to centroid
	std::vector<std::pair<double, int>> angles;
	for (int i = 0; i < coords.rows(); i++) {
		double x = coords(i, 0) - centroid(0);
		double y = coords(i, 1) - centroid(1);
		double angle = atan2(y, x);
		angles.push_back(std::make_pair(angle, i));
	}

	// Sort angles
	std::sort(angles.begin(), angles.end());

	// Reorder coordinates and index map
	Eigen::MatrixX2d sorted_coords(coords.rows(), coords.cols());
	std::vector<int> sorted_index_map(coords.rows());
	for (int i = 0; i < coords.rows(); i++) {
		sorted_coords.row(i) = coords.row(angles[i].second);
		sorted_index_map[angles[i].second] = i;
	}

	coords = sorted_coords;
	index_map = sorted_index_map;
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


