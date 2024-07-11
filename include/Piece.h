#pragma once
#include <Eigen/Dense>
#include <box2d/box2d.h>
#include <vector>
#include <algorithm>
#include <json.hpp>


class Piece
{
public:
	std::string id_;
	b2Body* refb2Body_;
	std::vector<b2Vec2> localCoordsAsVecs_;
	std::vector<b2Vec2> globalCoordinates_;
	b2Vec2 finalTranslate_;
	b2Rot finalRot_;
	bool isRotationFixed = false;
	float initialAngle = 0;
	std::string imagePath_;
	b2AABB aabb_;
	
	Piece(std::string pieceId, std::vector<b2Vec2>& localCoordsAsVecs, std::string imagePath);

	void DestroyBody();

	void translate();
	void getVeterxGlobalCoords(b2Vec2& oCoords, int iVertex);
	void getGlobalCoords(b2Vec2& oCoords, b2Vec2& localCoord);

	void computeBoundingBox();
	float getBodyBoundingBoxWidth();
	float getBodyBoundingBoxHeight();

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

	void getBodyWorldCenterPosition(b2Vec2& position);
};

