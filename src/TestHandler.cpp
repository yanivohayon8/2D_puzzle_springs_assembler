#include <TestHandler.h>

void RouteTests(std::map<std::string, std::string>& param2Value)
{
	std::string testName = param2Value["test"];

	// --test VisualGroundTruth --puzzleDir "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0"
	if (testName == "VisualGroundTruth")
	{
		TestReconstructors tester;
		tester.testVisualGroundTruth(); //param2Value["puzzleDir"]
	}

	if (testName == "VisualTwoReconstructs")
	{
		TestReconstructors tester;
		tester.testVisualTwoReconstructs(); //param2Value["puzzleDir"]
	}

	// --test SilentGroundTruth --puzzleDir "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0"
	if (testName == "SilentGroundTruth")
	{
		TestReconstructors tester;
		tester.testSilentGroundTruth(param2Value["puzzleDir"]);
	}
}