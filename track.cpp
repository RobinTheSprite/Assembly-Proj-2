
#include <functional>
#include <iostream>
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

void playSound(sf::Sound sound, float secs)
{
	sound.play();
	sf::Clock clock;

	if (secs != -1)
	{
		while (clock.getElapsedTime() < sf::seconds(secs)) {}
	}
	else
	{
		while (sound.getStatus() == sf::Sound::Playing) {}
	}

	sound.stop();
}

void rest(sf::Sound dummy, float secs)
{
	sf::Clock clock;
	while (clock.getElapsedTime() < sf::seconds(secs)) {}
}

void Track::play()
{
	std::vector < std::function<void(sf::Sound, float)> > action = { rest, playSound };
	for (auto t : _track)
	{
		action[std::get<0>(t)](std::get<2>(t), std::get<1>(t));
	}
}

void Track::parseAction(std::tuple<size_t, float, sf::Sound> & songEvent, bufferMapType & buffers,
					   std::string & instruction)
{
	if (instruction.substr(0, 2) == "00")
	{
		std::get<1>(songEvent) = -1;
	}
	else if (instruction.substr(0, 2) == "01")
	{
		std::get<1>(songEvent) = _wholeNote;
	}
	else if (instruction.substr(0, 2) == "02")
	{
		std::get<1>(songEvent) = _halfNote;
	}
	else if (instruction.substr(0, 2) == "04")
	{
		std::get<1>(songEvent) = _quarterNote;
	}
	else if (instruction.substr(0, 2) == "08")
	{
		std::get<1>(songEvent) = _eighthNote;
	}
	else if (instruction.substr(0, 2) == "16")
	{
		std::get<1>(songEvent) = _sixteenthNote;
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

void Track::insert(std::tuple<size_t, float, sf::Sound> & songEvent)
{
	_track.push_back(songEvent);
}