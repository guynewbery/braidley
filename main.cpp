#include "Core.h"
#include "Game.h"

int main(void)
{
    BeginGame();

    while (!GameShouldClose())    
    {
        UpdateGame();
        DrawGame();
    }

    EndGame();

    return 0;
}
