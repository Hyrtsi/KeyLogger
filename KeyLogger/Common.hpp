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
	EVENT_NONE = 1 << 0,
	EVENT_PRESS = 1 << 1,
	EVENT_RELEASE = 1 << 2,
	EVENT_MOVE = 1 << 3,
	EVENT_WAIT = 1 << 4
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

