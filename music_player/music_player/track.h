
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

	void play();
	void parseAction(std::tuple<size_t, float, sf::Sound> & songEvent, bufferMapType & buffers,
					 std::string & instruction);
	void insert(std::tuple<size_t, float, sf::Sound> &);

private:
	void _playSound(sf::Sound sound, float secs);
	void _rest(sf::Sound dummy, float secs);

	float _wholeNote;
	float _halfNote;
	float _quarterNote;
	float _eighthNote;
	float _sixteenthNote;
	std::vector<std::tuple<size_t, float, sf::Sound>> _track;
};

#endif // !FILE_TRACK_H_INCLUDED

