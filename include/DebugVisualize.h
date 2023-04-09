#pragma once
#include <box2d/box2d.h>
#include <opencv2/opencv.hpp>

class cvDebugDraw : public b2Draw
{
public:
	cvDebugDraw(float scale);
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
};