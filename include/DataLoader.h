#pragma once
#include <string>
#include "Piece.h"
#include "EdgeMating.h"
#include <iostream>
#include <fstream>
#include "VertexMating.h"
#include <sstream>
#include "Global_Parameters.h"


class DataLoader
{
public:
	std::string puzzleDirectoryPath_;

	DataLoader(std::string puzzleDirectoryPath);
	void loadPieces(std::vector<Piece> &olstPiece,bool isOfir = true);
	//void loadEdgeMatings(std::vector<EdgeMating>& olstMatings);
	void coordsToEigenCoords(Eigen::MatrixXd &eigenCoords,std::vector<std::pair<double, double>>& coords);
	
	void loadVertexMatings(std::vector<VertexMating>& olstMatings);//, const std::vector<Piece>& pieces
	void loadExtraInfo(std::vector<Piece>& olstPiece);

private:
	void loadCoordinates_(std::string fileName, bool isOfir = true);
	std::string getImagePath(std::string pieceId);
};

