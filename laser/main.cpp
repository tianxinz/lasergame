#include "game.h"
#include<iostream>
#include <windows.h>






int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Game game;
	game.run();
	return 0;
}
