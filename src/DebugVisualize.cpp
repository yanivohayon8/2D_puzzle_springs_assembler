DebugDraw(float32 scale) : m_scale(scale) {}
void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    cv::Point points[vertexCount];
    for (int32 i = 0; i < vertexCount; ++i) {
        points[i] = cv::Point(vertices[i].x * m_scale, vertices[i].y * m_scale);
    }
    const cv::Point* ppt[1] = { points };
    int npt[] = { vertexCount };
    cv::polylines(m_image, ppt, npt, 1, true, cv::Scalar(color.r * 255, color.g * 255, color.b * 255), 2);
}
void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    cv::Point points[vertexCount];
    for (int32 i = 0; i < vertexCount; ++i) {
        points[i] = cv::Point(vertices[i].x * m_scale, vertices[i].y * m_scale);
    }
    const cv::Point* ppt[1] = { points };
    int npt[] = { vertexCount };
    cv::fillPoly(m_image, ppt, npt, 1, cv::Scalar(color.r * 255, color.g * 255, color.b * 255));
}