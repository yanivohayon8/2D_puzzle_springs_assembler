#include "Geometry.h"
//#include "mapbox/earcut.hpp"
//using Coord = double;
//using N = uint32_t;
//using Point = std::array<Coord, 2>;



std::map<int, std::vector<b2Vec2>> triangulate(std::vector<b2Vec2>& polygon)
{
    
    std::vector<std::array<double, 2>> array_;

    for (const auto& v : polygon) {
        std::array<double, 2> cord{ {v.x,v.y} };
        array_.push_back(cord);
    }

    std::vector< std::vector<std::array<double, 2>>> tfira_container;
    tfira_container.push_back(array_);
    std::vector<uint32_t> triangleIndices = mapbox::earcut(tfira_container);
    
    std::map<int,std::vector<b2Vec2>> triangles;

    for (int i=0; i< polygon.size(); i++)
    {
        auto triangleIndex = triangleIndices[i];
        if (triangles.find(triangleIndex)==triangles.end())
        {
            std::vector<b2Vec2> emptyTriangle;
            triangles.insert({ triangleIndex,emptyTriangle });
        }

        triangles.at(triangleIndex).push_back(polygon[i]);
    }

    return triangles;
}