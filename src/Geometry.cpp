#include "Geometry.h"
//#include "mapbox/earcut.hpp"
//using Coord = double;
//using N = uint32_t;
//using Point = std::array<Coord, 2>;



void triangulate(std::vector<std::vector<b2Vec2>>&oTriangles,std::vector<b2Vec2>& polygon)
{
    
    std::vector<std::array<double, 2>> array_;

    for (const auto& v : polygon) {
        std::array<double, 2> cord{ {v.x,v.y} };
        array_.push_back(cord);
    }

    std::vector< std::vector<std::array<double, 2>>> tfira_container;
    tfira_container.push_back(array_);
    std::vector<uint32_t> triangleIndices = mapbox::earcut(tfira_container);
    
    //std::map<int,std::vector<b2Vec2>> triangles;

    // According to the documentation:
    // Returns array of indices that refer to the vertices of the input polygon.
    // Three subsequent indices form a triangle. Output triangles are clockwise.
    for (int i=0; i< triangleIndices.size(); i+=3)
    {
        std::vector<b2Vec2> emptyTriangle;
        emptyTriangle.push_back(polygon[triangleIndices[i]]);
        emptyTriangle.push_back(polygon[triangleIndices[i + 1]]);
        emptyTriangle.push_back(polygon[triangleIndices[i + 2]]);
       
        oTriangles.push_back(emptyTriangle);
    }
}