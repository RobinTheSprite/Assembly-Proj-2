/*
parsing-input.cpp
Mark Underwood
Contains functions for creating music instructions for use in player.cpp
*/

#include "parsing-input.h"
#include <map>
#include <iostream>
#include <exception>
#include <SFML\Audio.hpp>

using bufferMapType = std::map<std::string, sf::SoundBuffer>;

struct tempo
{
	float halfNote;
	float quarterNote;
	float eighthNote;
	float sixteenthNote;
};

tempo setTempo(float bpm)
{
	tempo noteLength;

	noteLength.quarterNote = 60.0f / bpm;
	noteLength.halfNote = noteLength.quarterNote * 2.0f;
	noteLength.eighthNote = noteLength.quarterNote / 2.0f;
	noteLength.sixteenthNote = noteLength.quarterNote / 4.0f;

	return noteLength;
}

void parseAction(std::tuple<size_t, float, sf::Sound> & songEvent, bufferMapType & buffers,
	std::string & instruction, float bpm)
{
	tempo noteLength = setTempo(bpm);

	if (instruction.substr(0, 2) == "00")
	{
		std::get<1>(songEvent) = -1;
	}
	else if (instruction.substr(0, 2) == "02")
	{
		std::get<1>(songEvent) = noteLength.halfNote;
	}
	else if (instruction.substr(0, 2) == "04")
	{
		std::get<1>(songEvent) = noteLength.quarterNote;
	}
	else if (instruction.substr(0, 2) == "08")
	{
		std::get<1>(songEvent) = noteLength.eighthNote;
	}
	else if (instruction.substr(0, 2) == "16")
	{
		std::get<1>(songEvent) = noteLength.sixteenthNote;
	}
	else if (instruction[0] == '-')
	{
		for (size_t i = 0; i < instruction.size(); ++i)
		{
			std::get<0>(songEvent) = 0;
			std::get<1>(songEvent) += noteLength.sixteenthNote;
		}
		return;
	}

	std::string noteType;
	try
	{
		noteType = instruction.substr(2, std::string::npos);
	}
	catch (std::exception & e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		std::cout << "String: " << instruction << std::endl;
	}

	if (noteType == "R")
	{
		std::get<0>(songEvent) = 1;
		std::get<2>(songEvent).setBuffer(buffers["R"]);
	}
	else
	{
		std::get<0>(songEvent) = 1;
		std::get<2>(songEvent).setBuffer(buffers[noteType]);
	}
}