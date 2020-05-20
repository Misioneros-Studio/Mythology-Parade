#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1TutorialScene.h"
#include "j1FadeToBlack.h"

#include"QuadTree.h"

j1TutorialScene::j1TutorialScene() : j1Module()
{
	name.append("tutorialscene");
}

// Destructor
j1TutorialScene::~j1TutorialScene()
{}

// Called before render is available
bool j1TutorialScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1TutorialScene::Start()
{
	App->render->camera.x = 578;
	App->render->camera.y = -1865;

	return true;
}

// Called each loop iteration
bool j1TutorialScene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1TutorialScene::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool j1TutorialScene::PostUpdate()
{

	return true;
}

// Called before quitting
bool j1TutorialScene::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(tutorial_tex);

	return true;
}