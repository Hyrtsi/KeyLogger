#include "stdafx.h"
#include "LogParser.hpp"
#include <sstream>      // std::istringstream
#include <iterator>

LogParser::LogParser()
{
}

void LogParser::parseLog(const std::string fileName)
{
	std::string line;
	std::ifstream myfile(fileName);

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

			/*
			OK, we get the event data
			How to store the events to be playbacked (or else like plotting)

			Lets look at the actor...

			*/

			for (int i = 0; i < results.size() - 1; ++i)
			{
				if (results[i][0] == 'M')
				{
					printf("Read mouse data %s\n", results[i].c_str());
					// Mouse is commma-separated two ints
					vec2 mousePos;
				}
				else if (results[i][0] == 'P')
				{
					printf("Read press key %s\n", results[i].c_str());
				}
				else if (results[i][0] == 'R')
				{
					printf("Read release key %s\n", results[i].c_str());
				}
				else
				{
					printf("ERROR while parsing string! Unexpected format... String: %s\n", results[i].c_str());
				}
				
			}






			// We know the last one is always timestamp

			printf("Time after last: %d\n", atoi(results[results.size()-1].c_str()));

			//int timeStamp;
			//sscanf_s(line.c_str(), "%d",
			//	&timeStamp);

			++nLines;
		}
		myfile.close();

		printf("Successfully read %d lines from file %s\n",
			nLines, fileName.c_str());
	}
	else
	{
		printf("Unable to open file %s\n", fileName.c_str());
	}
}
