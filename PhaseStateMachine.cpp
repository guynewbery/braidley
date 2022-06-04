#include "PhaseStateMachine.h"

#include "Phase.h"
#include "MenuPhase.h"

PhaseStateMachine::PhaseStateMachine() :
    currentPhase(nullptr),
    nextPhase(nullptr)
{
    phases[static_cast<int>(PhaseIds::FrontEnd)] = new MenuPhase();

    nextPhase = phases[static_cast<int>(PhaseIds::FrontEnd)];
}

PhaseStateMachine::~PhaseStateMachine()
{
    for (int i = 0; i < static_cast<int>(PhaseIds::Count); ++i)
    {
        delete phases[i];
    }
}

void PhaseStateMachine::UpdatePhaseStateMachine()
{
    if (currentPhase != nextPhase)
    {
        if (currentPhase)
        {
            currentPhase->ExitPhase();
        }

        currentPhase = nextPhase;

        if (currentPhase)
        {
            currentPhase->EnterPhase();
        }
    }

    if (currentPhase)
    {
        currentPhase->UpdatePhase();
    }
}

void PhaseStateMachine::DrawPhaseStateMachine()
{
    if (currentPhase)
    {
        currentPhase->DrawPhase();
    }
}