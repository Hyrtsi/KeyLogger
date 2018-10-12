#include "stdafx.h"
#include "LogParser.hpp"
#include <sstream>      // std::istringstream
#include <iterator>

LogParser::LogParser()
{
}

std::vector<Action> LogParser::parseLog(const std::string fileName)
{
	std::string line;
	std::ifstream myfile(fileName);

	std::vector<Action> actions;
	Action action;

	if (myfile.is_open())
	{
		int nLines = 0;
		
		while (getline(myfile, line))
		{
			if (line == "***")
			{
				// Reached the end
				break;
			}

			std::istringstream iss(line);
			std::vector<std::string> results(std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>());

			std::vector<std::string> entries;

			// TODO we have to be able to
			// load all on same line into same instance...
			// How to stack them?
			// Somehow they have to be grouped
			// The data structure could, however, still be std::vector (1d)
			// std vector of std vectors would not be so nice

			/*
			Idea 1

			Load all sequentially
			Upon playback, stack actions into the same instance until a sleep is hit
			Then flush (=SendInput)
			Then sleep
			Then repeat...
			*/



			for (int i = 0; i < results.size(); ++i)
			{
				auto& entry = results[i];
				auto& id = entry[0];
				auto& entryData = entry.substr(1, entry.size());

				//printf("Line: %s, id: %c, data: %s\n", line.c_str(), id, lineData.c_str());

				//printf("Entry %s\n", entry.c_str());

				if (id == 'M')
				{
					//printf("Read mouse data %s\n", line.c_str());

					// Mouse is commma-separated two ints
					// TODO sscanf_s rather...
					auto commaPos = entryData.find(',');
					vec2 mousePos =
					{
						atoi(entryData.substr(0, commaPos).c_str()),
						atoi(entryData.substr(commaPos + 1, 4).c_str())
					};

					action.e = EVENT_MOVE;
					action.mousePos = mousePos;

					//printf("Mouse pos read: %d %d\n", mousePos.x, mousePos.y);
				}
				else if (id == 'P')
				{
					//printf("Read press key %s\n", line.c_str());
					action.e = EVENT_PRESS;
					action.keyCode = atoi(entryData.c_str());
				}
				else if (id == 'R')
				{
					//printf("Read release key %s\n", line.c_str());
					action.e = EVENT_RELEASE;
					action.keyCode = atoi(entryData.c_str());
				}
				else if (id == 'S')
				{
					//printf("Read sleep %s\n", line.c_str());
					action.e = EVENT_WAIT;
					action.waitTimeMs = atoi(entryData.c_str());
				}
				else
				{
					printf("ERROR while parsing string! Unexpected format... String: %s\n",
						line.c_str());
				}
			}
			
			actions.push_back(action);
			++nLines;

		}
		myfile.close();

		printf("Successfully read %d lines from file %s\n",
			nLines, fileName.c_str());

		printf("TEMP: got %d actions\n", actions.size());
	}
	else
	{
		printf("Unable to open file %s\n", fileName.c_str());
	}


	return actions;
}
