//#include <DebugVisualize.h>
//
//cvDebugDraw::cvDebugDraw() : m_image(1, 1, CV_8UC3) {}
//
//void cvDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
//{
//
//void cvDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
//    cv::Scalar cvColor(color.r * 255, color.g * 255, color.b * 255);
//    //cv::Point points[vertexCount];
//    std::vector<cv::Point> points;
//    for (int i = 0; i < vertexCount; i++) {
//        points.push_back(cv::Point(vertices[i].x, vertices[i].y));
//    }
//
//    cv::fillPoly(m_image, points, cvColor);
//}
//
////cv::Point cvDebugDraw::ConvertWorldToScreen(const b2Vec2& worldPt) const
////{
////    float x = (worldPt.x - m_xOffset) * m_xScale;
////    float y = (m_height - (worldPt.y - m_yOffset)) * m_yScale;
////    return cv::Point(static_cast<int>(x), static_cast<int>(y));
////}
////
////
////void cvDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
////{
////    // Convert Box2D world coordinates to OpenCV image coordinates
////    cv::Point pt1 = ConvertWorldToScreen(p1);
////    cv::Point pt2 = ConvertWorldToScreen(p2);
////
////    // Draw the line on the OpenCV image
////    cv::line(m_image, pt1, pt2, cv::Scalar(color.r * 255, color.g * 255, color.b * 255), m_lineThickness);
////}
////
////void cvDebugDraw::DrawTransform(const b2Transform& xf)
////{
////    float k_axisScale = 0.4f;
////    b2Color red(1.0f, 0.0f, 0.0f);
////    b2Color green(0.0f, 1.0f, 0.0f);
////    b2Vec2 p1 = xf.p, p2;
////    p2 = p1 + k_axisScale * xf.q.GetXAxis();
////    DrawSegment(p1, p2, red);
////    p2 = p1 + k_axisScale * xf.q.GetYAxis();
////    DrawSegment(p1, p2, green);
////}
//
//
//
