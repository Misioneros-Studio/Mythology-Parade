#ifndef __J1ELEMENTSANIMATION_H__
#define __J1ELEMENTSANIMATION_H__

#include "j1Module.h"
#include "j1Timer.h"
#include "j1Textures.h"
#include "SDL\include\SDL_rect.h"

enum which_animation {
	boomer,
	fade,
	up_and_fade,
	wipe
};

enum fade_step {
	none,
	intro,
	outro
};

class j1ElementsAnimation : public j1Module
{
public:
	//main functions
	j1ElementsAnimation();
	~j1ElementsAnimation();

	bool Start();
	bool PostUpdate();


public:
	//this enum indicates the situation of the transition/effect we are in
	fade_step ongoingstep[50];

	bool Transition(which_animation type, SDL_Texture* t_tex, SDL_Rect t_rect, float t_pos_x, float t_pos_y, float time = 2);

	//the following calculates the percentatge of the transition that is done
	float LerpValue(float percent, float start, float end);


private:
	which_animation w_anim = which_animation::boomer;

	//used for calculating the percentage of screen doing the effect of the transition
	//we calculate it using the time of the transition and position of our rects that we create during it
	float percentatge[50];
	float globaltime[50];
	j1Timer timer[50];
	SDL_Texture** t_textures;
	SDL_Rect t_rects[50];
	float t_pos_x[50];
	float t_pos_y[50];
	float t_start_y[50];
	int many_objects = 0;

	float normalized[50];
};

#endif __J1ELEMENTSANIMATION_H__