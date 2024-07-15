#pragma once
#include <random>
#include <iostream>
#include <Windows.h>
#include <box2d/box2d.h>

void generate2DVectors(std::vector<b2Vec2>& vectors, int n, int maxX, int maxY, int padding, int seed);

int sampleIntUniformly(double max, double min, int seed);

void getScreenSize(int& oHeight, int& oWidth);