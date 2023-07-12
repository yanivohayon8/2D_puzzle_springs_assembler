#pragma once
#include <reconstruction.h>

class SilentReconstructor: public Reconstructor
{
private:
	void progress(int numIteration);

public:
	void Run(std::string resultScreenshotPath="");
};

