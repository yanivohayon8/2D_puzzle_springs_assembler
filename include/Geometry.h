#pragma once
#include "mapbox/earcut.hpp"
#include <vector>
#include <box2d/box2d.h>
#include <array>
#include <map>

// Function to perform Delaunay triangulation using Earcut
std::map<int, std::vector<b2Vec2>> triangulate(std::vector<b2Vec2> &polygon);