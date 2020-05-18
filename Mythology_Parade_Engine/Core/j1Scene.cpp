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
#include "j1FadeToBlack.h"
#include "HUD.h"
#include "ResearchMenu.h"

#include"QuadTree.h"

j1Scene::j1Scene() : j1Module()
{
	name.append("scene");
	winlose_tex = nullptr;
	clickToPath = false;
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
	hud = nullptr;
	research_menu = nullptr;
  
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

		mapLimitsRect = App->map->GetMapRect();
		App->pathfinding->maxPathLenght = App->map->GetMapMaxLenght();
		App->entityManager->LoadBuildingsBlitRect();

		SDL_ShowCursor(0);

		RELEASE_ARRAY(data);
	}

	App->render->camera.x = -2683;
	App->render->camera.y = -2000;

	

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

	WinViking_sound = App->audio->LoadFx("audio/fx/WinVikings.wav");
	WinGreek_sound = App->audio->LoadFx("audio/fx/win_greeks.wav");
	Lose_sound = App->audio->LoadFx("audio/fx/lose_sound.wav");
	

	paused_game = false;
	godMode = false;

	winlose_tex = App->tex->Load("gui/WinLoseBackground.png");

	//iPoint position;
	//iPoint size;
	//position = App->map->WorldToMap(0, 0);
	//size = iPoint(App->map->data.width * App->map->data.tile_width, App->map->data.height * App->map->data.tile_height);
	//quadTree = new QuadTree(TreeType::ISOMETRIC, position.x + (App->map->data.tile_width / 2), position.y, size.x, size.y);
	//quadTree->baseNode->SubDivide(quadTree->baseNode, 5);
  
	App->audio->PlayMusic("audio/music/Ambient1.ogg", 2.0F);

	//Creating players
	Player* player = static_cast<Player*>(App->entityManager->CreatePlayerEntity(App->fade_to_black->actual_civilization));

	if (player->player_type == CivilizationType::VIKING)
		App->render->camera.x = -2683;
	else if (player->player_type == CivilizationType::GREEK)
		App->render->camera.x = -1683;

	research_menu = new ResearchMenu(player);
	hud = new HUD(research_menu);
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		ClickToPath();
	}	
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && clickToPath)
	{
		ClickToPath();
		clickToPath = false;
		App->entityManager->getPlayer()->dontSelect = true;
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

void j1Scene::ClickToPath()
{
	//TMP: Temporal pathfinding debug
	std::list<Entity*> list = App->entityManager->getPlayer()->GetEntitiesSelected();

	if (list.size() > 0)
	{
		float n = App->entityManager->getPlayer()->GetEntitiesSelected().size();
		float x = 0, y = 0;

		for (std::list<Entity*>::iterator it = list.begin(); it != list.end(); it++)
		{
			x += it._Ptr->_Myval->position.x;
			y += it._Ptr->_Myval->position.y;
		}

		x /= n;
		y /= n;

		iPoint origin = App->map->WorldToMap((int)x, (int)y);
		iPoint ending = App->map->GetMousePositionOnMap();
		LOG("Origin: %i, %i", origin.x, origin.y);
		LOG("Ending: %i, %i", ending.x, ending.y);

		int posX, posY;
		App->input->GetMousePosition(posX, posY);
		iPoint p = App->render->ScreenToWorld(posX, posY);
		p = App->render->ScreenToWorld(posX, posY);

		CivilizationType playerCiv = App->entityManager->getPlayer()->civilization;
		bool attacking = false;

		for (std::list<Entity*>::iterator it = App->entityManager->entities[EntityType::UNIT].begin(); it != App->entityManager->entities[EntityType::UNIT].end(); it++)
		{
			SDL_Rect collider = it._Ptr->_Myval->getCollisionRect();
			if (it._Ptr->_Myval->civilization != playerCiv && EntityManager::IsPointInsideQuad(collider, p.x, p.y))
			{
				Unit* unt = nullptr;
				for (std::list<Entity*>::iterator sel = list.begin(); sel != list.end(); sel++)
				{
					unt = (Unit*)sel._Ptr->_Myval;
					unt->enemyTarget = (Unit*)it._Ptr->_Myval;
					attacking = true;
				}
			}
		}

		if (!attacking)
		{
			Unit* unt = nullptr;
			for (std::list<Entity*>::iterator sel = list.begin(); sel != list.end(); sel++)
			{
				unt = (Unit*)sel._Ptr->_Myval;
				unt->enemyTarget = nullptr;
			}
		}

		if (origin != ending)
			App->pathfinding->RequestPath(origin, ending, list);

	}
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---
	switch (hud->close_menus)
	{
	case CloseSceneMenus::Pause:
		App->audio->FadeAudio(which_audio_fade::change_volume, 2, 150);
		hud->DeactivatePauseMenu();
		hud->close_menus = CloseSceneMenus::None;
		break;
	case CloseSceneMenus::Options:
		hud->DeactivateOptionsMenu();
		hud->close_menus = CloseSceneMenus::None;
		break;
	case CloseSceneMenus::Confirmation:
		hud->DeactivateConfirmationMenu();
		hud->close_menus = CloseSceneMenus::None;
		break;
	case CloseSceneMenus::Confirmation_and_Pause:
		hud->DeactivateConfirmationMenu();
		hud->DeactivatePauseMenu();
		hud->close_menus = CloseSceneMenus::None;
		break;
	case CloseSceneMenus::Research:
		hud->DeactivateResearchMenu();
		hud->close_menus = CloseSceneMenus::None;
		break;
	}
	// -------
	//if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	//	App->LoadGame("save_game.xml");

	//if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	//	App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		hud->ActivatePauseMenu();
		App->audio->FadeAudio(which_audio_fade::change_volume, 2, 50);

    if (paused_game == true) {
      if (hud->ui_volume_sliders[0] != nullptr)
        hud->UpdateSlider(0);
      if (hud->ui_volume_sliders[3] != nullptr)
        hud->UpdateSlider(3);
    }
	}

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

	if (godMode) {
		iPoint p = App->map->GetMousePositionOnMap();
		if (!App->entityManager->crPreview.active && IN_RANGE(p.x, 0, App->map->data.width - 1) == 1 && IN_RANGE(p.y, 0, App->map->data.height - 1) == 1)
		{
			p = App->map->MapToWorld(p.x, p.y);
			App->render->Blit(debugBlue_tex, p.x, p.y);
		}
	}

	if (App->entityManager->getPlayer() != nullptr) {
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
	}
	//App->render->DrawQuad(mapLimitsRect, 255, 255, 255, 40);

	/*CheckSpatial Audio
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		Mix_HaltChannel(-1);
		Mix_SetPosition(2, 270, 200);
		App->audio->PlayFx(2, WinGreek_sound, 1);
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		Mix_HaltChannel(-1);
		Mix_SetPosition(3, 90, 1);
		App->audio->PlayFx(3, WinGreek_sound, 1);
	}*/

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	if (hud->thing_selected != nullptr) {
		hud->UpdateSelectedThing();
		if (App->entityManager->getPlayer()->player_type == CivilizationType::VIKING)
			hud->ManageActionButtons();
		else if (App->entityManager->getPlayer()->player_type == CivilizationType::GREEK)
			hud->ManageActionButtons(false, false);
	}
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
	if (hud != nullptr) {
		hud->DeactivateResearchMenu();
		hud->DeactivateConfirmationMenu();
		hud->DeactivateOptionsMenu();
		hud->DeactivatePauseMenu();
		hud->HUDDeleteListTroops();
		hud->HUDDeleteSelectedTroop();
		hud->HUDDeleteActionButtons();
		App->gui->DeleteUIElement(hud->ui_ingame);
		hud->ui_ingame = nullptr;
		for (int i = 0; i < 3; i++)
		{
			App->gui->DeleteUIElement(hud->ui_text_ingame[i]);
			hud->ui_text_ingame[i] = nullptr;
		}
		//quadTree->Clear();
		delete hud;
		hud = nullptr;
		delete research_menu;
		research_menu = nullptr;
	}
	return true;
}



// Called when returning to main menu (either winning/losing or by menu options like exit)
void j1Scene::BackToTitleMenu() {
	destroy = true;
	App->map->destroy = true;
	App->pathfinding->destroy = true;
	App->entityManager->destroy = true;
	App->minimap->destroy = true;
	App->fade_to_black->FadeToBlack(which_fade::scene_to_title, 2);

	//App->change_scene = true;
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

void j1Scene::OnClick(UI* element, float argument)
{

	switch (element->type)
	{

	case Type::BUTTON:

		if (element->name == "SAVE")
		{
			hud->confirmation_option = "SAVE";
			hud->ActivateConfirmationMenu("SAVE THE GAME");
		}
		else if (element->name == "LOAD")
		{
			hud->confirmation_option = "LOAD";
			hud->ActivateConfirmationMenu("LOAD THE GAME");
		}
		else if (element->name == "OPTIONS")
		{
			hud->ActivateOptionsMenu();
		}
		else if (element->name == "CLOSE OPTIONS")
		{
			hud->close_menus = CloseSceneMenus::Options;
		}
		else if (element->name == "RESTART")
		{
			hud->confirmation_option = "RESTART";
			hud->ActivateConfirmationMenu("RESTART");
		}
		else if (element->name == "SURRENDER")
		{
			hud->confirmation_option = "SURRENDER";
			hud->ActivateConfirmationMenu("SURRENDER");
			//////TODO: HERE LOSE CONDITION WILL BE TRUE
		}
		else if (element->name == "EXIT")
		{
			hud->confirmation_option = "EXIT";
			hud->ActivateConfirmationMenu("EXIT");
		}
		else if (element->name == "NO")
		{
			hud->close_menus = CloseSceneMenus::Confirmation;
		}
		else if (element->name == "YES")
		{
			hud->close_menus = CloseSceneMenus::Confirmation;
			if (hud->confirmation_option.compare("SAVE") == 0)
			{
				App->SaveGame("info.xml");
			}
			else if (hud->confirmation_option.compare("LOAD") == 0)
			{
				App->LoadGame("info.xml");
			}
			else if (hud->confirmation_option.compare("RESTART") == 0)
			{
				//close_menus = CloseSceneMenus::Confirmation_and_Pause;
				RestartGame();
			}
			else if (hud->confirmation_option.compare("SURRENDER") == 0)
			{
				App->entityManager->getPlayer()->player_lose = true;
				hud->close_menus = CloseSceneMenus::Confirmation_and_Pause;
			}
			else if (hud->confirmation_option.compare("EXIT") == 0)
			{
				BackToTitleMenu();
			}
		}
		else if (element->name == "CLOSE RESEARCH")
		{
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "FULLSCREEN") {
			App->win->ToggleFullscreen();
			if (hud->ui_button_options[1]->sprite1.y == 21) {
				hud->ui_button_options[1]->sprite1.y = hud->ui_button_options[1]->sprite2.y = hud->ui_button_options[1]->sprite3.y = 61;
			}
			else if(hud->ui_button_options[1]->sprite1.y == 61) {
				hud->ui_button_options[1]->sprite1.y = hud->ui_button_options[1]->sprite2.y = hud->ui_button_options[1]->sprite3.y = 21;
			}
		}
		else if (element->name == "CLOSE")
		{
			hud->close_menus = CloseSceneMenus::Pause;
		}
		else if (element->name == "Research") 
		{
			hud->ActivateResearchMenu();
		}
		else if (element->name == "RESEARCH TEMPLE") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Temple");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH ENCAMPMENT") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Encampment");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH CLERIC") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Cleric");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH ASSASSIN") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Assassin");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH LAWFUL BEAST") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Lawful Beast");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH CHAOTIC BEAST") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Chaotic Beast");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH LAWFUL MIRACLE") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Lawful Miracle");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH CHAOTIC MIRACLE") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Chaotic Miracle");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH LAWFUL VICTORY") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Lawful Victory");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "RESEARCH CHAOTIC VICTORY") {
			Building* building = (Building*)hud->thing_selected;
			building->StartResearching("Chaotic Victory");
			hud->close_menus = CloseSceneMenus::Research;
		}
		else if (element->name == "Produce_Temple")
		{
			//CONSTRUCT TEMPLE
			App->entityManager->EnterBuildMode();
			if (App->entityManager->getPlayer()->civilization == CivilizationType::VIKING)
				App->entityManager->SetBuildIndex(2);
			else if (App->entityManager->getPlayer()->civilization == CivilizationType::GREEK)
				App->entityManager->SetBuildIndex(6);
		}
		else if (element->name == "Produce_Encampment")
		{
			//CONSTRUCT ENCAMPMENT
			App->entityManager->EnterBuildMode();
			if(App->entityManager->getPlayer()->civilization==CivilizationType::VIKING)
				App->entityManager->SetBuildIndex(3);
			else if (App->entityManager->getPlayer()->civilization == CivilizationType::GREEK)
				App->entityManager->SetBuildIndex(7);

		}
		else if (element->name == "Produce_Monastery")
		{
			//CONSTRUCT MONASTERY
			App->entityManager->EnterBuildMode();
			if (App->entityManager->getPlayer()->civilization == CivilizationType::VIKING)
				App->entityManager->SetBuildIndex(1);
			else if (App->entityManager->getPlayer()->civilization == CivilizationType::GREEK)
				App->entityManager->SetBuildIndex(5);
		}
		else if (element->name == "Produce_Assassin")
		{
			Building* building = (Building*)hud->thing_selected;
			App->entityManager->getPlayer()->DecreaseFaith(100);
			building->StartProducing("Assassin");
		}
		else if (element->name == "Produce_Monk")
		{
			Building* building = (Building*)hud->thing_selected;
			App->entityManager->getPlayer()->DecreaseFaith(50);
			building->StartProducing("Monk");
		}
		else if (element->name == "Produce_Victory")
		{
			Building* building = (Building*)hud->thing_selected;
			App->entityManager->getPlayer()->DecreaseFaith(600);  
			building->StartProducing("Victory");
		}
		else if (element->name == "Produce_Sacrifices")
		{
			Building* building = (Building*)hud->thing_selected;
			App->entityManager->getPlayer()->DecreaseFaith(40);
			building->StartProducing("Sacrifices");
		}
		else if (element->name == "Produce_Prayers")
		{
			Building* building = (Building*)hud->thing_selected;
			App->entityManager->getPlayer()->DecreaseFaith(100);
			building->StartProducing("Prayers");
		}
		else if (element->name == "Upgrade") {
			//Upgrade level
			CombatUnit* unit =(CombatUnit*)App->entityManager->getPlayer()->GetEntitiesSelected().begin()._Ptr->_Myval;
			unit->LevelUp();
		}
		else if (element->name == "Move")
		{
			clickToPath = true;
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

	if (hud->start_timer == false) {
		hud->timer_win_lose.Start();
		animation_win_lose_timer.Start();
	}

	hud->start_timer = true;

	if (animation_win_lose_timer.ReadSec() <= 2) {
		global_pos = DoTransitionWinLose(230, 100, winlose_tex, animation_win_lose_timer);
	}

	else if (animation_win_lose_timer.ReadSec() >=2) {
		global_pos.y = 100;
	}

	if (type == 1) {
		if (win == true) {
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_viking, NULL, 0.0F);
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_win, NULL, 0.0F);
			if (Mix_Playing(3) == 0) 
			{
				App->audio->PlayFx(3, WinViking_sound);
			}
		}
		else {
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_greek, NULL, 0.0F);
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_lose, NULL, 0.0F);
			if (Mix_Playing(3) == 0) {
				App->audio->PlayFx(3, Lose_sound);
			}
		}
	}

	if (type == 2) {
		if (win == true) {
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_greek, NULL, 0.0F);
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_lose, NULL, 0.0F);
			if (Mix_Playing(3) == 0) {
				App->audio->PlayFx(3, WinGreek_sound);
			}
		}
		else {
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_viking, NULL, 0.0F);
			App->render->Blit(winlose_tex, global_pos.x, global_pos.y, &sec_win, NULL, 0.0F);
			if (Mix_Playing(3) == 0) {
				App->audio->PlayFx(3, Lose_sound);
			}
		}
	}
	if (hud->timer_win_lose.ReadSec() >= 5) {

		BackToTitleMenu();
	}
}

void j1Scene::FinishResearching(std::string thing_researched) {
	if (thing_researched == "Temple") {
		App->entityManager->getPlayer()->research_temple = true;
	}
	else if (thing_researched == "Encampment") {
		App->entityManager->getPlayer()->research_encampment = true;
	}
	else if (thing_researched == "Cleric") {
		App->entityManager->getPlayer()->research_cleric = true;
	}
	else if (thing_researched == "Assassin") {
		App->entityManager->getPlayer()->research_assassin = true;
	}
	else if (thing_researched == "Lawful Beast") {
		App->entityManager->getPlayer()->research_lawful_beast = true;
	}
	else if (thing_researched == "Chaotic Beast") {
		App->entityManager->getPlayer()->research_chaotic_beast = true;
	}
	else if (thing_researched == "Lawful Miracle") {
		App->entityManager->getPlayer()->research_lawful_miracle = true;
	}
	else if (thing_researched == "Chaotic Miracle") {
		App->entityManager->getPlayer()->research_chaotic_miracle = true;
	}
	else if (thing_researched == "Lawful Victory") {
		App->entityManager->getPlayer()->research_lawful_victory = true;
	}
	else if (thing_researched == "Chaotic Victory") {
		App->entityManager->getPlayer()->research_chaotic_victory = true;
	}

}


fPoint j1Scene::DoTransitionWinLose(int pos_x, int pos_y, SDL_Texture* tex, j1Timer time) {
	fPoint position_global;
	position_global.x = pos_x;

	if (first_time_timer_win == false) {
		animation_win_lose_timer.Start();
		first_time_timer_win = true;
	}

	float percentatge = time.ReadSec() *0.5f;
	position_global.y = LerpValue(percentatge, 200, 100);

	SDL_SetTextureAlphaMod(tex, 255*percentatge);

	return position_global;
}

float j1Scene::LerpValue(float percent, float start, float end)
{
	return start + percent * (end - start);
}
