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
	EVENT_PRESS,
	EVENT_RELEASE,
	EVENT_MOVE
};

struct Action
{
	Event_t e;
	int keyCode;			// For mouse move, this is ignored
	int timeUntilNextMs;
};

/*
struct Action_t
{
	int keyCode;			// -1 = mouse move
	bool press;				// true = press, false = release. For mouse move, whatever
	int timeUntilNextMs;	// Milliseconds from this event to the next event
};
*/

