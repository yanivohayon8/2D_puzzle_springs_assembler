#pragma once
#include <Eigen/Dense>
#include <box2d/box2d.h>
#include <vector>
#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<float> BoostPoint;
typedef bg::model::polygon<BoostPoint> BoostPolygon;


class Piece
{
public:
	std::string id_;
	b2Body* refb2Body_;
	Eigen::MatrixX2d localCoordinates_;
	Eigen::MatrixX2d finalCoordinates_;
	std::vector<b2Vec2> localCoordsAsVecs_;
	std::vector<b2Vec2> globalCoordinates_;
	b2Vec2 finalTranslate_;
	b2Rot finalRot_;
	bool isRotationFixed = false;
	float initialAngle = 0;
	std::string imagePath_;
	b2AABB aabb_;
	BoostPolygon boostPolygonGlobalCoords_;
	float boostPolygonArea_;

	Piece(std::string pieceId,Eigen::MatrixX2d coordinates, std::string imagePath);

	void DestroyBody();

	void printCoords();
	std::pair<double, double> getVertexCoord(int iVertex);
	int getNumCoords();
	void rotate(const b2Rot& rot);
	void translate();
	std::pair<int, int> getEdgeVertexIndexes(int iEdge);
	b2Vec2* getVeterxLocalCoords(int iVertex);
	void getVeterxGlobalCoords(b2Vec2& oCoords, int iVertex);
	void sortVerticesCCW(Eigen::MatrixX2d& coords, std::vector<int>& index_map);
	void getGlobalCoordsMoved(Eigen::MatrixX2d &oCoords, b2Vec2 translate);
	void getVertexGlobalCoordsAsEigen(Eigen::MatrixX2d& oCoords);

	void computeBoundingBox();
	float getBodyBoundingBoxWidth();
	float getBodyBoundingBoxHeight();

	void initBoostPolygon();
	float computeOverlappingArea(const BoostPolygon& otherPolyon);
	float computeArea();

	void setCollideOff();
	void setCollideOn();
	void switchColide();
	void setLinearDamping(double linearDamping);
	void setAngularDamping(double angularDamping);
	void applyLinearImpulse(int powerX, int powerY);

	float getBodyRotationRadians();
	void getBodyPosition(b2Vec2& position);

	void setIsRotationFixed(bool flag);// effective only before reconstructor.init()...
	void setInitialAngle(float radians);
};

