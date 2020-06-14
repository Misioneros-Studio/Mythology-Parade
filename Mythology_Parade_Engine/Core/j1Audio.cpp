#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "AssetsManager.h"

#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.append("audio");
	Building_destruction = Building_placed = Death_sfx = Decrease_Faith = Getting_resources = Increase_faith = 0;
	nullptrs = 0;
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}
	active = true;
	//LoadingFX
	Building_destruction = LoadFx("audio/fx/Building_destruction.wav");
	Building_placed =LoadFx("audio/fx/BuildingPlaced.wav");
	Decrease_Faith =LoadFx("audio/fx/Deacrease_FAITH.wav");
	Getting_resources =LoadFx("audio/fx/Getting_Resources.wav");
	hit_1 = App->audio->LoadFx("audio/fx/hit_1.wav");
	increase_prayers =LoadFx("audio/fx/Increase_prayers.wav");
	increase_sacrifice = LoadFx("audio/fx/Increase_sacrifice.wav");
	Walking_troops =LoadFx("audio/fx/Walking_troop.wav");
	Walking_troop2 = LoadFx("audio/fx/Walking_troop2.wav");

	OpenMenu_sfx = LoadFx("audio/ui/Close_Menu.wav");

	hit_2 = LoadFx("audio/fx/hit2.wav");
	UpgradeUnit = LoadFx("audio/fx/UpgradeUnit.wav");
	Death_sfx = LoadFx("audio/fx/Death.wav");

	return ret;
}


bool j1Audio::PostUpdate()
{
	if (a_actual_change == which_audio_fade::none)
		return true;

	float now = a_timer.ReadSec();

	if (a_actual_change == which_audio_fade::fade_out) {

		Mix_FadeOutMusic((int)(a_total_time * 1000.0f));

		if (a_timer.ReadSec() >= a_total_time)
		{
			a_actual_change = which_audio_fade::none;
		}
	}

	return true;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (std::list<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end(); it++)
	{
		Mix_FreeChunk(it._Ptr->_Myval);
	}

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS_RW(App->assets_manager->Load(path), 1);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(App->assets_manager->Load(path), 1);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		if (nullptrs == 0) {
			fx.push_back(chunk);
			ret = fx.size();
		}
		else {
			int i = 0;
			bool finish = false;
			for (std::list<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end() && finish == false; it++) {
				if (it._Ptr->_Myval == nullptr) {
					finish = true;
					ret = i + 1;
					nullptrs--;
					it._Ptr->_Myval = chunk;
				}
				i++;
			}
		}
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(int channel, unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.size())
	{

		std::list<Mix_Chunk*>::iterator it = fx.begin();
		std::advance(it, id - 1);

		if(it._Ptr->_Myval != fx.end()._Ptr->_Myval)
			Mix_PlayChannel(channel, it._Ptr->_Myval, repeat);
	}

	return ret;
}

// Clean all fxs to change scene

bool j1Audio::CleanFxs() {
	bool ret = false;

	if (!active)
		return false;
	Mix_HaltChannel(-1);
	for (std::list<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end(); it++)
	{
		Mix_FreeChunk(it._Ptr->_Myval);
	}
	nullptrs = 0;
	fx.clear();

	return ret;
}

bool j1Audio::CleanFxs(int fx_to_delete)
{
	bool ret = false;

	if (!active || fx.size() == 0)
		return false;
	Mix_HaltChannel(-1);

	std::list<Mix_Chunk*>::iterator it = fx.begin();
	std::advance(it, fx_to_delete - 1);
	Mix_FreeChunk(it._Ptr->_Myval);
	it._Ptr->_Myval = nullptr;
	nullptrs++;

	return ret;
}


void j1Audio::FadeAudio(which_audio_fade w_fade, float time, int volume) {

	a_actual_change = w_fade;
	a_timer.Start();
	a_total_time = time;
	volume_fade = volume;

	if (a_actual_change == which_audio_fade::change_volume) {
		Mix_VolumeMusic(volume_fade);
	}
}
// Change volume music
void j1Audio::ChangeVolumeMusic(float volume) {
	int volume_int = volume * 128;
	Mix_VolumeMusic(volume_int);
}

// Change volume fxs
void j1Audio::ChangeVolumeFx(float volume) {
	int volume_int = volume * 128;
	Mix_Volume(-1, volume_int);
}

// Get volume music
int j1Audio::GetVolumeMusic() {
	return Mix_VolumeMusic(-1);
}

// Get volume fxs
int j1Audio::GetVolumeFx() {
	return Mix_Volume(-1, -1);
}

void j1Audio::OnClick(UI* element, float volume)
{

	switch (element->type)
	{

	case Type::IMAGE:

		if (element->name.compare("VOLUME_CONTROL") == 0)
		{
			ChangeVolumeMusic(volume);
		}
		else if (element->name.compare("FX_CONTROL")==0)
		{
			ChangeVolumeFx(volume);
		}
		break;


	default:
		break;
	}

}

bool j1Audio::Save(pugi::xml_node& s) const
{
	pugi::xml_node node4 = s.append_child("volume");
	node4.append_attribute("music").set_value(App->audio->GetVolumeMusic());
	node4.append_attribute("fx").set_value(App->audio->GetVolumeFx());

	return true;
}

bool j1Audio::Load(pugi::xml_node& s)
{
	ChangeVolumeFx(s.child("volume").attribute("fx").as_float() / 128);
	ChangeVolumeMusic(s.child("volume").attribute("music").as_float() / 128);

	return true;
}
