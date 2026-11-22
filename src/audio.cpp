#include "audio.h"

// initialize
bool Audio::init()
{
	if(ma_engine_init(NULL, & engine) != MA_SUCCESS)
	{
		return false;
	}
	engine_ready = true;
	return true;
}

// stop
void Audio::stop()
{
	if(music_loaded)
	{
		ma_sound_uninit(& music);
		music_loaded = false;
	}
}

// play a file
bool Audio::play(const std::string & path, bool loop)
{
	if(!engine_ready)
	{
		return false;
	}
	
	stop();
	
	if(ma_sound_init_from_file(& engine, path.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, & music) == MA_SUCCESS)
	{
		music_loaded = true;
		ma_sound_start(& music);
	}
	
	return ma_sound_start(& music) == MA_SUCCESS;
}
