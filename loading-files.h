/*
loading-files.h
Mark Underwood
Contains function declarations for loading-files.cpp
*/


#ifndef FILE_LOADING-FILES_H_INCLUDED
#define FILE_LOADING_FILES_H_INCLUDED

#include <SFML\Audio\SoundBuffer.hpp>
#include <string>
#include <map>

sf::SoundBuffer getWavFile(std::string filename);
std::map<std::string, sf::SoundBuffer> getNotes(std::string fileLocation);
std::map<std::string, sf::SoundBuffer> getSongSounds(std::string & track);

#endif // !FILE_LOADINGFILES_H_INCLUDED

