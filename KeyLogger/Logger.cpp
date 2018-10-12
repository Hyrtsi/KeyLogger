#include "stdafx.h"
#include "Logger.hpp"

Logger::Logger(int samplesPerSecond = 60, int maxTimeToLogMinutes = 60)
{
	_samplesPerSecond = samplesPerSecond;
	_maxTimeToLogMinutes = maxTimeToLogMinutes;

	auto keys = { VK_LBUTTON, VK_RBUTTON, VK_SPACE,
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		49,50,51 };

	for (auto key : keys)
	{
		Key keyElem = { key, false };
		_keysToTrack.push_back(keyElem);
	}
}

void Logger::createLog(void)
{
	auto currentUnixTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto beginTime = std::chrono::steady_clock::now();

	std::string fileName = "log_" + std::to_string(currentUnixTime) + ".txt";
	std::ofstream logFile(fileName);

	int sleepTimeMs = (int)(1000.f / (float)_samplesPerSecond);
	int lineCount = 0;
	
	bool anyKeyChanged = false;

	printf("Beginning logging to file %s\n",
		fileName.c_str());

	//auto lastTimeStamp = std::chrono::steady_clock::now();
	auto timeStamp = std::chrono::steady_clock::now();
	auto lastSendTimeStamp = timeStamp;
	int totalElapsedTime = 0;
	int timeSinceLastSendMs = 0;

	std::string line = "";
	GetCursorPos(&_mousePos);
	while (true)
	{
		if (GetAsyncKeyState(VK_F10) == -32767)
		{
			printf("Got F10. Exiting...\n");
			break;
		}

		timeStamp = std::chrono::steady_clock::now();
		totalElapsedTime = std::chrono::duration_cast<std::chrono::minutes>(timeStamp - beginTime).count();
		if (totalElapsedTime >= _maxTimeToLogMinutes)
		{
			printf("Elapsed time %d > max time %d. Shutting down logging.\n", totalElapsedTime, _maxTimeToLogMinutes);
			break;
		}

		// LOG HERE

		line.clear();

		POINT p;
		GetCursorPos(&p);
		
		if (p.x != _mousePos.x || p.y != _mousePos.y)
		{
			line += "M" + std::to_string(p.x) + ',' + std::to_string(p.y);
			anyKeyChanged = true;
		}

		_mousePos = p;

		for (auto& key : _keysToTrack)
		{
			if (keyPressed(key.code) != key.status)
			{
				key.status = !key.status;

				std::string pressReleaseChar = "P";
				if (key.status == false)
				{
					pressReleaseChar = "R";
				}

				if (anyKeyChanged) line += " ";

				line += pressReleaseChar + std::to_string(key.code);

				anyKeyChanged = true;
			}
		}

		if (anyKeyChanged)
		{
			int timeFromLastLine = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastSendTimeStamp).count();
			line += " " + std::to_string(timeFromLastLine);
			line += "\n";
			logFile << line.c_str();
			anyKeyChanged = false;

			lastSendTimeStamp = std::chrono::steady_clock::now();
		}

		Sleep(sleepTimeMs);
		// Finished logging

		++lineCount;
	}

	auto hours = totalElapsedTime / 3600;
	totalElapsedTime %= 3600;
	auto minutes = totalElapsedTime / 60;
	totalElapsedTime %= 60;
	auto seconds = totalElapsedTime;

	logFile << "***\n";
	logFile << "Elapsed time: " << hours << " hours " << minutes << " minutes " << seconds << " seconds\n";
	logFile << lineCount << " measurements gathered with rate of " << _samplesPerSecond << " samples per second\n";

	printf("Closing the log file\n");
	logFile.close();
}

bool Logger::keyPressed(int key)
{
	return(GetKeyState(key) < 0);
}
