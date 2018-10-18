#include "stdafx.h"
#include "Logger.hpp"

Logger::Logger(int samplesPerSecond = 60, int maxTimeToLogMinutes = 60)
{
	_samplesPerSecond = samplesPerSecond;
	_maxTimeToLogMinutes = maxTimeToLogMinutes;

	// TODO these have been added to common
	// If they work, remove these from here...

	int keys[] = { VK_LBUTTON, VK_RBUTTON, VK_SPACE,
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		'1', '2', '3', VK_ESCAPE };

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

	// TEMP: while debugging, this will save time...
	//std::string fileName = "log_" + std::to_string(currentUnixTime) + ".txt";
	std::string fileName = "debuglog.txt";
	std::ofstream logFile(fileName);

	int sleepTimeMs = (int)(1000.f / (float)_samplesPerSecond);
	int lineCount = 0;
	
	bool anyKeyChanged = false;

	printf("Beginning logging to file %s\n",
		fileName.c_str());

	auto timeStamp = std::chrono::steady_clock::now();
	auto lastSendTimeStamp = timeStamp;
	int totalElapsedTimeSec = 0;
	int totalElapsedTimeMin = 0;
	int timeFromLastLine = 0;
	int processingTimeTaken = 0;

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
		totalElapsedTimeMin = std::chrono::duration_cast<std::chrono::minutes>(timeStamp - beginTime).count();
		if (totalElapsedTimeMin >= _maxTimeToLogMinutes)
		{
			printf("Elapsed time %d (sec) > max time %d (min). Shutting down logging.\n", totalElapsedTimeMin, _maxTimeToLogMinutes);
			break;
		}

		line.clear();
		anyKeyChanged = logMouse(line);
		anyKeyChanged |= logKeyboard(line);

		if (anyKeyChanged)
		{
			timeFromLastLine = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastSendTimeStamp).count();

			// Log the duration since last ("sleep")
			logFile << "S" << timeFromLastLine << "\n";

			// Do not save the time from last line here...? That breaks the chronology
			// Idea: event, sleep before the next, next event, ...
			//line += " " + std::to_string(timeFromLastLine);
			
			logFile << line.c_str() << "\n";
			anyKeyChanged = false;

			lastSendTimeStamp = std::chrono::steady_clock::now();

			++lineCount;
		}

		// TEMP TEST
		// TODO no negative numbers please
		processingTimeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeStamp).count();

		//printf("Processing took %d total sleep %d = %d\n",
		//	processingTimeTaken, sleepTimeMs, sleepTimeMs - processingTimeTaken);

		Sleep(sleepTimeMs - processingTimeTaken);


		//Sleep(sleepTimeMs);


	} // Finished logging

	// We may use the last timestamp
	totalElapsedTimeSec = std::chrono::duration_cast<std::chrono::seconds>(timeStamp - beginTime).count();

	auto hours = totalElapsedTimeSec / 3600;
	totalElapsedTimeSec %= 3600;
	auto minutes = totalElapsedTimeSec / 60;
	totalElapsedTimeSec %= 60;
	auto seconds = totalElapsedTimeSec;

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

bool Logger::logMouse(std::string & line)
{
	bool anyKeyChanged = false;

	POINT p;
	GetCursorPos(&p);

	if (p.x != _mousePos.x || p.y != _mousePos.y)
	{
		line += 'M' + std::to_string(p.x) + ',' + std::to_string(p.y);
		anyKeyChanged = true;
	}

	_mousePos = p;

	return anyKeyChanged;
}

bool Logger::logKeyboard(std::string & line)
{
	bool anyKeyChanged = false;

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

			// Add space as delimiter if there already is something written
			if (line.length() > 0) line += " ";

			line += pressReleaseChar + std::to_string(key.code);

			anyKeyChanged = true;
		}
	}

	return anyKeyChanged;
}
