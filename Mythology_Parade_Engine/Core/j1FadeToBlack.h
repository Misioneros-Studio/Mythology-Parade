#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool PostUpdate();

	bool FadeToBlack(j1Module* module_off, j1Module* module_on, bool is_changing_to_scene=false, float time = 2.0f);


public:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;
private:

	j1Module* module_on = nullptr;
	j1Module* module_off = nullptr;
	bool changing_to_scene = false;



	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__MODULEFADETOBLACK_H__