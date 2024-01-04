#include "Piece.h"
#include <iostream>

Piece::Piece(std::string pieceId, Eigen::MatrixX2d coordinates, std::string imagePath)
{
	id_ = pieceId;
	localCoordinates_ = coordinates;

	for (int i = 0; i < coordinates.rows(); i++)
	{
		float x_ = static_cast<float>(coordinates.coeff(i, 0));
		float y_ = static_cast<float>(coordinates.coeff(i, 1));
		localCoordsAsVecs_.push_back(b2Vec2{ x_,y_ });
	}

	imagePath_ = imagePath;
	boostPolygonArea_ = 0;
}

void Piece::DestroyBody()
{
	refb2Body_->GetWorld()->DestroyBody(refb2Body_);
}

void Piece::printCoords()
{
	for (int ii = 0; ii < localCoordinates_.rows(); ii++)
	{
		std::cout << "(" << localCoordinates_.coeff(ii, 0) << "," << localCoordinates_(ii, 1) << ")" << std::endl;
	}
}

std::pair<double, double> Piece::getVertexCoord(int iVertex)
{
	std::pair<double, double> xy;
	xy.first = localCoordinates_.coeff(iVertex, 0);
	xy.second = localCoordinates_.coeff(iVertex, 1);
	return xy;
}

int Piece::getNumCoords()
{
	return localCoordinates_.rows();
}

void Piece::rotate(const b2Rot& rot)
{
	// This is inspired from pelegs code: check it! (remember in Passover about the traslate method)
	Eigen::Matrix2d rotationEigen;
	rotationEigen << rot.c, -rot.s, rot.s, rot.c;
	localCoordinates_ = localCoordinates_ * rotationEigen.transpose();
}


void Piece::translate()
{
	for (int i = 0; i < globalCoordinates_.size(); i++)
	{
		globalCoordinates_[i] = refb2Body_->GetWorldPoint(localCoordsAsVecs_.at(i));
	}
}

std::pair<int, int> Piece::getEdgeVertexIndexes(int iEdge)
{
	int indexFirstVertex = iEdge;
	int indexSecondVertex = (iEdge + 1) % getNumCoords();
	return std::pair<int, int>(indexFirstVertex, indexSecondVertex);
}

b2Vec2* Piece::getVeterxLocalCoords(int iVertex)
{
	return &localCoordsAsVecs_[iVertex];
}

void Piece::getVeterxGlobalCoords(b2Vec2 &oCoords,int iVertex)
{
	oCoords = globalCoordinates_[iVertex];
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

void Piece::getGlobalCoordsMoved(Eigen::MatrixX2d& oCoords, b2Vec2 translate)
{
	for (int i = 0; i < oCoords.rows(); i++)
	{
		/*oCoords(i, 0) = globalCoordinates_[i].x - translate.x;
		oCoords(i, 1) = globalCoordinates_[i].y - translate.y;*/
		oCoords(i, 0) = oCoords(i,0) - translate.x;
		oCoords(i, 1) = oCoords(i, 1) - translate.y;
	}
}

void Piece::getVertexGlobalCoordsAsEigen(Eigen::MatrixX2d& oCoords)
{
	oCoords.resizeLike(localCoordinates_);
	for (int coordIndex = 0; coordIndex < oCoords.rows(); coordIndex++)
	{
		oCoords(coordIndex, 0) = globalCoordinates_.at(coordIndex).x;
		oCoords(coordIndex, 1) = globalCoordinates_.at(coordIndex).y;
	}
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


void Piece::initBoostPolygon()
{

	std::vector<BoostPoint> points;

	for (auto& point:globalCoordinates_)
	{
		BoostPoint boostPoint(point.x, point.y);
		points.push_back(boostPoint);
	}

	bg::assign_points(boostPolygonGlobalCoords_, points);
}

float Piece::computeOverlappingArea(const BoostPolygon& otherPolyon)
{
	std::vector<BoostPolygon> output;
	bg::intersection(boostPolygonGlobalCoords_, otherPolyon, output);

	float area = 0.0f;
	for (const auto& p : output)
	{
		area += bg::area(p);
	}

	return area;
}

float Piece::computeArea()
{
	if (boostPolygonArea_ == 0)
	{
		boostPolygonArea_ = bg::area(boostPolygonGlobalCoords_);
	}
	return boostPolygonArea_;
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


