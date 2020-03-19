#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Scene.h"

#include"QuadTree.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if (App->map->Load("MainMap.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		SDL_ShowCursor(0);

		RELEASE_ARRAY(data);
	}

	ui_ingame=(ImageUI*)App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,590,1280,130 }, { 0,590,1280,130 });


	debug_tex = App->tex->Load("maps/path2.png");
	cursor_tex = App->tex->Load("gui/cursors.png");



	iPoint position;
	iPoint size;
	position = App->map->WorldToMap(0, 0);
	size = iPoint(App->map->data.width * App->map->data.tile_width, App->map->data.height * App->map->data.tile_height);
	quadTree = new QuadTree(TreeType::ISOMETRIC, position.x + (App->map->data.tile_width / 2), position.y, size.x, size.y);
	quadTree->baseNode->SubDivide(quadTree->baseNode, 4);
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---

	// -------
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ActivateOrDeactivatePauseMenu();

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(700.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(700.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(700.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(700.0f * dt);


	App->map->Draw();


	//Quad draw
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->input->drawDebug = !App->input->drawDebug;

	if (App->input->drawDebug)
		App->render->DrawQuadTree(quadTree->type, quadTree->baseNode);



	int x, y;
	//iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	//p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
	//	App->map->data.width, App->map->data.height,
	//	App->map->data.tile_width, App->map->data.tile_height,
	//	App->map->data.tilesets.count(),
	//	map_coordinates.x, map_coordinates.y);
	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	//Show cursor ------------------------------
	SDL_Rect sec = { 0, 0, 54, 45 };
	p = App->render->ScreenToWorld(x, y);
	App->render->Blit(cursor_tex, p.x, p.y, &sec);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->tex->UnLoad(debug_tex);
	App->tex->UnLoad(cursor_tex);

	quadTree->Clear();

	return true;
}

// Called when clicking esc
void j1Scene::ActivateOrDeactivatePauseMenu() {
	if (ui_pause_window == nullptr) {
		ui_pause_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,300,459,168 }, { 216,18,459,168 });
		ui_button = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,360,237,38 }, { 787,240,237,38 }, "EXIT", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 610,372,237,38 }, {0,0,100,100},"EXIT");
	}
	else {
		App->gui->DeleteUIElement(ui_pause_window);
		App->gui->DeleteUIElement(ui_button);
		App->gui->DeleteUIElement(ui_text);
		ui_text = nullptr;
		ui_button = nullptr;
		ui_pause_window = nullptr;
	}
}

void j1Scene::OnClick(UI* element, float argument)
{

	switch (element->type)
	{

	case Type::BUTTON:

		if (element->name == (p2SString)"EXIT")
		{
			exitGame = true;
		}
		break;


	default:
		break;
	}


}
