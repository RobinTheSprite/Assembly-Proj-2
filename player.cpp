
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <SFML\Audio.hpp>

using bufferMapType = std::map<std::string, sf::SoundBuffer>;
sf::Clock sync;

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

bufferMapType getNotes(std::string fileLocation)
{
	bufferMapType notes;

	std::ifstream names(fileLocation + "names.txt");
	std::string line;
	
	while (std::getline(names, line))
	{
		std::string filename = fileLocation;
		filename.append(line);
		filename += ".wav";

		sf::SoundBuffer tempSB;
		if (!tempSB.loadFromFile(filename))
			std::cout << "File " << filename << ".wav " << "not available." << std::endl;

		notes[line] = tempSB;
	}

	return notes;
}

void playSound(sf::Sound sound, float secs)
{
	sound.play();
	sf::Clock clock;
	//if (secs != -1)
	//{
		while (clock.getElapsedTime() < sf::seconds(secs))
		{
			//if (clock.getElapsedTime() > sf::seconds(secs - 2))
			//{
			//	for (size_t i = 1000000000; i < 0; --i)
			//	{
			//		sound.setVolume(i / 10000000);
			//	}
			//}
		}
	//}
	//else
	//{
	//	while (sound.getStatus() == sf::SoundSource::Status::Playing) {}
	//}
	sound.stop();
}

void rest(sf::Sound dummy, float secs)
{
	sf::Clock clock;
	while (clock.getElapsedTime() < sf::seconds(secs)) {}
}

void parseAction(std::tuple<size_t, float, sf::Sound> & songEvent, bufferMapType & buffers, 
				 std::string & instruction, float bpm)
{
	tempo noteLength = setTempo(bpm);

	if (instruction.substr(0, 2) == "02")
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

	std::string noteType = instruction.substr(2, std::string::npos);
	if (noteType == "R")
	{
		std::get<0>(songEvent) = 1;
		std::get<2>(songEvent).setBuffer(buffers["R"]);
	}
	else if (instruction[0] == '-')
	{
		for (auto c : instruction)
		{
			std::get<0>(songEvent) = 0;
			std::get<1>(songEvent) += noteLength.sixteenthNote;
		}
	}
	else
	{
		std::get<0>(songEvent) = 1;
		std::get<2>(songEvent).setBuffer(buffers[noteType]);
	}
}

void playTrack(std::string line, float bpm)
{							
	bufferMapType soundBuffers;

	sf::Sound dummy;
	std::vector<std::tuple<size_t, float, sf::Sound>> track;
	
	std::istringstream iss(line);
	std::string instruction;
	while (iss >> instruction)
	{
		std::tuple<size_t, float, sf::Sound> songEvent = std::make_tuple(0, 0.0, dummy);
		
		if (instruction.substr(0, 3) == "[R]")
		{
			std::string filename = "Instruments\\Rhythm\\" + instruction.substr(3, std::string::npos);
			filename += ".wav";

			if(!soundBuffers["R"].loadFromFile(filename))
				std::cout << "File " << filename << ".wav " << "not available." << std::endl;
		}
		else if (instruction.substr(0, 3) == "[M]")
		{
			soundBuffers = getNotes("Instruments\\" + instruction.substr(3, std::string::npos) + "\\");
		}
		else
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

void playSong(std::string filename, float bpm)
{
	std::vector<std::string> songTracks;
	std::ifstream songfile(filename);
	std::string line;
	
	while (std::getline(songfile, line))
	{
		songTracks.push_back(line);
	}

	std::thread one(playTrack, songTracks[0], bpm);
	std::thread two(playTrack, songTracks[1], bpm);
	std::thread three(playTrack, songTracks[2], bpm);

	one.join();
	two.join();
	three.join();
}

void testFunc(sf::SoundBuffer & buffer)
{
	buffer.loadFromFile("Instruments\\Rhythm\\hihat-plain.wav");
}

int main()
{

	playSong("mary-had-a-little-lamb.txt", 120);

	//sf::SoundBuffer test2;
	//std::thread t(testFunc, test2);
	//
	//sf::SoundBuffer test;
	//test.loadFromFile("Instruments\\Rhythm\\kick-electro02.wav");
	//t.join();
	

	std::cout << "Press ENTER to exit";
	while(std::cin.get() != '\n') 

	return 0;
}