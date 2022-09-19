#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "External/SDL_mixer/include/SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path,int volume = 100);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0,int channelu = -1);

private:

	Mix_Music* music = nullptr;
	List<Mix_Chunk*> fx;
};

#endif // __ModuleAudio_H__