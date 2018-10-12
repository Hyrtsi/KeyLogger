#include "stdafx.h"
#include "Actor.hpp"

Actor::Actor()
{
	_screenSize = { 1920.0, 1080.0 };
}

void Actor::doThing(void)
{
	INPUT input = { 0 };
	int x = 300;
	int y = 330;
	vec2 point = { 1200, 330 };

	//moveMouse(input, point, false, true, false, false);
	//SendInput(1, &input, sizeof(INPUT));
	//Sleep(31);

	LogParser p;
	std::vector<Action> actions = p.parseLog("debuglog.txt");

	for (auto a : actions)
	{
		if (GetAsyncKeyState(VK_F12) == -32767)
		{
			break;
		}

		if (a.e == EVENT_PRESS)
		{
			//input = { 0 };
			memset(&input, 0, sizeof(INPUT));
			moveMouse(input, point, false, true, false, false);
			SendInput(1, &input, sizeof(INPUT));
			Sleep(31);
		}

		if (a.e == EVENT_RELEASE)
		{
			//input = { 0 };
			memset(&input, 0, sizeof(INPUT));
			moveMouse(input, point, true, false, false, false);
			SendInput(1, &input, sizeof(INPUT));
			Sleep(31);
		}

		if (a.e == EVENT_MOVE)
		{
			//input = { 0 };
			memset(&input, 0, sizeof(INPUT));
			moveMouse(input, a.mousePos, false, false, false, false);
			SendInput(1, &input, sizeof(INPUT));
		}

		if (a.e == EVENT_WAIT)
		{
			Sleep(a.waitTimeMs);
		}
	}

	//input = { 0 };
	//moveMouse(input, point, true, false, false, false);
	//SendInput(1, &input, sizeof(INPUT));
	//Sleep(31);

}

void Actor::doAnotherThing(void)
{
	INPUT input = { 0 };
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

		if (GetAsyncKeyState(VK_F12) == -32767)
		{
			break;
		}


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
	printf("- - - Preparing the replaying of log %s\n",
		fileName.c_str());

	printf("Parsing log before replay\n");
	LogParser p;
	std::vector<Action> actions = p.parseLog(fileName);

	std::vector<INPUT> inputs;

	printf("Replaying log\n");
	for (auto a : actions)
	{
		INPUT input;

		if (GetAsyncKeyState(VK_F12) == -32767)
		{
			printf("Got F12: breaking from log replay\n");
			// TODO release all unreleased keys...
			moveMouse(input, a.mousePos, true, false, false, false);
			SendInput(1, &input, sizeof(INPUT));
			return;
		}


		switch (a.e)
		{
		case EVENT_NONE:
			break;
		case EVENT_PRESS:
			// TEMP notice that both left and right act as left
			if (a.keyCode == VK_LBUTTON || a.keyCode == VK_RBUTTON)
			{
				moveMouse(input, a.mousePos, false, true, false, false);
				SendInput(1, &input, sizeof(INPUT));
				//inputs.push_back(input);
			}
			break;
		case EVENT_RELEASE:
			// TEMP notice that both left and right act as left
			if (a.keyCode == VK_LBUTTON || a.keyCode == VK_RBUTTON)
			{
				moveMouse(input, a.mousePos, true, false, false, false);
				SendInput(1, &input, sizeof(INPUT));
				//inputs.push_back(input);
			}
			break;
		case EVENT_MOVE:
			a.mousePos;
			moveMouse(input, a.mousePos);
			SendInput(1, &input, sizeof(INPUT));
			//inputs.push_back(input);
			break;
		case EVENT_WAIT:

			// Each time wait is reached,
			// SendInput
			// Flush buffer
			// Wait

			// TODO if sleep is first...
			//if (inputs.size() > 0)
			//{
			//	SendInput(inputs.size(), &inputs[0], sizeof(INPUT));
			//}
			//inputs.clear();
			Sleep(a.waitTimeMs);
			break;
		default:
			break;
		}
		Sleep(31);
	}


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
}


void Actor::moveMouse(INPUT & input, vec2 point)
{
	point = pointToGlobal(point);

	input.type = INPUT_MOUSE;
	input.mi.dx = point.x;
	input.mi.dy = point.y;
	input.mi.mouseData = 0;
	input.mi.dwFlags |= (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
	input.mi.time = 0;
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



/*
TEMP TODO USE
*/
void putKeystrokeDown(int times, int keyCode)
{
	INPUT ip = { 0 };
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = keyCode;

	std::vector<INPUT> ips;
	ips.reserve((times * 2));

	for (int i = 0; i < times; i++)
	{
		ip.ki.dwFlags = 0;
		ips.push_back(ip);

		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		ips.push_back(ip);
	}

	SendInput(ips.size(), &ips[0], sizeof(INPUT));
}