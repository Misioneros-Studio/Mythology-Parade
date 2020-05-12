#pragma once

#ifndef __J1MODULEPARTICLES__
#define __J1MODULEPARTICLES__

#include "j1Module.h"
#include "ClassicAnimation.h"
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#include <stdlib.h>

class j1ParticleSystem;
class j1Emiter;

class j1ModuleParticles : public j1Module
{
public:
	j1ModuleParticles(bool start_enabled = true);
	~j1ModuleParticles();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void DeleteAllParticles();
	void DoUnitsPathParticles(int pos_x, int pos_y);

private:
	j1ParticleSystem* particleSystem;

	float dtime;

	SDL_Rect rect;
	SDL_Rect rect2;

	SDL_Rect arrow_rect;
	SDL_Texture* particles_tex;
	ClassicAnimation arrow_animation;
};

#endif //__J1MODULEPARTICLES__