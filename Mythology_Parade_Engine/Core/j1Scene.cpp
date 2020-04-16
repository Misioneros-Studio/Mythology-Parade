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
#include "j1Minimap.h"
#include "CombatUnit.h"
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

		mapLimitsRect = App->map->GetMapRect();
		App->pathfinding->maxPathLenght = App->map->GetMapMaxLenght();

		SDL_ShowCursor(0);

		RELEASE_ARRAY(data);
	}

	ui_ingame=(ImageUI*)App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,590,1280,130 }, { 0,590,1280,130 });


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
	for (int i = 0; i < 13; i++)
	{
		hud_list_troops[i] = nullptr;
		hud_number_troops[i] = nullptr;
		hud_stats_selected_troop[i] = nullptr;
	}
	hud_selected_troop = nullptr;

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

	paused_game = false;

	winlose_tex = App->tex->Load("gui/WinLoseBackground.png");

	//iPoint position;
	//iPoint size;
	//position = App->map->WorldToMap(0, 0);
	//size = iPoint(App->map->data.width * App->map->data.tile_width, App->map->data.height * App->map->data.tile_height);
	//quadTree = new QuadTree(TreeType::ISOMETRIC, position.x + (App->map->data.tile_width / 2), position.y, size.x, size.y);
	//quadTree->baseNode->SubDivide(quadTree->baseNode, 5);
  
	App->audio->PlayMusic("audio/music/Ambient1.ogg");
  
	//Creating players
	App->entityManager->CreatePlayerEntity();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		//TMP: Temporal pathfinding debug
		Entity* ent = App->entityManager->entities[EntityType::UNIT].begin()._Ptr->_Myval;
		iPoint origin = App->map->WorldToMap(ent->position.x, ent->position.y);
		iPoint ending = App->map->GetMousePositionOnMap();

		if (origin != ending)
			App->pathfinding->RequestPath(origin, ending);
	}
	if (App->pathfinding->pathfinderList[0].pathCompleted)
	{
		Unit* ent = (Unit*)App->entityManager->entities[EntityType::UNIT].begin()._Ptr->_Myval;
		ent->SetPath(*App->pathfinding->pathfinderList.begin()->GetLastPath());
		App->pathfinding->pathfinderList[0].pathCompleted = false;
	}


	// Move Camera if click on the minimap
	int mouse_x, mouse_y;
	if (((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) || (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)) && paused_game == false)
	{
		App->input->GetMousePosition(mouse_x, mouse_y);
		SDL_Rect minimap = { App->minimap->position.x +15, App->minimap->position.y + 7, App->minimap->width - 28, App->minimap->height -15};

		if ((mouse_x > minimap.x) && (mouse_x < minimap.x + minimap.w) && (mouse_y > minimap.y) && (mouse_y < minimap.y + minimap.h))
		{
			iPoint minimap_mouse_position = App->minimap->ScreenToMinimapToWorld(mouse_x, mouse_y);
			App->render->camera.x = -(minimap_mouse_position.x - App->render->camera.w * 0.5f);
			App->render->camera.y = -(minimap_mouse_position.y - App->render->camera.h * 0.5f);
		}
	}

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
	//if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	//	App->LoadGame("save_game.xml");

	//if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	//	App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ActivatePauseMenu();


	SDL_Rect correctedCamera = App->render->camera;
	correctedCamera.x = -correctedCamera.x;
	correctedCamera.y = -correctedCamera.y;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (correctedCamera.y - floor(1000.0f * dt) >= mapLimitsRect.y)
		{
			App->render->camera.y += floor(1000.0f * dt);
		}
		else
		{
			App->render->camera.y = 0;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) 
	{
		if (correctedCamera.y + App->render->camera.h + floor(1000.0f * dt) <= mapLimitsRect.h) 
		{
			App->render->camera.y -= floor(1000.0f * dt);
		}
		else 
		{
			App->render->camera.y = -mapLimitsRect.h + App->render->camera.h;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) 
	{
		if (correctedCamera.x - floor(1000.0f * dt) >= mapLimitsRect.x)
		{
			App->render->camera.x += floor(1000.0f * dt);
		}
		else 
		{
			App->render->camera.x = -mapLimitsRect.x;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) 
	{
		if (correctedCamera.x + App->render->camera.w + floor(1000.0f * dt) <= mapLimitsRect.x + mapLimitsRect.w)
		{
			App->render->camera.x -= floor(1000.0f * dt);
		}
		else
		{
			App->render->camera.x = -(mapLimitsRect.x + mapLimitsRect.w) + App->render->camera.w;
		}
	}


	App->map->Draw();


	//Quad draw

	//if (App->input->drawDebug)
	//	App->render->DrawQuadTree(quadTree->type, quadTree->baseNode);

	iPoint p = App->map->GetMousePositionOnMap();
	if (!App->entityManager->crPreview.active && IN_RANGE(p.x, 0, App->map->data.width-1) == 1 && IN_RANGE(p.y, 0, App->map->data.height-1) == 1)
	{
		p = App->map->MapToWorld(p.x, p.y);
		App->render->Blit(debugBlue_tex, p.x, p.y);
	}

	for (int i = 0; i < App->pathfinding->pathfinderList.size(); i++)
	{
		std::vector<iPoint> path = *App->pathfinding->pathfinderList[i].GetLastPath();

		for (uint i = 0; i < path.size(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path.at(i).x, path.at(i).y);
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

	//App->render->DrawQuad(mapLimitsRect, 255, 255, 255, 40);

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


	App->tex->UnLoad(debugBlue_tex);
	App->tex->UnLoad(debugRed_tex);
	App->tex->UnLoad(winlose_tex);

	DeactivateConfirmationMenu();
	DeactivateOptionsMenu();
	DeactivatePauseMenu();
	HUDDeleteListTroops();
	App->gui->DeleteUIElement(ui_ingame);
	ui_ingame = nullptr;
	for (int i = 0; i < 3; i++)
	{
		App->gui->DeleteUIElement(ui_text_ingame[i]);
		ui_text_ingame[i] = nullptr;
	}
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
	paused_game = true;
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
	paused_game = false;
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

// Called when returning to main menu (either winning/losing or by menu options like exit)
void j1Scene::BackToTitleMenu() {
	App->change_scene = true;
	destroy = true;
	App->map->destroy = true;
	App->pathfinding->destroy = true;
	App->entityManager->destroy = true;
	App->minimap->destroy = true;
}

// Called when restarting the game
void j1Scene::RestartGame() {
	App->restart_scene = true;
	destroy = true;
	App->map->destroy = true;
	App->pathfinding->destroy = true;
	App->entityManager->destroy = true;
	App->minimap->destroy = true;
}

// Called when selecting troops or buildings
void j1Scene::HUDUpdateSelection(std::list<Entity*> listEntities) {
	int i = 0;
	HUDDeleteListTroops();
	HUDDeleteSelectedTroop();
	for (std::list<Entity*>::iterator it = listEntities.begin(); it != listEntities.end(); it++) {
		if (it._Ptr->_Myval->type == EntityType::UNIT) {
			Unit* unit = (Unit*)it._Ptr->_Myval;
			if (i == 0) {
				number_of_troops[i]++;
				type_of_troops[i] = unit->unitType;
				i++;
			}
			else {
				bool new_troop = true;
				for (int j = 0; j < i; j++) {
					if (type_of_troops[j] == unit->unitType) {
						number_of_troops[j]++;
						new_troop = false;
					}
				}
				if (new_troop == true) {
					number_of_troops[i]++;
					type_of_troops[i] = unit->unitType;
					i++;
				}
			}
		}
	}
	SDL_Rect r = { 825,618,30,41 };
	SDL_Rect r2 = { 828,645,17,9 };
	
	for (int j = 0; j < i; j++) {
		
		if (j == 6) {
			r.y += 50;
			r2.y += 50;
			r.x = 825;
			r2.x = 828;
		}
			hud_list_troops[j]=(ImageUI*)App->gui->CreateUIElement(Type::IMAGE, (UI*)ui_ingame, r, GetSpritePortrait(0, type_of_troops[j]), "Troop", { 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 },
				nullptr, 0, false, -1.0F, 1);
			if (number_of_troops[j] > 1) 
				hud_number_troops[j] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, r2, { 0,0,100,100 }, std::to_string(number_of_troops[j]), { 255,255,255,255 });
			r.x += 34;
			r2.x += 34;
	}

	SDL_Rect position_name = { 725,603,30,30 };
	if (listEntities.begin()._Ptr->_Myval->type == EntityType::UNIT) {
		Unit* unit = (Unit*)listEntities.begin()._Ptr->_Myval; 	
		hud_selected_troop = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, (UI*)ui_ingame, { 640,613,76,105 }, GetSpritePortrait(1, unit->unitType), "Troop", { 0,0,0,0 }, { 0,0,0,0 }, false,
			{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
		hud_stats_selected_troop[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, position_name, { 0,0,100,100 }, "Unit", { 0,0,0,255 });
		bool combat_unit = false;
		switch (unit->unitType) {
		case UnitType::MONK:
			hud_stats_selected_troop[0]->SetString("Monk");
			position_name.x += 18;
			hud_stats_selected_troop[0]->SetRect(position_name);
			break;
		case UnitType::CLERIC:
			hud_stats_selected_troop[0]->SetString("Cleric");
			break;
		case UnitType::ASSASSIN:
			hud_stats_selected_troop[0]->SetString("Assassin");
			position_name.x += 6;
			hud_stats_selected_troop[0]->SetRect(position_name);
			combat_unit = true;
			break;
		case UnitType::PIKEMAN:
			hud_stats_selected_troop[0]->SetString("Pikeman");
			combat_unit = true;
			break;
		}
		if (combat_unit == true) {
			CombatUnit* cunit = (CombatUnit*)unit;
			hud_stats_selected_troop[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,622,30,30 }, { 0,0,100,100 }, "Damage:", { 0,0,0,255 });
			hud_stats_selected_troop[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 780,622,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetDamageValue()), { 0,0,0,255 });
			hud_stats_selected_troop[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,636,30,30 }, { 0,0,100,100 }, "Range:", { 0,0,0,255 });
			hud_stats_selected_troop[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 768,636,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetRangeValue()), { 0,0,0,255 });
			hud_stats_selected_troop[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,650,30,30 }, { 0,0,100,100 }, "Speed:", { 0,0,0,255 });
			hud_stats_selected_troop[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 760,650,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetSpeedValue()), { 0,0,0,255 });
			hud_stats_selected_troop[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,664,30,30 }, { 0,0,100,100 }, "Level:", { 0,0,0,255 });
			hud_stats_selected_troop[8] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 760,664,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetLevel()), { 0,0,0,255 });
			hud_stats_selected_troop[9] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,678,30,30 }, { 0,0,100,100 }, "Exp:", { 0,0,0,255 });
			hud_stats_selected_troop[10] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 747,678,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetExperience()), { 0,0,0,255 });
			hud_stats_selected_troop[11] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,703,30,10 }, { 0,0,100,100 }, "Health:", { 0,0,0,255 });
			hud_stats_selected_troop[12] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 773,703,30,10 }, { 0,0,100,100 }, std::to_string(unit->GetHealth()), { 0,0,0,255 });
		}
		else {
			hud_stats_selected_troop[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,703,30,10 }, { 0,0,100,100 }, "Health:", { 0,0,0,255 });
			hud_stats_selected_troop[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 773,703,30,10 }, { 0,0,100,100 }, std::to_string(unit->GetHealth()), { 0,0,0,255 });
		}
	}
	else if (listEntities.begin()._Ptr->_Myval->type == EntityType::BUILDING) {
		Building* building = (Building*)listEntities.begin()._Ptr->_Myval;
		hud_selected_troop = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, (UI*)ui_ingame, { 640,613,76,105 }, GetSpritePortraitBuilding(0, building->GetBuildingType()), "Building",
			{ 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
		hud_stats_selected_troop[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, position_name, { 0,0,100,100 }, "Building", { 0,0,0,255 });
		switch (building->GetBuildingType()) {
		case BuildingType::FORTRESS:
			hud_stats_selected_troop[0]->SetString("Fortress");
			//position_name.x += 18;
			hud_stats_selected_troop[0]->SetRect(position_name);
			break;
		case BuildingType::MONASTERY:
			hud_stats_selected_troop[0]->SetString("Monastery");
			break;
		case BuildingType::TEMPLE:
			hud_stats_selected_troop[0]->SetString("Temple");
			//position_name.x += 6;
			hud_stats_selected_troop[0]->SetRect(position_name);
			break;
		case BuildingType::ENCAMPMENT:
			hud_stats_selected_troop[0]->SetString("Encampment");
			break;
		}
		hud_stats_selected_troop[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,622,30,30 }, { 0,0,100,100 }, "Influence:", { 0,0,0,255 });
		hud_stats_selected_troop[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 780,622,30,30 }, { 0,0,100,100 }, std::to_string(building->GetInfluence()), { 0,0,0,255 });
		hud_stats_selected_troop[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,636,30,30 }, { 0,0,100,100 }, "Damage:", { 0,0,0,255 });
		hud_stats_selected_troop[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 768,636,30,30 }, { 0,0,100,100 }, std::to_string(building->GetDamage()), { 0,0,0,255 });
		hud_stats_selected_troop[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,650,30,30 }, { 0,0,100,100 }, "Max Capacity:", { 0,0,0,255 });
		hud_stats_selected_troop[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 760,650,30,30 }, { 0,0,100,100 }, std::to_string(building->GetMaxCap()), { 0,0,0,255 });
		hud_stats_selected_troop[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,703,30,10 }, { 0,0,100,100 }, "Health:", { 0,0,0,255 });
		hud_stats_selected_troop[8] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 773,703,30,10 }, { 0,0,100,100 }, std::to_string(building->GetHealth()), { 0,0,0,255 });
	}
}

// Called when deleting the list of troops in the HUD
void j1Scene::HUDDeleteListTroops() {
	for (int i = 12; i >= 0; i--) {
		number_of_troops[i] = 0;
		if (hud_list_troops[i] != nullptr) {
			App->gui->DeleteUIElement(hud_list_troops[i]);
			hud_list_troops[i] = nullptr;
		}
		if (hud_number_troops[i] != nullptr) {
			App->gui->DeleteUIElement(hud_number_troops[i]);
			hud_number_troops[i] = nullptr;
		}
	}
}

// Called when deleting the selected troop in the HUD
void j1Scene::HUDDeleteSelectedTroop() {
	for (int i = 12; i >= 0; i--) {
		if (hud_stats_selected_troop[i] != nullptr) {
			App->gui->DeleteUIElement(hud_stats_selected_troop[i]);
			hud_stats_selected_troop[i] = nullptr;
		}
	}
	if (hud_selected_troop != nullptr) {
		App->gui->DeleteUIElement(hud_selected_troop);
		hud_selected_troop = nullptr;
	}
}

// Called to get the rect of the sprite of the portrait
SDL_Rect j1Scene::GetSpritePortrait(int type_of_portrait, UnitType unit_type) {
	SDL_Rect sprite = { 0,0,0,0 };
	if (type_of_portrait == 0) {
		switch (unit_type) {
		case UnitType::ASSASSIN:
			sprite = { 299,194,30,41 };
			break;
		case UnitType::PIKEMAN:
			sprite = { 331,194,30,41 };
			break;
		case UnitType::MONK:
			sprite = { 362,194,30,41 };
			break;
		case UnitType::PRIEST:
			sprite = { 393,194,30,41 };
			break;
		}
	}
	else if (type_of_portrait == 1) {
		switch (unit_type) {
		case UnitType::ASSASSIN:
			sprite = { 2,321,76,105 };
			break;
		case UnitType::PIKEMAN:
			sprite = { 82,321,76,105 };
			break;
		case UnitType::MONK:
			sprite = { 162,321,76,105 };
			break;
		case UnitType::PRIEST:
			sprite = { 242,321,76,105 };
			break;
		}
	}
	return sprite;

}

// Called to get the rect of the sprite of the portrait of the building
SDL_Rect j1Scene::GetSpritePortraitBuilding(int type_of_portrait, BuildingType building_type) {
	SDL_Rect sprite = { 0,0,0,0 };
	if (type_of_portrait == 0) {
		switch (building_type) {
		case BuildingType::FORTRESS:
			sprite = { 299,431,30,41 };
			break;
		case BuildingType::MONASTERY:
			sprite = { 331,431,30,41 };
			break;
		case BuildingType::ENCAMPMENT:
			sprite = { 362,431,30,41 };
			break;
		case BuildingType::TEMPLE:
			sprite = { 393,431,30,41 };
			break;
		}
	}
	return sprite;
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
			else if (confirmation_option.compare("RESTART") == 0)
			{
				//close_menus = CloseSceneMenus::Confirmation_and_Pause;
				RestartGame();
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
			App->render->Blit(winlose_tex, 230, 100, &sec_win, NULL, 0.0F);
			App->render->Blit(winlose_tex, 230, 100, &sec_viking, NULL, 0.0F);
		}
		else {
			App->render->Blit(winlose_tex, 230, 100, &sec_greek, NULL, 0.0F);
			App->render->Blit(winlose_tex, 230, 100, &sec_lose, NULL, 0.0F);
		}
	}

	if (type == 2) {
		if (win == true) {
			App->render->Blit(winlose_tex, 230, 100, &sec_greek, NULL, 0.0F);
			App->render->Blit(winlose_tex, 230, 100, &sec_lose, NULL, 0.0F);
		}
		else {
			App->render->Blit(winlose_tex, 230, 100, &sec_viking, NULL, 0.0F);
			App->render->Blit(winlose_tex, 230, 100, &sec_win, NULL, 0.0F);
		}
	}
	if (timer_win_lose.ReadSec() >= 5) {
		BackToTitleMenu();
	}
}