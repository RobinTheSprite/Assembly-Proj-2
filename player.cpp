/*
player.cpp
Mark Underwood
Plays a song using instructions from a text file
*/

#include <thread>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <SFML\Audio.hpp>
#include "loading-files.h"
#include "parsing-input.h"

using bufferMapType = std::map<std::string, sf::SoundBuffer>;
sf::Clock sync;

void playSound(sf::Sound sound, float secs)
{
	sound.play();
	sf::Clock clock;

	if (secs != -1)
	{
		while (clock.getElapsedTime() < sf::seconds(secs)){}
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

void playTrack(std::string & line, bufferMapType & soundBuffers, float bpm)
{							
	sf::Sound dummy;
	std::vector<std::tuple<size_t, float, sf::Sound>> track;
	
	std::istringstream iss(line);
	std::string instruction;
	while (iss >> instruction)
	{
		std::tuple<size_t, float, sf::Sound> songEvent = std::make_tuple(0, 0.0, dummy);
	
		if (instruction[0] != '!')
		{
			parseAction(songEvent, soundBuffers, instruction, bpm);
			track.push_back(songEvent);
		}
	}

	std::vector < std::function<void(sf::Sound, float)> > action = { rest, playSound };
	while (sync.getElapsedTime() < sf::seconds(1)) {}
	for (auto t : track)
	{
		action[std::get<0>(t)](std::get<2>(t), std::get<1>(t));
	}
}

bool checkExtension(std::string & filename)
{
	std::string ext;
	for (auto it = (filename.end() - 1); it > (filename.end() - 4); --it)
	{
		ext += *it;
	}
	return (ext == "lm.");
}

void playSong(std::string filename, float bpm)
{
	if (!checkExtension(filename))
	{
		std::cout << "File Extension Incorrect" << std::endl;
		return;
	}
	std::vector<std::string> songTracks;
	std::vector<bufferMapType> soundsForEachTrack;
	
	std::ifstream songfile(filename);
	std::string line;
	while (std::getline(songfile, line))
	{
		songTracks.push_back(line);
		soundsForEachTrack.push_back(getSongSounds(songTracks.back()));
	}

	std::vector<std::thread> allThreads(songTracks.size());

	for (size_t i = 0; i < songTracks.size(); ++i)
	{
		allThreads[i] = std::thread(playTrack, songTracks[i], soundsForEachTrack[i], bpm);
	}
	
	for (size_t i = 0; i < songTracks.size(); ++i)
	{
		allThreads[i].join();
	}
}

int main()
{
	playSong("codebreaker.ml", 120);

	std::cout << "Press ENTER to exit";
	while(std::cin.get() != '\n') 

	return 0;
}