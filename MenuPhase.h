#pragma once
#include "Core.h"
#include "Phase.h"

class MenuPhase : public pkPhase
{
public:
    virtual void EnterPhase() override;
    virtual void ExitPhase() override;
    virtual void UpdatePhase() override;
    virtual void DrawPhase() override;
};
