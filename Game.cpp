#include "Game.h"

#include "PhaseStateMachine.h"

static int constexpr screenWidth = 800;
static int constexpr screenHeight = 450;

struct Game
{
    PhaseStateMachine phaseStateMachine;
};

static Game * theGame;

void BeginGame()
{
    theGame = new Game();
    assert(theGame);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
}

void EndGame()
{
    CloseWindow();

    delete theGame;
}

void UpdateGame()
{
    theGame->phaseStateMachine.UpdatePhaseStateMachine();
}

void DrawGame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    theGame->phaseStateMachine.DrawPhaseStateMachine();
    
    EndDrawing();
}

bool GameShouldClose()
{
    return WindowShouldClose(); // Detect window close button or ESC key
}
