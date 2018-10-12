#include "stdafx.h"
#include "Actor.hpp"

Actor::Actor()
{
	_screenSize = { 1920.0, 1080.0 };
}

void Actor::doThing(void)
{
	printf("Pressing down the button...\n");
	Sleep(910);
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = 'A';
	input.ki.wScan = 'A';
	input.ki.dwFlags = 0; // key down

	// TODO one send is not enough! Send as many as needed...

	SendInput(1, &input, sizeof(INPUT));
	Sleep(31);

	SendInput(1, &input, sizeof(INPUT));
	Sleep(31);

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = 'A';
	input.ki.wScan = 'A';
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void Actor::doAnotherThing(void)
{
	INPUT input = { 0 };

	printf("Pressing\n");
	Sleep(1000);
	for (int i = 0; i < 10; ++i)
	{
		changeKeyState(input, 'A', true);
		SendInput(1, &input, sizeof(INPUT));
		Sleep(31);
	}
	return;

	auto begin = std::chrono::steady_clock::now();
	auto end = std::chrono::steady_clock::now();
	auto timeElapsed = std::chrono::duration_cast<std::chrono::minutes>(end - begin).count();

	//printf("Putting the key A down\n");
	//Sleep(1000);
	//changeKeyState(input, 'A', true);
	
	printf("Putting mouse left down for some time\n");
	Sleep(1000);
	// 0...65535
	int x = 300;
	int y = 330;
	vec2 point = { 1200, 330 };

	moveMouse(input, point, false, true, false, false);
	SendInput(1, &input, sizeof(INPUT));
	Sleep(31);

	float t = 0.f;
	for (int i = 0; i < 1000; ++i)
	{
		input = { 0 };
		point.x = x + (int)(100 * sin(20.5f * t));
		point.y = y + (int)(100 * cos(50.f * t));

		//printf("Point: %d %d\n", point.x, point.y);

		moveMouse(input, point, false, false, false, false);
		SendInput(1, &input, sizeof(INPUT));
		Sleep(31);

		t += 0.005f;
	}

	printf("Finished: Point: %d %d\n", point.x, point.y);

	input = { 0 };
	moveMouse(input, point, true, false, false, false);
	SendInput(1, &input, sizeof(INPUT));
	Sleep(31);

	/*
	changeKeyState(input, 'a', true);
	Sleep(31);

	changeKeyState(input, 'a', false);
	Sleep(31);
	*/
}

void Actor::mouseDemo(void)
{
	INPUT input = { 0 };
	int x = 1000;
	int y = 1000;

	printf("Beginning mouse demo in 2 seconds\n");
	Sleep(2000);
	printf("Start\n");
	/*
	for (int i = 0; i < 10; ++i)
	{
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = 10;
		input.mi.dy = 10;
		input.mi.dwFlags |= MOUSEEVENTF_MOVE;

		SendInput(1, &input, sizeof(INPUT));
		Sleep(31);
	}
	return;
	*/
	/*
	for (int i = 0; i < 200; ++i)
	{
		input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags |= (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);

		x += 200;
		y += 200;

		SendInput(1, &input, sizeof(INPUT));
		Sleep(31);
	}
	*/
	printf("End\n");
}

void Actor::replayLog(const std::string fileName)
{
	// (1) get data
	// TODO:
	// - this class owns logparser?
	// - read data here
	// - save to some vector(s)


	// Data is a list of actions: event or wait
	// event is press or release or move
	// wait is sleep ms before next event

	// Could the vector be a list of pairs (action, wait) ?

	std::vector<Action> actions;	// TODO get from function

	LogParser p;
	p.parseLog(fileName);

}

/*
This is a test
Maybe the input has to be initialized each time...
Hope nothing bad happens...
*/
void Actor::changeKeyState(INPUT& input, int keyCode, bool pressed)
{
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = keyCode;
	input.ki.wScan = keyCode;
	input.ki.dwFlags = (pressed) ? 0 : KEYEVENTF_KEYUP;
}

/*
Actually this doesn't work for multiple actions unless multi thread
*/
void Actor::pressAndReleaseKey(INPUT & input, int keyCode, int durationMs)
{
	changeKeyState(input, keyCode, true);
	Sleep(31);
	changeKeyState(input, keyCode, false);
}

/*

The bit flags that specify mouse button status
are set to indicate changes in status,
not ongoing conditions.

For example, if the left mouse button is pressed and held down,
MOUSEEVENTF_LEFTDOWN is set when the left button is first pressed,
but not for subsequent motions. Similarly, MOUSEEVENTF_LEFTUP is set
only when the button is first released.

*/
void Actor::moveMouse(INPUT & input, vec2 point, bool leftUp, bool leftDown, bool rightUp, bool rightDown)
{
	point = pointToGlobal(point);

	input.type = INPUT_MOUSE;
	input.mi.dx = point.x;
	input.mi.dy = point.y;
	input.mi.mouseData = 0;
	input.mi.dwFlags |= (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
	input.mi.time = 0;

	if (leftUp)		input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
	if (leftDown)	input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
	if (rightUp)	input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
	if (rightDown)	input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;

	/*
	MOUSEEVENTF_LEFTDOWN
	MOUSEEVENTF_LEFTUP
	MOUSEEVENTF_RIGHTDOWN
	MOUSEEVENTF_RIGHTUP
	*/

	//input.ki.wVk = keyCode;
	//input.ki.wScan = keyCode;
	//input.ki.dwFlags = (pressed) ? 0 : KEYEVENTF_KEYUP;
}

/*
Gets some
x in 0...1920
y in 0...1080
location,

transforms it into
x in 0...65535
y in 0...65535
location
*/
vec2 Actor::pointToGlobal(const vec2 & point)
{
	int x = (int)(65535.0 * point.x / _screenSize.x);
	int y = (int)(65535.0 * point.y / _screenSize.y);
	
	vec2 ret =
	{
		x,
		y
	};

	return ret;
}
