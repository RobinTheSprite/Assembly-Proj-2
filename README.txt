MIDI-Like Music Player

This program takes special instructions from a text file, reads them like sheet music, 
and plays the song in multiple threads. Each line of the text file is one track of music,
using one instrument each. Some of these tracks are marked as rhythm ([R]), meaning that they only play
one sound, like a drumbeat. Some are melody tracks ([M]), meaning they can play several different sounds in a musical scale.
Currently there is only one melody instrument, which I call PureSine, which has seven notes of the C Major scale 
available. 

Included in this rough draft in addition to the base code a text file playing "mary had a little lamb," 
and the .wav files that the song is constructed from. Place the Instruments folder in a directory where 
the source files can find it. The same goes for mary-had-a-little-lamb.txt.

Note that SFML is required to compile the program.

Things to (potentially) work on:

	-Introduce a Song class that will carry around important info and be capable of
  	 globally modifying the song in some way, like a key change or tempo change.

	-Write some of this in assembly. Currently none of the code is in assembly, following the rationale
 	 that I really wish to do as little programming in assembly as possible.

	-Make the code generally more error-safe. Almost goes without saying.

	-Find a nice tool to make a GUI. I am getting a little tired of writing CLIs for
 	 projects like these.

Let me know in your comments on this draft which of these sound like the best ideas,
or if you have any ideas of your own. 