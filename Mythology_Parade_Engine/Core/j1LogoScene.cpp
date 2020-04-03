#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1LogoScene.h"
#include"j1Audio.h"


j1LogoScene::j1LogoScene() : j1Module()
{
	name.append("intro_scene");
	active = true;
}

// Destructor
j1LogoScene::~j1LogoScene()
{}

// Called before the first frame
bool j1LogoScene::Start()
{
	timer_logo.Start();
	debug_tex = App->tex->Load("gui/TitleAssets.png");
	App->audio->PlayMusic("audio/music/MainTitle_Use.ogg");
	return true;
}

// Called each loop iteration
bool j1LogoScene::Update(float dt)
{
	SDL_Rect sec2 = { 0, 0, 1280, 720 };
	App->render->Blit(debug_tex, 0, 0, &sec2);

	if (timer_logo.ReadSec() >= 5) {
		ChangeToTitleScene();
	}

	return true;
}

// Called before quitting
bool j1LogoScene::CleanUp()
{

	return true;
}

void j1LogoScene::ChangeToTitleScene()
{


}