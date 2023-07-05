#pragma once
#include <World.h>
#include <Piece.h>


class Reconstruction
{
private:
	//World* parentWorld_;
	std::vector<Piece> pieces_;
	std::vector<VertexMating> matings_;

public:
	Reconstruction(std::vector<Piece>& pieces, std::vector<VertexMating>& matings);
	void close(); // delete the matings and pieces...
};

