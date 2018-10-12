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

// Common already included in the header files above


// TODO remove below
using namespace std;

void LOG(string input)
{
	fstream LogFile;
	LogFile.open("dat.txt", fstream::app);
	if (LogFile.is_open())
	{
		LogFile << input;
		LogFile.close();
	}
}

void LOG(int input)
{
	fstream LogFile;
	LogFile.open("dat.txt", fstream::app);
	if (LogFile.is_open())
	{
		LogFile << char(input);
		LogFile.close();
	}
}
/*
void GatherData(void)
{
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto begin = std::chrono::steady_clock::now();

	std::string fileName = "log_" + std::to_string(now) + ".txt";
	std::ofstream logFile(fileName);

	int keyEntry;
	bool gotZero = false;
	POINT p;

	int lineCount = 0;

	// 10 samples per second means that each cycle takes 100ms
	int samplesPerSecond = 30;
	// Subtract the time taken to run the actual code
	// LOL this is naive but good enough for us
	int runTime = 2;
	int timePerCycleMs = 1000 / samplesPerSecond - runTime;

	while (1)
	{
		// Poll for exit: F10
		if (_kbhit())
		{
			keyEntry = _getch();
			if (keyEntry == 0)
			{
				gotZero = true;
			}

			if (gotZero && keyEntry == 68)
			{
				printf("Got F10; Exiting!\n");
				break;
			}

			printf("Character hit: %d\n", keyEntry);
		}

		// Save the cursor data
		GetCursorPos(&p);
		logFile << p.x << "," << p.y << "\n";
		++lineCount;

		Sleep(timePerCycleMs);
	}

	auto end = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

	auto hours = elapsedTime / 3600;
	elapsedTime %= 3600;
	auto minutes = elapsedTime / 60;
	elapsedTime %= 60;
	auto seconds = elapsedTime;

	logFile << "***\n";
	logFile << "Elapsed time: " << hours << " hours " << minutes << " minutes " << seconds << " seconds\n";
	logFile << lineCount << " measurements gathered with rate of " << samplesPerSecond << " samples per second\n";

	printf("Closing the log file\n");
	logFile.close();
}
*/

void logKey(int button, std::ofstream& logFile)
{
	if (GetKeyState(button) < 0)
	{
		logFile << " " << 1;
	}
	else
	{
		logFile << " " << 0;
	}
}

void CreateLogEntry(std::ofstream& logFile)
{
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	logFile << now << " ";

	POINT p;
	GetCursorPos(&p);
	logFile << p.x << " " << p.y;

	auto keysToTrack = { VK_LBUTTON, VK_RBUTTON, VK_SPACE,
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		49,50,51};
	 
	for (auto key : keysToTrack)
	{
		logKey(key, logFile);
	}

	logFile << "\n";

	/*
	if (GetAsyncKeyState(VK_LBUTTON) == -32767)
	{
		// Left mouse pressed :)
		//printf("The user pressed left button\n");
	}

	if (GetAsyncKeyState(VK_RBUTTON) == -32767)
	{
		// RMB
		//printf("The user pressed right button\n");
	}

	if (GetAsyncKeyState(VK_SPACE) == -32767)
	{
		// space
		//printf("The user pressed spacebar\n");
	}
	*/
}


void doTheUIThing(void)
{
	Logger eventLogger(120, 1);
	Actor actor;

	
	printf("F9: Create a log\nF10: End log\nF11: Replay log");

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
			actor.replayLog("debuglog.txt");

		}
	}

}

void main(void)
{
	
	

	doTheUIThing();
	





	// Prevent the console from closing...
	system("pause");
}





int main2()
{
	// To hide the console...
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto begin = std::chrono::steady_clock::now();

	std::string fileName = "log_" + std::to_string(now) + ".txt";
	std::ofstream logFile(fileName);

	int samplesPerSecond = 60;
	int sleepTimeMs = 1000 / samplesPerSecond;
	int lineCount = 0;

	printf("Beginning logging\n");

	while (true)
	{
		if (GetAsyncKeyState(VK_F10) == -32767)
		{
			printf("Got F10. Exiting...\n");
			break;
		}

		Sleep(sleepTimeMs);
		
		CreateLogEntry(logFile);

		++lineCount;
	}

	auto end = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

	auto hours = elapsedTime / 3600;
	elapsedTime %= 3600;
	auto minutes = elapsedTime / 60;
	elapsedTime %= 60;
	auto seconds = elapsedTime;

	logFile << "***\n";
	logFile << "Elapsed time: " << hours << " hours " << minutes << " minutes " << seconds << " seconds\n";
	logFile << lineCount << " measurements gathered with rate of " << samplesPerSecond << " samples per second\n";

	printf("Closing the log file\n");
	logFile.close();

	return 0;
}