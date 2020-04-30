#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Gui.h"
#include "j1Map.h"
#include "j1Minimap.h"
#include "j1Pathfinding.h"
#include "EntityManager.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1LogoScene.h"
#include "j1TitleScene.h"
#include "j1Scene.h"
#include "j1Minimap.h"
#include "j1Map.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "Console.h"
#include "EntityManager.h"

j1FadeToBlack::j1FadeToBlack()
{
	screen = { 0, 0, 1280,720 };
	actual_civilization = "";
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	actual_civilization = "";
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::PostUpdate()
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			switch (actual_change) {
			case(which_fade::logo_to_title):
				App->logo_scene->Disable();
				App->title_scene->Enable();
				break;
				
			case(which_fade::title_to_scene):
				App->title_scene->Disable();
				App->title_scene->destroy = false;
				App->entityManager->Enable();
				App->pathfinding->Enable();
				App->scene->Enable();
				App->minimap->Enable();

				break;

			case(which_fade::scene_to_title):
				App->entityManager->Disable();
				App->pathfinding->Disable();
				App->scene->Disable();
				App->minimap->Disable();
				App->title_scene->Enable();
				break;
			}

			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(which_fade fade2, float time, std::string civilization)
{

	bool ret = false;

	if (current_step == fade_step::none)
	{
		actual_civilization = "";

		actual_change = fade2;
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		if (civilization != "")
			actual_civilization = civilization;
		ret = true;
	}

	return ret;
}