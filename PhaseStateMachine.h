#pragma once
#include "Core.h"

class pkPhase;

enum class PhaseIds
{
    FrontEnd,
    Count
};

class PhaseStateMachine
{
public:
    PhaseStateMachine();
    ~PhaseStateMachine();

    void UpdatePhaseStateMachine();
    void DrawPhaseStateMachine();

private:
    pkPhase * currentPhase;
    pkPhase * nextPhase;
    pkPhase * phases[static_cast<int>(PhaseIds::Count)];
};
