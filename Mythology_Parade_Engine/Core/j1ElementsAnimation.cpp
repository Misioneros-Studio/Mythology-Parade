#include <math.h>
#include "j1App.h"
#include "j1ElementsAnimation.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Window.h"

j1ElementsAnimation::j1ElementsAnimation()
{

}

j1ElementsAnimation::~j1ElementsAnimation()
{}

// Load assets
bool j1ElementsAnimation::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1ElementsAnimation::PostUpdate()
{
	for (int i = 0; i <= many_objects; i++) {

		if (ongoingstep[i] == fade_step::none)
			continue;

		normalized[i] = MIN(1.0f, (float)timer[i].ReadSec() / (float)globaltime[i]);

		switch (ongoingstep[i])
		{
		case fade_step::intro:
		{
			//the following switch decides which animation we're doing
			switch (w_anim) {

				//FADE TO BLACK TRANSITION
			case(which_animation::fade):
				percentatge[i] = timer[i].ReadSec() * (1 / (globaltime[i]));
				//App->render->Blit(t_textures[i],t_pos_x[i], t_pos_y[i], &t_rects[i]);
				break;

				//FADE TO WHITE TRANSITION
			case(which_animation::up_and_fade):
				percentatge[i] = timer[i].ReadSec() * (1 / (globaltime[i]));
				t_pos_y[i] = LerpValue(percentatge[i], t_start_y[i], t_start_y[i]+100);
				break;

				//WIPE TRANSITION
			case (which_animation::wipe):
				/*
				if (ongoingstep == fade_step::entering) {
					percentatge = timer.ReadSec() * (1 / (globaltime));
					float normalized_x_position = LerpValue(percentatge, -(int)App->win->GetWidth(), 0);

					if (normalized_x_position >= 0) {
						WipeTransRect.x = 0;
					}
					else WipeTransRect.x = normalized_x_position;
				}
				SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(App->render->renderer, &WipeTransRect);
				*/
				break;
			}
			if (timer[i].ReadSec() >= globaltime[i])
			{
				timer[i].Start();
				ongoingstep[i] = fade_step::none;
			}
		} break;

		case fade_step::outro:
		{
			normalized[i] = 1.0f - normalized[i];

			if (timer[i].ReadSec() >= globaltime[i])
				ongoingstep[i] = fade_step::none;
		} break;
		}
	}

	if (many_objects >= 1) {
		for (int j = 0; j <= many_objects; j++) {
			App->render->Blit(t_textures[j], t_pos_x[j], t_pos_y[50], &t_rects[j]);
		}
	}

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1ElementsAnimation::Transition(which_animation type, SDL_Texture* t_tex, SDL_Rect t_rect, float t_pos_x, float t_pos_y, float time)
{
	bool ret = false;

	w_anim = type;

	if (ongoingstep[many_objects] == fade_step::none)
	{
		many_objects++;
		ongoingstep[many_objects] = fade_step::intro;

		percentatge[many_objects] = 0;
		globaltime[many_objects] = time;
		timer[many_objects].Start();

		t_rects[many_objects] = t_rect;
		t_textures[many_objects] = t_tex;
		this->t_pos_x[many_objects] = t_pos_x;
		this->t_pos_y[many_objects] = t_pos_y;
		this->t_start_y[many_objects] = t_pos_y;


		ret = true;
	}

	return ret;
}

float j1ElementsAnimation::LerpValue(float percent, float start, float end)
{
	return start + percent * (end - start);
}