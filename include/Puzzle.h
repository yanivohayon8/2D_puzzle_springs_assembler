#pragma once
#include <Piece.h>
#include <World.h>
#include <DataLoader.h>
#include <string>

class Puzzle
{
private:
	std::vector<Piece> pieces_;
	std::map<std::string, Piece> id2piece_;
	DataLoader dataLoader_;
	std::vector<VertexMating> trueMatings_;
	World simulator_;

public:
	Puzzle(DataLoader& dataLoader);
	void initPuzzle();
	void reconstruct(std::vector<VertexMating>& matings);
};

