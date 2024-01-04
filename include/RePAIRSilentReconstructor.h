#pragma once
#include "SilentReconstructor.h"
class RePAIRSilentReconstructor :
    public SilentReconstructor
{
public:
    std::map<std::string, std::pair<float, b2Vec2>> piece2BeforeCollisionTransformation_;
    RePAIRSilentReconstructor();
    void Run(std::string screenshotPathBeforeCollide = "", std::string screenshotPathAfterCollide = "");
};

