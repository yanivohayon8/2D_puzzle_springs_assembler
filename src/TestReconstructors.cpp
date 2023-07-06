#include "TestReconstructors.h"

TestReconstructors::TestReconstructors(){}

void TestReconstructors::testVisualGroundTruth(std::string puzzleDirectory)
{
	DataLoader dataLoader(puzzleDirectory);
	std::vector<VertexMating> trueMatings;
	dataLoader.loadVertexMatings(trueMatings, "springs_anchors_correct.csv");
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces);

	VisualReconstructor vsReconstructor;
	vsReconstructor.init();

	vsReconstructor.initRun(pieces,trueMatings);
	vsReconstructor.Run();
	vsReconstructor.closeRun();
}

void TestReconstructors::testSilentGroundTruth(std::string puzzleDirectory)
{
	DataLoader dataLoader(puzzleDirectory);
	std::vector<VertexMating> trueMatings;
	dataLoader.loadVertexMatings(trueMatings, "springs_anchors_correct.csv");
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces);

	SilentReconstructor vsReconstructor;
	vsReconstructor.init();

	vsReconstructor.initRun(pieces, trueMatings);
	vsReconstructor.Run(puzzleDirectory+"/silent_assembly.png");
	vsReconstructor.closeRun();
}
