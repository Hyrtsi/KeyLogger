#include "stdafx.h"
#include "Actor.hpp"

Actor::Actor()
{
	_screenSize = { 1920.0, 1080.0 };
}

void Actor::replayFromFile(const std::string fileName)
{
	INPUT input = { 0 };
	POINT mousePos;
	GetCursorPos(&mousePos);
	vec2 point = { mousePos.x, mousePos.y }; 

	LogParser p;
	std::vector<Action> actions = p.parseLog("debuglog.txt");
	bool insideBounds = false;
	for (auto a : actions)
	{
		if (GetAsyncKeyState(VK_F12) == -32767)
		{
			printf("Got F12: Breaking from playback...\n");
			break;
		}

		// TEMP
		// Click only if inside some pre-defined game window area

		if (a.e == EVENT_PRESS)
		{

			insideBounds = (a.mousePos.x > 970 && a.mousePos.x < 1905
				&& a.mousePos.y > 35 && a.mousePos.y < 1025);

			if (a.keyCode == VK_LBUTTON && insideBounds)
			{
				memset(&input, 0, sizeof(INPUT));
				moveMouse(input, point, false, true, false, false);
				SendInput(1, &input, sizeof(INPUT));
			}
			else if (a.keyCode == VK_RBUTTON && insideBounds)
			{
				memset(&input, 0, sizeof(INPUT));
				moveMouse(input, point, false, false, false, true);
				SendInput(1, &input, sizeof(INPUT));
			}
			else
			{
				changeKeyState(input, a.keyCode, true);
				SendInput(1, &input, sizeof(INPUT));
			}
		}

		if (a.e == EVENT_RELEASE)
		{
			if (a.keyCode == VK_LBUTTON)
			{
				memset(&input, 0, sizeof(INPUT));
				moveMouse(input, point, true, false, false, false);
				SendInput(1, &input, sizeof(INPUT));
			}
			else if (a.keyCode == VK_RBUTTON)
			{
				memset(&input, 0, sizeof(INPUT));
				moveMouse(input, point, false, false, true, false);
				SendInput(1, &input, sizeof(INPUT));
			}
			else
			{
				changeKeyState(input, a.keyCode, false);
				SendInput(1, &input, sizeof(INPUT));
			}
		}

		if (a.e == EVENT_MOVE)
		{
			point = a.mousePos;
			//memset(&input, 0, sizeof(INPUT));
			moveMouse(input, point, false, false, false, false);
			SendInput(1, &input, sizeof(INPUT));
		}

		if (a.e == EVENT_WAIT)
		{
			if (a.waitTimeMs > 10000)
			{
				printf("Sleeping %d ms\n", a.waitTimeMs);
			}

			Sleep(a.waitTimeMs);
		}
	}

	printf("Finished playback\n");

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

void Actor::changeKeyStateHw(INPUT& input, int keyCode, bool pressed)
{
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = 0;
	input.ki.wScan = keyCode;
	input.ki.dwFlags = (pressed) ? KEYEVENTF_SCANCODE : (KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE);
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

int Actor::virtualToHardware(int virtualCode)
{
	/*
	TODO hex or decimal?
	*/
	switch (virtualCode)
	{
	case VK_SPACE:
		return 57;
	case VK_LEFT:
		return 73;	//75;
	case VK_RIGHT:
		return 74;	//77;
	case VK_UP:
		//return 140;	//72;
		return VK_UP;		// 38
	case VK_DOWN:
		//return 145;	//80;
		return VK_DOWN;		// 40
	case VK_ESCAPE:
		return 1;
	case 49:
		return 2;
	case 50:
		return 3;
	case 51:
		return 4;
	default:
		printf("ERROR: invalid virtual key code %d\n", virtualCode);
		return 0;
		break;
	}

	/*
	74 is down
	38 is N
	73 is T
	*/
}

/*
TEMP TODO USE
*/
/*
void Actor::putKeystrokeDown(int times, int keyCode)
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

void Actor::mouseDemo(void)
{
	INPUT input = { 0 };
	int x = 1000;
	int y = 1000;

	printf("Beginning mouse demo in 2 seconds\n");
	Sleep(2000);
	printf("Start\n");
	
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
	
	printf("End\n");
}
*/