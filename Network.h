#pragma once


//https://gafferongames.com/categories/game-networking/
//https://gafferongames.com/categories/building-a-game-network-protocol/
//https://stackoverflow.com/questions/15435994/how-do-i-open-an-exe-from-another-c-exe

bool InitializeSockets();
void ShutdownSockets();
bool TestConnection();
