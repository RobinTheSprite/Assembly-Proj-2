
#include <functional>
#include <iostream>
#include <atomic>
#include <SFML\Audio.hpp>
#include "track.h"

using bufferMapType = std::map<std::string, sf::SoundBuffer>;

Track::Track(float bpm)
{
	_quarterNote = 60.0f / bpm;
	_wholeNote = _quarterNote * 4.0f;
	_halfNote = _quarterNote * 2.0f;
	_eighthNote = _quarterNote / 2.0f;
	_sixteenthNote = _quarterNote / 4.0f;
}

void Track::play(sf::Clock & sync)
{
	for (auto & t : _track)
	{
		if (std::get<0>(t) == 1)
		{
			std::get<2>(t).play();
		}
		while (sync.getElapsedTime() < sf::seconds(std::get<1>(t))) {}
		std::get<2>(t).stop();
	}
}

void Track::parseAction(std::tuple<size_t, float, sf::Sound> & songEvent, bufferMapType & buffers,
					   std::string & instruction)
{
	if (instruction.substr(0, 2) == "01")
	{
		_timeDistance += _wholeNote;
	}
	else if (instruction.substr(0, 2) == "02")
	{
		_timeDistance += _halfNote;
	}
	else if (instruction.substr(0, 2) == "04")
	{
		_timeDistance += _quarterNote;
	}
	else if (instruction.substr(0, 2) == "08")
	{
		_timeDistance += _eighthNote;
	}
	else if (instruction.substr(0, 2) == "16")
	{
		_timeDistance += _sixteenthNote;
	}
	std::get<1>(songEvent) = _timeDistance;

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
	else if (noteType == "-")
	{
		std::get<0>(songEvent) = 0;
	}
	else if (buffers.count(noteType) > 0)
	{
		std::get<0>(songEvent) = 1;
		std::get<2>(songEvent).setBuffer(buffers[noteType]);
	}
	else
	{
		std::cout << "ERROR: Instruction \"" << instruction << "\" not found" << std::endl;
	}
}

void Track::push_back(std::tuple<size_t, float, sf::Sound> & songEvent)
{
	_track.push_back(songEvent);
}