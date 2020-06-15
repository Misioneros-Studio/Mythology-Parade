// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
	paused = false;
	paused_at = 0;
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	if(paused==false)
		return SDL_GetTicks() - started_at;
	else
		return paused_at - started_at;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	
	if (paused == false)
		return float(SDL_GetTicks() - started_at) / 1000.0f;
	else
		return float(paused_at - started_at) / 1000.0f;
	
}

void j1Timer::Pause() 
{
	if (paused == false) {
		paused = true;
		paused_at = SDL_GetTicks();
	}
}

void j1Timer::Resume()
{
	if (paused == true) {
		paused = false;
		started_at += SDL_GetTicks() - paused_at;
		paused_at = 0;
	}
}

bool j1Timer::isPaused()
{
	return paused;
}

bool j1Timer::StartAt(int var)
{
	started_at = started_at - var;
	Resume();
	return true;
}
