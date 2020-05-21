#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "j1Timer.h"
#include "SDL_mixer/include/SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define MAX_VOLUME 150
#define MAX_SFX 80

struct _Mix_Music;
struct Mix_Chunk;

enum which_audio_fade {
	none,
	fade_in,
	fade_out,
	change_volume
};

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(int channel, unsigned int fx, int repeat = 0);

	// Clean all fxs to change scene
	bool CleanFxs();

	void FadeAudio(which_audio_fade w_fade, float time = 2.0f, int volume=50);

	// Get & Change volume fxs
	int GetVolumeFx();
	void ChangeVolumeFx(float);

	// Get & Set volume music
	int GetVolumeMusic();
	void ChangeVolumeMusic(float);

	void OnClick(UI*, float);

private:

	_Mix_Music*			music = NULL;
	int sfxList[MAX_SFX];
	std::list<Mix_Chunk*>	fx;
	float MusicVolume;

	j1Timer a_timer;
	float a_total_time;
	int volume_fade;

public:
	enum afade_step
	{
		boomer,
		in,
		out
	} acurrent_step = afade_step::boomer;

	which_audio_fade a_actual_change = which_audio_fade::none;
};

#endif // __j1AUDIO_H__