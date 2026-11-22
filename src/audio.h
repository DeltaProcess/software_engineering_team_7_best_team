#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include "../lib/miniaudio-0.11.25/miniaudio.h"

struct ma_engine;
struct ma_sound;

class Audio
{
	public:
		bool init();
		bool play(const std::string & path, bool loop = false);
		void stop();
	
	private:
		ma_engine engine;
		ma_sound music;
		bool engine_ready = false;
		bool music_loaded = false;
};

#endif
