/*
parsing-input.h
Mark Underwood
Definitions for a struct and functions in parsing-input.cpp
*/

#ifndef FILE_PARSING-INPUT_H
#define FILE_PARSING_INPUT_H
#include <SFML\Audio\Sound.hpp>
#include <tuple>
#include <map>
#include <string>

struct tempo;
tempo setTempo(float bpm);
void parseAction(std::tuple<size_t, float, sf::Sound> & songEvent,
				 std::map<std::string, sf::SoundBuffer> & buffers, std::string & instruction, float bpm);

#endif // !FILE_PARSING-INPUT_H

