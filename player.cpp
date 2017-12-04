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
#include <SFML\Audio.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include "loading-files.h"
#include "track.h"

using bufferMapType = std::map<std::string, sf::SoundBuffer>;
sf::Clock sync;

void playTrack(std::string & line, bufferMapType & soundBuffers, float bpm)
{							
	sf::Sound dummy;
	Track track(bpm);
	
	std::istringstream iss(line);
	std::string instruction;
	while (iss >> instruction)
	{
		std::tuple<size_t, float, sf::Sound> songEvent = std::make_tuple(0, 0.0, dummy);
	
		track.parseAction(songEvent, soundBuffers, instruction);
		track.insert(songEvent);
	}

	while (sync.getElapsedTime() < sf::seconds(1)) {}
	track.play();
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
		std::cout << "ERROR: File Extension Incorrect" << std::endl;
		return;
	}
	std::vector<std::string> songTracks;
	std::vector<bufferMapType> buffersForEachTrack;
	
	assignBuffers(filename, songTracks, buffersForEachTrack);

	std::vector<std::thread> allThreads(songTracks.size());

	for (size_t i = 0; i < songTracks.size(); ++i)
	{
		allThreads[i] = std::thread(playTrack, songTracks[i], buffersForEachTrack[i], bpm);
	}
	
	for (size_t i = 0; i < songTracks.size(); ++i)
	{
		allThreads[i].join();
	}
}

int main()
{

	sf::Font impact;
	if (!impact.loadFromFile("impact.ttf"))
	{
		std::cout << "ERROR: Could not load font" << std::endl;
	}

	sf::Font times;
	if (!times.loadFromFile("times.ttf"))
	{
		std::cout << "ERROR: Could not load font" << std::endl;
	}

	sf::RectangleShape enter(sf::Vector2f(100, 50));
	enter.setPosition(550, 275);
	enter.setOutlineThickness(2);
	enter.setOutlineColor(sf::Color(200, 200, 200, 255));

	sf::RectangleShape textBox(sf::Vector2f(400, 40));
	textBox.setPosition(80, 280);
	textBox.setOutlineThickness(2);
	textBox.setOutlineColor(sf::Color(200, 200, 200, 255));

	sf::Text pipe;
	pipe.setFont(times);
	pipe.setFillColor(sf::Color::Black);
	pipe.setPosition(80, 280);
	pipe.setString("|");
	int pipeDistance = 80;
	sf::Clock blinkPipe;

	sf::RenderWindow window(sf::VideoMode(800, 600), "MIDI-Like Music Player");
	while (window.isOpen())
	{
		bool onEnter = sf::Mouse::getPosition(window).x > 550 && sf::Mouse::getPosition(window).y > 275
						&& sf::Mouse::getPosition(window).x < 650 && sf::Mouse::getPosition(window).y < 325;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (onEnter)
			{
				enter.setFillColor(sf::Color(200,0,0));
			}
			else if (!onEnter)
			{
				enter.setFillColor(sf::Color::Red);
			}

			if (event.type == sf::Event::TextEntered)
			{
				blinkPipe.restart();
				pipeDistance += pipe.getCharacterSize();
				pipe.setPosition(sf::Vector2f(pipeDistance, 280));
			}

			if (onEnter && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				playSong("immigrantsong.ml", 112);
			}
		}

		window.clear(sf::Color(100, 100, 100, 255));

		window.draw(enter);

		window.draw(textBox);

		sf::Text enterLabel;
		enterLabel.setFont(impact);
		enterLabel.setFillColor(sf::Color::Black);
		enterLabel.setPosition(568, 283);
		enterLabel.setString("Play");
		window.draw(enterLabel);

		if (blinkPipe.getElapsedTime() < sf::seconds(1))
		{
			window.draw(pipe);
		}
		else if (blinkPipe.getElapsedTime() > sf::seconds(1) && blinkPipe.getElapsedTime() < sf::seconds(2))
		{

		}
		else if (blinkPipe.getElapsedTime() > sf::seconds(2))
		{
			blinkPipe.restart();
		}
		
		window.display();
	}

	return 0;
}