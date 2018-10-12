#pragma once

#include <stdio.h>
#include <Windows.h>
#include <chrono>
#include "Common.hpp"

struct Key
{
	int		code;
	bool	status;
};

class Logger
{
public:
	Logger(int samplesPerSecond, int maxTimeToLogMinutes);

	void createLog(void);

private:

	bool keyPressed(int key);


	//std::vector<int>	_keysToTrack;
	//std::vector<bool>	_keyStatus;
	std::vector<Key>	_keysToTrack;

	int					_samplesPerSecond;
	int					_maxTimeToLogMinutes;
	POINT				_mousePos;
};

