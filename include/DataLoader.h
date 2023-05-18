#pragma once
#include <string>
#include "Piece.h"
#include "EdgeMating.h"
#include <iostream>
#include <fstream>
#include "VertexMating.h"

class DataLoader
{
public:
	std::string puzzleDirectoryPath_;

	DataLoader(std::string puzzleDirectoryPath);

	void loadPieces(std::vector<Piece> &olstPiece,bool isOfir=false);
	void loadEdgeMatings(std::vector<EdgeMating>& olstMatings);
	void coordsToEigenCoords(Eigen::MatrixXd &eigenCoords,std::vector<std::pair<double, double>>& coords);
	
	void loadVertexMatings(std::vector<VertexMating>& olstMatings);//, const std::vector<Piece>& pieces
	void loadExtraInfo(std::vector<Piece>& olstPiece);
};

