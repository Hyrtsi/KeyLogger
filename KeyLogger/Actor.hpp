#pragma once

#include <chrono>
// TODO move to common?
#include <Windows.h>
#include "Common.hpp"
#include "LogParser.hpp"

class Actor
{
public:
	Actor();
	void replayFromFile(const std::string fileName);

private:
	void changeKeyState(INPUT& input, int keyCode, bool pressed);
	void changeKeyStateHw(INPUT& input, int keyCode, bool pressed);
	void pressAndReleaseKey(INPUT& input, int keyCode, int durationMs);
	
	void moveMouse(INPUT & input, vec2 point, bool leftUp, bool leftDown, bool rightUp, bool rightDown);
	void moveMouse(INPUT & input, vec2 point);
	vec2 pointToGlobal(const vec2& point);

	void mouseLissajousDrawDemo(void);
	void putKeystrokeDown(int times, int keyCode);

	void mouseDemo(void);

	void replayLog(const std::string fileName);


	// How many milliseconds the button will be down at minimum
	int		_sampleLengthMs;
	vec2d	_screenSize;
};

