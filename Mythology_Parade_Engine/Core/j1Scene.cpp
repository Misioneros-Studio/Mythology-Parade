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
#include "Animation.h"
#include "j1Scene.h"

#include"QuadTree.h"

j1Scene::j1Scene() : j1Module()
{
	name.append("scene");
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
	for (int i = 0; i < 3; i++) {
		ui_text_ingame[i] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 104,610+(i*33),237,38 }, { 0,0,100,100 }, "999", { 255,255,255,255 }, { 1,0,0,0 });
	}
	for (int i = 0; i < 8; i++) {
		if (i != 7) {
			ui_button[i] = nullptr;
		}
		ui_text[i] = nullptr;
	}

	debug_tex = App->tex->Load("maps/path2.png");
	cursor_tex = App->tex->Load("gui/cursors.png");


	//Eudald: This shouldn't be here but we don't have an entity system to load each animation yet
	App->animation->Load("assets/units/Assassin.tmx");
		

	iPoint position;
	iPoint size;
	position = App->map->WorldToMap(0, 0);
	size = iPoint(App->map->data.width * App->map->data.tile_width, App->map->data.height * App->map->data.tile_height);
	quadTree = new QuadTree(TreeType::ISOMETRIC, position.x + (App->map->data.tile_width / 2), position.y, size.x, size.y);
	quadTree->baseNode->SubDivide(quadTree->baseNode, 5);
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
		ActivatePauseMenu();

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(1000.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(1000.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(1000.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(1000.0f * dt);


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
	y += 16;
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (IN_RANGE(p.x, 0, App->map->data.width-1) == 1 && IN_RANGE(p.y, 0, App->map->data.height-1) == 1)
	{
		p = App->map->MapToWorld(p.x, p.y);
		App->render->Blit(debug_tex, p.x, p.y);
		App->render->Blit(debug_tex, p.x - 32, p.y, { 128, 64 });
	}

	std::list<iPoint> path = *App->pathfinding->GetLastPath();

	if (path.size() != 0) 
	{
		for (std::list<iPoint>::iterator it = path.begin(); it != path.end(); it++)
		{
			iPoint pos = App->map->MapToWorld(it->x, it->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	//Show cursor ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	SDL_Rect sec = { 0, 0, 54, 45 };
	p = App->render->ScreenToWorld(x, y);
	App->render->Blit(cursor_tex, p.x, p.y, &sec);

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
void j1Scene::ActivatePauseMenu() {
	if (ui_pause_window == nullptr) {
		ui_pause_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,100,459,531 }, { 216,21,459,531 });
		ui_button[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,160,237,38 }, { 787,240,237,38 }, "SAVE", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,172,237,38 }, { 0,0,100,100 }, "Save Game", {0,0,0,255});
		ui_button[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,220,237,38 }, { 787,240,237,38 }, "LOAD", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,232,237,38 }, { 0,0,100,100 }, "Load Game", { 0,0,0,255 });
		ui_button[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,280,237,38 }, { 787,240,237,38 }, "OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 613,292,237,38 }, { 0,0,100,100 }, "Options", { 0,0,0,255 });
		ui_button[3] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,340,237,38 }, { 787,240,237,38 }, "RESTART", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 614,352,237,38 }, { 0,0,100,100 }, "Restart", { 0,0,0,255 });
		ui_button[4] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,400,237,38 }, { 787,240,237,38 }, "SURRENDER", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,412,237,38 }, { 0,0,100,100 }, "Surrender");
		ui_button[5] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,460,237,38 }, { 787,240,237,38 }, "EXIT", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 625,472,237,38 }, { 0,0,100,100 }, "Exit", { 0,0,0,255 });
		ui_button[6] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,560,237,38 }, { 787,240,237,38 }, "CLOSE", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 620,572,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,112,237,38 }, { 0,0,100,100 }, "PAUSE", { 255,255,255,255 }, { 1,0,0,0 });
	}
}

// Called when clicking close button in pause menu
void j1Scene::DeactivatePauseMenu() {
	if (ui_pause_window != nullptr) {
		App->gui->DeleteUIElement(ui_pause_window);
		ui_pause_window = nullptr;
		for (int i = 7; i >= 0; i--) {
			if (i != 7) {
				if (ui_button[i] != nullptr) {
					App->gui->DeleteUIElement(ui_button[i]);
					ui_button[i] = nullptr;
				}
			}
			if (ui_text[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text[i]);
				ui_text[i] = nullptr;
			}
		}
	}
}




// Called when clicking options button in pause menu
void j1Scene::ActivateOptionsMenu() {
	if (ui_options_window == nullptr) {
		ui_options_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 });
		ui_button_options = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text_options[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,312,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text_options[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,212,237,38 }, { 0,0,100,100 }, "OPTIONS", { 255,255,255,255 }, { 1,0,0,0 });
	}
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking close button in options menu
void j1Scene::DeactivateOptionsMenu() {
	if (ui_options_window != nullptr) {
		App->gui->DeleteUIElement(ui_options_window);
		ui_options_window = nullptr;
		if (ui_button_options != nullptr) {
			App->gui->DeleteUIElement(ui_button_options);
			ui_button_options = nullptr;
		}
		for (int i = 1; i >= 0; i--) {
			if (ui_text_options[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_options[i]);
				ui_text_options[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}
}


void j1Scene::OnClick(UI* element, float argument)
{

	switch (element->type)
	{

	case Type::BUTTON:

		if (element->name == "SAVE")
		{
			App->SaveGame("save_game.xml");
		}
		else if (element->name == "LOAD")
		{
			App->LoadGame("save_game.xml");
		}
		else if (element->name == "OPTIONS")
		{
			ActivateOptionsMenu();
		}
		else if (element->name == "CLOSE OPTIONS")
		{
			DeactivateOptionsMenu();
		}
		else if (element->name == "EXIT")
		{
			exitGame = true;
		}
		else if (element->name == "CLOSE")
		{
			DeactivatePauseMenu();
		}
		break;


	default:
		break;
	}


}
