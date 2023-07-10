#include "TestReconstructors.h"

TestReconstructors::TestReconstructors(){}

void TestReconstructors::testVisualGroundTruth(std::string puzzleDirectory)
{
	DataLoader dataLoader(puzzleDirectory);
	std::vector<VertexMating> trueMatings;
	dataLoader.loadVertexMatings(trueMatings, "springs_anchors_correct.csv"); //"springs_anchors_correct.csv");
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces);

	VisualReconstructor vsReconstructor;
	vsReconstructor.init();

	vsReconstructor.initRun(pieces,trueMatings);
	vsReconstructor.Run();
	vsReconstructor.closeRun();
}

void TestReconstructors::testVisualTwoReconstructs(std::string puzzleDirectory)
{
	DataLoader dataLoader(puzzleDirectory);
	std::vector<VertexMating> trueMatings;
	dataLoader.loadVertexMatings(trueMatings, "springs_anchors_correct.csv");
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces);

	VisualReconstructor vsReconstructor;
	vsReconstructor.init();

	// First run
	vsReconstructor.initRun(pieces, trueMatings);
	vsReconstructor.Run();
	vsReconstructor.closeRun();

	// Second run with missing springs
	std::vector<VertexMating> matings;
	std::vector<Piece> filteredPieces;

	/// Hard coded the springs_anchors_missing_springs.csv file
	// does not has the springs for pieces 7,8,9
	for (int i = 0; i < 7; i++)
	{
		filteredPieces.push_back(pieces[i]);
	}

	dataLoader.loadVertexMatings(matings, "springs_anchors_missing_springs.csv");
	vsReconstructor.initRun(filteredPieces, matings);
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
