#pragma once
#include <reconstruction.h>

class SilentReconstructor: public Reconstructor
{
public:
	void Run(std::string resultScreenshotPath="");
};

