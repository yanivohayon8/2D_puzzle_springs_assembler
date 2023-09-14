#pragma once
#include "SilentReconstructor.h"
class RePAIRSilentReconstructor :
    public SilentReconstructor
{
public:
    RePAIRSilentReconstructor();
    void Run(std::string screenshotPathBeforeCollide = "", std::string screenshotPathAfterCollide = "");
};

