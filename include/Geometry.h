#pragma once
#include "mapbox/earcut.hpp"
#include <vector>
#include <box2d/box2d.h>
#include <array>
#include <map>

// Function to perform Delaunay triangulation using Earcut
void triangulate(std::vector<std::vector<b2Vec2>>& oTriangles, std::vector<b2Vec2>& polygon);