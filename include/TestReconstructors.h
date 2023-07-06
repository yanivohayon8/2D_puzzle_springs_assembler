#pragma once
#include <VertexMating.h>
#include <Piece.h>
#include <VisualReconstructor.h>
#include <SilentReconstructor.h>
#include <DataLoader.h>

class TestReconstructors
{
public:
	TestReconstructors();
	void testVisualGroundTruth(std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0");
	void testVisualTwoReconstructs(std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0");
	void testSilentGroundTruth(std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0");
};
