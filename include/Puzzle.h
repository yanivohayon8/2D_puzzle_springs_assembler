#pragma once
#include <Piece.h>
#include <reconstruction.h>
#include <VisualReconstructor.h>
#include <SilentReconstructor.h>
#include <DataLoader.h>
#include <string>

class Puzzle
{
private:
	std::vector<Piece> pieces_;
	std::map<std::string, Piece> id2piece_;
	DataLoader dataLoader_;
	std::vector<VertexMating> trueMatings_;
	Reconstructor* reconstructor_;
	void initReconstruct(std::vector<VertexMating>& matings);
	
public:
	Puzzle(DataLoader& dataLoader, std::string mode="Visual");
	void initPuzzle();
	void reconstruct(std::vector<VertexMating>& matings);
};

