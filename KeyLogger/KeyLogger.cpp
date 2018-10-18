// KeyLogger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
//#include <iostream>
#include <chrono>

#include "Logger.hpp"
#include "Actor.hpp"

void doTheUIThing(void)
{
	Logger eventLogger(120, 15);
	Actor actor;

	printf("$-$-$ \tWelcome to AurumBot\t $-$-$\n\n");

	printf("F9: Create a log\nF10: End log\nF11: Replay log\nF12: Quit\n");

	while (true)
	{
		// Create a log
		if (GetAsyncKeyState(VK_F9) == -32767)
		{
			printf("\n");
			eventLogger.createLog();
		}

		// Replay log
		if (GetAsyncKeyState(VK_F11) == -32767)
		{
			printf("\n");
			actor.replayFromFile("debuglog.txt");
		}
	}
}

void main(void)
{
	doTheUIThing();

	// Prevent the console from closing...
	system("pause");
}