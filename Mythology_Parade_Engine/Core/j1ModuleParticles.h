#pragma once

#ifndef __J1MODULEPARTICLES__
#define __J1MODULEPARTICLES__

#include "j1Module.h"
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

private:
	std::list<j1Emiter> particleArray;
	j1ParticleSystem* particleSystem;

	float dtime;

	SDL_Rect rect;
	SDL_Rect rect2;
};

#endif //__J1MODULEPARTICLES__