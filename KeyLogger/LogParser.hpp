#pragma once

#include "Common.hpp"

class LogParser
{
public:
	LogParser();
	std::vector<Action> parseLog(const std::string fileName);

private:
	
};

