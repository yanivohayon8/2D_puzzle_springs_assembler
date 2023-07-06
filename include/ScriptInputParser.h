#pragma once
#include <string>
#include <iostream>
#include <map>

//void parseInput(bool& oIsSimulationAuto, std::string &oPuzzleDirectory, int argc, char** argv);
void parseInput(std::map<std::string,std::string>& oParam2Value, int argc, char** argv);