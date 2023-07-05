#include "reconstruction.h"

Reconstruction::Reconstruction(std::vector<Piece>& pieces, std::vector<VertexMating>& matings)
{
	pieces_ = pieces;
	matings_ = matings;
}