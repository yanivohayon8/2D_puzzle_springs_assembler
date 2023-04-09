//#pragma once
//#include <Box2D/Box2D.h>
//#include <opencv2/opencv.hpp>
//
//class cvDebugDraw : public b2Draw {
//public:
//    cvDebugDraw();
//
//    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
//
//    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
//
//    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
//
//    //void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
//
//    //void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
//
//    //void DrawTransform(const b2Transform& xf) override;
//
//    ////void SetViewport(const cv::Rect& viewport);
//    //cv::Point cvDebugDraw::ConvertWorldToScreen(const b2Vec2& worldPt) const;
//
//private:
//    cv::Mat m_image;
//    cv::Rect m_viewport;
//    float m_pixelsPerMeter = 32.f;
//    float m_thickness = 1.f;
//};
