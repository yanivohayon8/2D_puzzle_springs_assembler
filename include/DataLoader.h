#pragma once
#include <string>
#include "Piece.h"
#include "EdgeMating.h"
#include <iostream>
#include <fstream>

class DataLoader
{
public:
	std::string puzzleDirectoryPath_;

	DataLoader(std::string puzzleDirectoryPath);

	void loadPieces(std::vector<Piece> &olstPiece);
	void loadEdgeMatings(std::vector<EdgeMating>& olstMatings);
};

