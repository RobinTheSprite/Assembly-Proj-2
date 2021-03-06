
#ifndef FILE_TRACK_H_INCLUDED
#define FILE_TRACK_H_INCLUDED
#include <vector>
#include <functional>
#include <map>
#include <SFML\Audio.hpp>

class Track
{
public:
	using bufferMapType = std::map<std::string, sf::SoundBuffer>;
	Track();
	Track(float bpm);

	void play(sf::Clock &);
	void parseAction(std::tuple<size_t, float, sf::Sound> & songEvent, bufferMapType & buffers,
					 std::string & instruction);
	void push_back(std::tuple<size_t, float, sf::Sound> &);

private:

	float _wholeNote;
	float _halfNote;
	float _quarterNote;
	float _eighthNote;
	float _sixteenthNote;
	float _timeDistance;
	std::vector<std::tuple<size_t, float, sf::Sound>> _track;
};

#endif // !FILE_TRACK_H_INCLUDED

