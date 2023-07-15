#include <TestHandler.h>

void RouteTests(std::map<std::string, std::string>& param2Value)
{
	std::string testName = param2Value["test"];

	// --test VisualGroundTruth --puzzleDir "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0"
	if (testName == "VisualGroundTruth")
	{
		TestReconstructors tester;
		if (param2Value.count("puzzleDir"))
		{
			tester.testVisualGroundTruth(param2Value["puzzleDir"]);
		}
		else
		{
			tester.testVisualGroundTruth(); //Default Puzzle
		}
	}

	else if (testName == "VisualTwoReconstructs")
	{
		TestReconstructors tester;
		tester.testVisualTwoReconstructs(); //param2Value["puzzleDir"]
	}

	// --test SilentGroundTruth --puzzleDir "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0"
	else if (testName == "SilentGroundTruth")
	{
		TestReconstructors tester;
		if (param2Value.count("puzzleDir"))
		{
			tester.testSilentGroundTruth(param2Value["puzzleDir"]);
		}
		else
		{
			tester.testSilentGroundTruth(); //Default Puzzle
		}
	}
}