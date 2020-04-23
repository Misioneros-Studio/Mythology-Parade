#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1LogoScene.h"
#include"j1Audio.h"
#include"j1FadeToBlack.h"


j1LogoScene::j1LogoScene() : j1Module()
{
	name.append("logo_scene");
	active = true;
}

// Destructor
j1LogoScene::~j1LogoScene()
{}

// Called before the first frame
bool j1LogoScene::Start()
{
	timer_logo.Start();
	debug_tex = App->tex->Load("gui/Logo.png");
	sfx_logo= App->audio->LoadFx("audio/titlescene/introscene2.wav");
	App->audio->PlayFx(1,sfx_logo);
	return true;
}

// Called each loop iteration
bool j1LogoScene::PostUpdate()
{
	if (timer_logo.ReadSec() >= 1.5f) {
		SDL_Rect sec2 = { 543, 0, 301, 285 };
		App->render->Blit(debug_tex, 368, 217, &sec2,0.0f,angle++);
	}

	if (timer_logo.ReadSec() >= 0.5f) {
		SDL_Rect sec1 = { 0, 0, 543, 285 };
		App->render->Blit(debug_tex, 368, 217, &sec1);
	}

	if (timer_logo.ReadSec() >= 5) {
		ChangeToTitleScene();
	}

	return true;
}

// Called before quitting
bool j1LogoScene::CleanUp()
{
	App->tex->UnLoad(debug_tex);
	return true;
}

void j1LogoScene::ChangeToTitleScene()
{
	destroy = true;
	App->fade_to_black->FadeToBlack(which_fade::logo_to_title, 2);
}