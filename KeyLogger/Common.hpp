#pragma once

#include <string>
#include <fstream>
#include <vector>

struct vec2
{
	int x;
	int y;
};

struct vec2d
{
	double x;
	double y;
};

enum Event_t
{
	EVENT_NONE = 0,
	EVENT_PRESS = 1,
	EVENT_RELEASE = 2,
	EVENT_MOVE =  3,
	EVENT_WAIT =  4
};

struct Action
{
	Event_t e;
	int keyCode;			// For mouse move, this is ignored
	vec2 mousePos;			// For key press, this is ignored
	int waitTimeMs;
};

/*
struct Action_t
{
	int keyCode;			// -1 = mouse move
	bool press;				// true = press, false = release. For mouse move, whatever
	int timeUntilNextMs;	// Milliseconds from this event to the next event
};
*/

/*
// TODO if used here, include windows...
auto g_virtualKeyCodes =
{
	VK_SPACE,
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	VK_ESCAPE,
	49,			// '1'
	50,			// '2'
	51,			// '3'
};

auto g_hardwareKeyCodes =
{
	57,				// Space
	75,				// Left arrow
	77,				// Right arrow
	72,				// Up arrow
	80,				// Down arrow
	1,				// Escape
	2,				// '1'
	3,				// '2'
	4,				// '3'
};
*/