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
#include "EntityManager.h"
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
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	App->scene->active = false;
	SDL_ShowCursor(0);
  
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	start_timer = false;

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

	faith_symbol = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,590,1280,130 });

	for (int i = 0; i < 3; i++) 
	{
		ui_text_ingame[i] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 104,610+(i*33),237,38 }, { 0,0,100,100 }, "9999", { 255,255,255,255 }, { 1,0,0,0 });
	}
	for (int i = 0; i < 8; i++) 
	{
		if (i != 7) 
		{
			ui_button[i] = nullptr;
		}
		ui_text[i] = nullptr;
	}

  //Load building debug textures
	debugBlue_tex = App->tex->Load("maps/path2.png");
	debugRed_tex = App->tex->Load("maps/cantBuild.png");

	App->audio->CleanFxs();

	App->gui->sfx_UI[(int)UI_Audio::SAVE] = App->audio->LoadFx("audio/ui/Save.wav");
	App->gui->sfx_UI[(int)UI_Audio::LOAD] = App->audio->LoadFx("audio/ui/load.wav");
	App->gui->sfx_UI[(int)UI_Audio::CONFIRMATION] = App->audio->LoadFx("audio/ui/Click_Standard2.wav");
	App->gui->sfx_UI[(int)UI_Audio::OPTIONS] = App->audio->LoadFx("audio/ui/Settings_Click.wav");
	App->gui->sfx_UI[(int)UI_Audio::RESTART] = App->audio->LoadFx("audio/ui/Restart.wav");
	App->gui->sfx_UI[(int)UI_Audio::SURRENDER] = App->audio->LoadFx("audio/ui/Surrender.wav");
	App->gui->sfx_UI[(int)UI_Audio::EXIT] = App->audio->LoadFx("audio/ui/Exit.wav");
	App->gui->sfx_UI[(int)UI_Audio::CLOSE] = App->audio->LoadFx("audio/ui/Close_Menu.wav");

	for (int i = 0; i < 4; i++)
	{
		if (i < 3)
		{
			ui_button_confirmation[i] = nullptr;
		}
		ui_text_confirmation[i] = nullptr;
	}
	confirmation_option = "";

	close_menus = CloseSceneMenus::None;

	cursor_tex = App->tex->Load("gui/cursors.png");

	winlose_tex = App->tex->Load("gui/WinLoseBackground.png");

	//iPoint position;
	//iPoint size;
	//position = App->map->WorldToMap(0, 0);
	//size = iPoint(App->map->data.width * App->map->data.tile_width, App->map->data.height * App->map->data.tile_height);
	//quadTree = new QuadTree(TreeType::ISOMETRIC, position.x + (App->map->data.tile_width / 2), position.y, size.x, size.y);
	//quadTree->baseNode->SubDivide(quadTree->baseNode, 5);
  

	//Eudald: This shouldn't be here but we don't have an entity system to load each animation yet
	App->animation->Load("assets/units/Assassin.tmx");
  
	App->audio->PlayMusic("audio/music/Ambient1.ogg");
  
	//Creating players
	App->entityManager->CreatePlayerEntity();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	//static iPoint origin;
	//static bool origin_selected = false;

	//iPoint p = App->map->GetMousePositionOnMap();

	//if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	if (origin_selected == true)
	//	{
	//		App->pathfinding->CreatePath(origin, p);
	//		origin_selected = false;
	//	}
	//	else
	//	{
	//		origin = p;
	//		origin_selected = true;
	//	}
	//}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---
	switch (close_menus)
	{
	case::CloseSceneMenus::Pause:
		DeactivatePauseMenu();
		close_menus = CloseSceneMenus::None;
		break;
	case::CloseSceneMenus::Options:
		DeactivateOptionsMenu();
		close_menus = CloseSceneMenus::None;
		break;
	case::CloseSceneMenus::Confirmation:
		DeactivateConfirmationMenu();
		close_menus = CloseSceneMenus::None;
		break;
	case::CloseSceneMenus::Confirmation_and_Pause:
		DeactivateConfirmationMenu();
		DeactivatePauseMenu();
		close_menus = CloseSceneMenus::None;
		break;
	}
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

	//if (App->input->drawDebug)
	//	App->render->DrawQuadTree(quadTree->type, quadTree->baseNode);

	iPoint p = App->map->GetMousePositionOnMap();

	if (IN_RANGE(p.x, 0, App->map->data.width-1) == 1 && IN_RANGE(p.y, 0, App->map->data.height-1) == 1)
	{
		//p = App->map->MapToWorld(p.x, p.y);
		//App->render->Blit(debug_tex, p.x, p.y);
		//App->render->Blit(debug_tex, p.x - 32, p.y, { 128, 64 });
	}

	std::list<iPoint> path = *App->pathfinding->GetLastPath();

	if (path.size() != 0) 
	{
		for (std::list<iPoint>::iterator it = path.begin(); it != path.end(); it++)
		{
			iPoint pos = App->map->MapToWorld(it->x, it->y);
			App->render->Blit(debugBlue_tex, pos.x, pos.y);
		}
	}

	if (App->entityManager->getPlayer()->player_win == true) {
		if (App->entityManager->getPlayer()->player_type == CivilizationType::VIKING) {
			DoWinOrLoseWindow(1, true);
		}
		else {
			DoWinOrLoseWindow(2, true);
		}
	}

	else if (App->entityManager->getPlayer()->player_lose == true) {
		if (App->entityManager->getPlayer()->player_type == CivilizationType::VIKING) {
			DoWinOrLoseWindow(1, false);
		}
		else {
			DoWinOrLoseWindow(2, false);
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


	App->tex->UnLoad(debugBlue_tex);
	App->tex->UnLoad(debugRed_tex);

	DeactivateConfirmationMenu();
	DeactivateOptionsMenu();
	DeactivatePauseMenu();
	App->gui->DeleteUIElement(ui_ingame);
	ui_ingame = nullptr;
	for (int i = 0; i < 3; i++)
	{
		App->gui->DeleteUIElement(ui_text_ingame[i]);
		ui_text_ingame[i] = nullptr;
	}

	App->tex->UnLoad(cursor_tex);


	//quadTree->Clear();

	return true;
}

// Called when clicking esc
void j1Scene::ActivatePauseMenu() {
	if (ui_pause_window == nullptr) {
		ui_pause_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,100,459,531 }, { 216,21,459,531 });
		ui_button[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,160,237,38 }, { 787,240,237,38 }, "SAVE", { 787,342,237,38 }, { 787,291,237,38 }, false, 
			{ 0,0,0,0 }, this,(int)UI_Audio::SAVE);
		ui_text[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,172,237,38 }, { 0,0,100,100 }, "Save Game", {0,0,0,255});
		ui_button[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,220,237,38 }, { 787,240,237,38 }, "LOAD", { 787,342,237,38 }, { 787,291,237,38 }, false, 
			{ 0,0,0,0 }, this, (int)UI_Audio::LOAD);
		ui_text[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,232,237,38 }, { 0,0,100,100 }, "Load Game", { 0,0,0,255 });
		ui_button[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,280,237,38 }, { 787,240,237,38 }, "OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false, 
			{ 0,0,0,0 }, this, (int)UI_Audio::OPTIONS);
		ui_text[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 613,292,237,38 }, { 0,0,100,100 }, "Options", { 0,0,0,255 });
		ui_button[3] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,340,237,38 }, { 787,240,237,38 }, "RESTART", { 787,342,237,38 }, { 787,291,237,38 }, false, 
			{ 0,0,0,0 }, this, (int)UI_Audio::RESTART);
		ui_text[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 614,352,237,38 }, { 0,0,100,100 }, "Restart", { 0,0,0,255 });
		ui_button[4] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,400,237,38 }, { 787,240,237,38 }, "SURRENDER", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, this, (int)UI_Audio::SURRENDER);
		ui_text[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,412,237,38 }, { 0,0,100,100 }, "Surrender");
		ui_button[5] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,460,237,38 }, { 787,240,237,38 }, "EXIT", { 787,342,237,38 }, { 787,291,237,38 }, false, 
			{ 0,0,0,0 }, this, (int)UI_Audio::EXIT);
		ui_text[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 625,472,237,38 }, { 0,0,100,100 }, "Exit", { 0,0,0,255 });
		ui_button[6] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,560,237,38 }, { 787,240,237,38 }, "CLOSE", { 787,342,237,38 }, { 787,291,237,38 }, false, 
			{ 0,0,0,0 }, this, (int)UI_Audio::CLOSE);
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

		ui_button_options = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE OPTIONS", { 787,342,237,38 }, 
			{ 787,291,237,38 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CLOSE);
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

// Called when clicking a button in the menu with confirmation message
void j1Scene::ActivateConfirmationMenu(std::string str) {
	if (ui_confirmation_window == nullptr) {
		ui_confirmation_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 });
		ui_button_confirmation[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 470,300,117,38 }, { 834,125,117,24 }, "YES", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION);
		ui_text_confirmation[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 509,306,237,38 }, { 0,0,100,100 }, "Yes", { 0,0,0,255 }, { 1,0,0,0 });
		ui_button_confirmation[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 690,300,117,38 }, { 834,125,117,24 }, "NO", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION);
		ui_text_confirmation[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 731,306,237,38 }, { 0,0,100,100 }, "No", { 0,0,0,255 }, { 1,0,0,0 });
		std::string text = str + " ?";
		ui_text_confirmation[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 463,212,237,38 }, { 0,0,100,100 }, "ARE YOU SURE YOU WANT TO", { 255,255,255,255 }, { 1,0,0,0 });
		int size = text.length();
		ui_text_confirmation[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 640 - (6 * size),247,237,38 }, { 0,0,100,100 }, text, { 255,255,255,255 }, { 1,0,0,0 });

	}
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking no in the confirmation message
void j1Scene::DeactivateConfirmationMenu() {
	confirmation_option.clear();
	if (ui_confirmation_window != nullptr) {
		App->gui->DeleteUIElement(ui_confirmation_window);
		ui_confirmation_window = nullptr;
		for (int i = 3; i >= 0; i--) {
			if (i < 2)
			{
				if (ui_button_confirmation[i] != nullptr) {
					App->gui->DeleteUIElement(ui_button_confirmation[i]);
					ui_button_confirmation[i] = nullptr;
				}
			}
			if (ui_text_confirmation[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_confirmation[i]);
				ui_text_confirmation[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}
}

void j1Scene::BackToTitleMenu() {
	App->change_scene = true;
	destroy = true;
	App->map->destroy = true;
	App->pathfinding->destroy = true;
	App->entityManager->destroy = true;
	App->audio->PlayFx(App->audio->ExitGame_transition);
}

void j1Scene::OnClick(UI* element, float argument)
{

	switch (element->type)
	{

	case Type::BUTTON:

		if (element->name == "SAVE")
		{
			confirmation_option = "SAVE";
			ActivateConfirmationMenu("SAVE THE GAME");
		}
		else if (element->name == "LOAD")
		{
			confirmation_option = "LOAD";
			ActivateConfirmationMenu("LOAD THE GAME");
		}
		else if (element->name == "OPTIONS")
		{
			ActivateOptionsMenu();
		}
		else if (element->name == "CLOSE OPTIONS")
		{
			close_menus = CloseSceneMenus::Options;
		}
		else if (element->name == "RESTART")
		{
			confirmation_option = "RESTART";
			ActivateConfirmationMenu("RESTART");
		}
		else if (element->name == "SURRENDER")
		{
			confirmation_option = "SURRENDER";
			ActivateConfirmationMenu("SURRENDER");
			//////TODO: HERE LOSE CONDITION WILL BE TRUE
		}
		else if (element->name == "EXIT")
		{
			confirmation_option = "EXIT";
			ActivateConfirmationMenu("EXIT");
		}
		else if (element->name == "NO")
		{
			close_menus = CloseSceneMenus::Confirmation;
		}
		else if (element->name == "YES")
		{
			close_menus = CloseSceneMenus::Confirmation;
			if (confirmation_option.compare("SAVE") == 0)
			{
				App->SaveGame("save_game.xml");
			}
			else if (confirmation_option.compare("LOAD") == 0)
			{
				App->LoadGame("save_game.xml");
			}
			else if (confirmation_option.compare("SURRENDER") == 0)
			{
				App->entityManager->getPlayer()->player_lose = true;
				close_menus = CloseSceneMenus::Confirmation_and_Pause;
			}
			else if (confirmation_option.compare("EXIT") == 0)
			{
				BackToTitleMenu();
			}
		}
		else if (element->name == "CLOSE")
		{
			close_menus = CloseSceneMenus::Pause;
		}
		break;


	default:
		break;
	}


}


void j1Scene::DoWinOrLoseWindow(int type, bool win) {
	SDL_Rect sec_viking = { 0, 0,807, 345 };
	SDL_Rect sec_greek = { 0, 345,807, 345 };

	SDL_Rect sec_win = { 807, 0,807, 345 };
	SDL_Rect sec_lose = { 807, 345,807, 345 };

	if (start_timer == false)
		timer_win_lose.Start();

	start_timer = true;

	if (type == 1) {
		if (win == true) {
			App->render->Blit(winlose_tex, 230, 100, &sec_win);
			App->render->Blit(winlose_tex, 230, 100, &sec_viking);
			App->audio->PlayFx(App->audio->WinVikings_sound);
		}
		else {
			App->render->Blit(winlose_tex, 230, 100, &sec_greek);
			App->render->Blit(winlose_tex, 230, 100, &sec_lose);
			App->audio->PlayFx(App->audio->Lose_Sound);
		}
	}

	if (type == 2) {
		if (win == true) {
			App->render->Blit(winlose_tex, 230, 100, &sec_greek);
			App->render->Blit(winlose_tex, 230, 100, &sec_lose);
			App->audio->PlayFx(App->audio->WinGreeks_sound);
		}
		else {
			App->render->Blit(winlose_tex, 230, 100, &sec_viking);
			App->render->Blit(winlose_tex, 230, 100, &sec_win);
			App->audio->PlayFx(App->audio->Lose_Sound);
		}
	}
	if (timer_win_lose.ReadSec() >= 5) {
		BackToTitleMenu();
	}
}