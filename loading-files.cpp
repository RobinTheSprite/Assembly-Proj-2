/*
loading-files.cpp
Mark Underwood
Contains functions for getting instrument sounds for player.cpp
*/

#include "loading-files.h"
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using bufferMapType = std::map<std::string, sf::SoundBuffer>;

sf::SoundBuffer getWavFile(std::string filename)
{
	filename += ".wav";
	sf::SoundBuffer toBeReturned;
	if (!toBeReturned.loadFromFile(filename))
		std::cout << "File " << filename << ".wav " << "not available." << std::endl;

	return toBeReturned;
}

bufferMapType getNotes(std::string fileLocation)
{
	bufferMapType notes;

	std::ifstream names(fileLocation + "names.txt");
	std::string line;

	while (std::getline(names, line))
	{
		std::string filename = fileLocation;
		filename.append(line);

		notes[line] = getWavFile(filename);
	}

	return notes;
}

bufferMapType getSongSounds(std::string & track)
{
	bufferMapType soundBuffers;
	std::istringstream trackStream(track);
	std::string instrument;
	trackStream >> instrument;
	if (track.substr(0, 3) == "[R]")
	{
		std::string filename = "Instruments\\Rhythm\\" + instrument.substr(3, std::string::npos);

		soundBuffers["R"] = getWavFile(filename);
	}
	else if (track.substr(0, 3) == "[M]")
	{
		soundBuffers = getNotes("Instruments\\" + instrument.substr(3, std::string::npos) + "\\");
	}

	return soundBuffers;
}