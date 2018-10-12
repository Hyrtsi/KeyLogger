#pragma once

#include "Common.hpp"

class LogParser
{
public:
	LogParser();

	// TODO
	// This will read the log to memory, but how to represent the data?
	// Class or struct?
	// Who takes the data?
	void parseLog(const std::string fileName);

	//std::vector<Action> parseLog(const std::string fileName);

	// TODO public getters for the data

private:
	// TODO data here
	// TODO does this class have to hold the data in memory or just pass it?
	// Who is responsible of the data after parsing?
	// Who has the ownership?
	
	// TEMP: hold this here
	std::vector<Action> _actions;

	//std::vector<vec2>	_mousePos;
	//std::vector<int>	_timeStamp;
	
};

