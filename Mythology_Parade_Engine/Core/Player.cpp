#include "Player.h"
#include <iostream>
#include "CurrencySystem.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "HUD.h"
#include "EntityManager.h"

Player::Player()
{
	research_assassin = research_chaotic_beast = research_chaotic_miracle = research_cleric = research_encampment = research_lawful_beast = research_lawful_miracle = research_lawful_victory =
		research_temple = research_chaotic_victory = false;
	Start();
	marc = 69;
}

Player::~Player()
{
}

bool Player::Awake()
{
	return true;
}

bool Player::Start()
{

	tick2 = SDL_GetTicks();
	player_win = player_lose = false;

	CurrencySystem::faith = 500;
	CurrencySystem::prayers = 0;
	CurrencySystem::sacrifices = 0;

	dontSelect = false;
	num_encampment = num_monastery = num_temple = 0;
	time_production_victory = 10;

	player_type = CivilizationType::VIKING;
	displayDebug = false;
	oneTime = true;


	if (civilization == CivilizationType::GREEK)
		name = "greek";
	else
		name = "viking";


	buildingSelect = nullptr;

	if (App->entityManager->initCivilizations)
	{
		App->entityManager->InitVikings();
		App->entityManager->InitGreek();
		App->entityManager->initCivilizations = false;
	}
	App->entityManager->playerCreated = true;
	return true;
}

bool Player::PreUpdate()
{
	//Logic Faith Increase
	tick1 = SDL_GetTicks();
	if (tick1 - tick2 >= 2000) 
	{
		
		IncreaseFaith();
		tick2 = SDL_GetTicks();
	}


	//Sending all numbers to strings to print
	faith = std::to_string(CurrencySystem::faith);
	sacrifice = std::to_string(CurrencySystem::sacrifices);
	prayer = std::to_string(CurrencySystem::prayers);

	//if (oneTime)
	//{
	//	InitVikings();
	//	InitGreek();
	//	oneTime = false;
	//}

	return true;
}

bool Player::Update(float dt)
{
	App->scene->hud->ui_text_ingame[0]->SetString(faith);
	App->scene->hud->ui_text_ingame[1]->SetString(sacrifice);
	App->scene->hud->ui_text_ingame[2]->SetString(prayer);
	
	//if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN && !App->entityManager->crPreview.active) 
	//{
	//	//Unit spawn
	//	iPoint mouse = App->map->GetMousePositionOnMap();
	//	iPoint spawnPos = App->map->TileCenterPoint(mouse);

	//	//Todo change assassin for the type of unit
	//	App->entityManager->CreateUnitEntity(UnitType::MONK, spawnPos);
	//}
	
	//Selection logics and drawing
	if (!App->scene->paused_game)
	{
		SelectionDraw_Logic(); 
		PlayerInputs();
	}

	return true;
}

bool Player::PostUpdate()
{
	return true;
}

bool Player::CleanUp()
{
	listEntities.clear();
	return true;
}

void Player::SelectionDraw_Logic()
{
	if (!App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		App->input->GetMousePosition(preClicked.x, preClicked.y);
		if(preClicked.y>=590) 
		{
			dontSelect = true;
			return;
		}
		else
		{
			dontSelect = false;
		}
		preClicked = App->render->ScreenToWorld(preClicked.x, preClicked.y);
	}
	if (!dontSelect)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
		{
			App->input->GetMousePosition(postClicked.x, postClicked.y);
			if (postClicked.y >= 590)
			{
				postClicked.y = 588;
			}
			postClicked = App->render->ScreenToWorld(postClicked.x, postClicked.y);

			App->render->DrawQuad({ preClicked.x, preClicked.y, postClicked.x - preClicked.x, postClicked.y - preClicked.y }, 255, 255, 255, 255, false);
		}

		if (App->input->GetMouseButtonDown(1) == KEY_UP)
		{
			for each (Unit* unit in listEntities)
			{
				unit->SetSelected(false);
			}
			if(buildingSelect!=nullptr)
				buildingSelect->SetSelected(false);
			listEntities.clear();
			buildingSelect = nullptr;
			ClickLogic();
			SeeEntitiesInside();
			ActionToUnit();
			ActionToBuilding();
			App->scene->hud->HUDUpdateSelection(listEntities, (Building*)buildingSelect);
		}
	}
}

std::list<Entity*> Player::GetEntitiesSelected()
{
	return listEntities;
}

Building* Player::GetSelectedBuild()
{
	return (Building*) buildingSelect;
}

Building* Player::GetEnemySelectedBuild()
{
	return (Building*)enemyBuildingSelect;
}

void Player::ActionToUnit()
{
	if (listEntities.size() == 1 && App->scene->nextUnit_selected)
	{
		Unit* unit = static_cast<Unit*>(listEntities.begin()._Ptr->_Myval);
		unit->SetMaxUnitHealth();
		App->scene->nextUnit_selected = false;
	}
}

void Player::ActionToBuilding()
{
	if (App->scene->nextBuilding_selected && GetEnemySelectedBuild()->name == "encampment" && civilization != GetEnemySelectedBuild()->civilization)
	{
		CivilizationType civ;
		int info;
		if (GetEnemySelectedBuild()->civilization == CivilizationType::GREEK)
		{
			civ = CivilizationType::VIKING; info = 3;
		}
		else
		{
			civ = CivilizationType::GREEK; info = 7;
		}

		iPoint pos = { (int)GetEnemySelectedBuild()->position.x, (int)GetEnemySelectedBuild()->position.y };
		Building* building = static_cast<Building*>(App->entityManager->CreateBuildingEntity(pos, BuildingType::ENCAMPMENT, App->entityManager->buildingsData[info], civ));
		building->SetTimeProducing(0);
		App->entityManager->DeleteEntity(GetEnemySelectedBuild());
		App->scene->nextBuilding_selected = false;
	}
	else if (App->scene->building_meteor && GetEnemySelectedBuild()->name == "encampment")
	{
		App->scene->building_meteor = false;
		App->entityManager->DeleteEntity(GetEnemySelectedBuild());
	}
}

void Player::SeeEntitiesInside()
{
	//ALERT MAYK
	std::list<Entity*>::iterator it = App->entityManager->entities[EntityType::UNIT].begin();
	for (it; it != App->entityManager->entities[EntityType::UNIT].end(); ++it)
	{
		if ((it._Ptr->_Myval->position.x >= preClicked.x && it._Ptr->_Myval->position.x <= postClicked.x) || (it._Ptr->_Myval->position.x <= preClicked.x && it._Ptr->_Myval->position.x >= postClicked.x))
		{
			if ((it._Ptr->_Myval->position.y >= preClicked.y && it._Ptr->_Myval->position.y <= postClicked.y) || it._Ptr->_Myval->position.y <= preClicked.y && it._Ptr->_Myval->position.y >= postClicked.y)
			{
				if (it._Ptr->_Myval->civilization == player_type)
				{
					it._Ptr->_Myval->SetSelected(true);
					listEntities.push_back(it._Ptr->_Myval);
				}
			}
		}
	}
}

void Player::PlayerInputs()
{

	//CHANGE ALL ITINERATIONS TO entityInsideCamera LIST
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->scene->godMode = !App->scene->godMode;
		App->input->drawDebug = !App->input->drawDebug;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT && App->scene->godMode)
	{
		CurrencySystem::IncreaseAll(10);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && App->scene->godMode)
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->TileCenterPoint(mouse);
		App->entityManager->CreateUnitEntity(UnitType::CYCLOP, spawnPos,civilization);
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && App->scene->godMode)
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->TileCenterPoint(mouse);
		App->entityManager->CreateUnitEntity(UnitType::DRAUGAR , spawnPos, civilization);
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && App->scene->godMode)
	{
		if (!listEntities.empty())
		{
			std::list<Entity*>::iterator it = listEntities.begin();
			for (it; it != listEntities.end(); ++it)
			{
				App->entityManager->DeleteEntity(it._Ptr->_Myval);
			}
			listEntities.clear();
			App->scene->hud->HUDUpdateSelection(listEntities, nullptr);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->scene->godMode)
	{
		player_win = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && App->scene->godMode)
	{
		player_lose = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && App->scene->godMode)
	{
		//Display collisions
		displayDebug = !displayDebug;

		for (unsigned int i = 1; i < App->entityManager->entities.size(); i++)
		{
			std::list<Entity*>::iterator it = App->entityManager->entities[(EntityType)i].begin();

			Entity* ent = nullptr;
			for (it; it != App->entityManager->entities[(EntityType)i].end(); ++it)
			{
				ent = it._Ptr->_Myval;
				ent->displayDebug = displayDebug;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN && App->scene->godMode)
	{
		if (!listEntities.empty())
		{
			std::list<Entity*>::iterator it = listEntities.begin();
			for (it; it != listEntities.end(); ++it)
			{
				Unit* unit = static_cast<Unit*>(it._Ptr->_Myval);
				unit->DivideHealth();
			}
			listEntities.clear();
		}
	}
}

void Player::ClickLogic()
{
	App->input->GetMousePosition(click.x,click.y);
	click = App->render->ScreenToWorld(click.x, click.y);
	std::list<Entity*>::iterator it = App->entityManager->entities[EntityType::BUILDING].begin();
	for (it; it != App->entityManager->entities[EntityType::BUILDING].end(); ++it)
	{
		if (click.x >= it._Ptr->_Myval->getCollisionRect().x && click.x <= it._Ptr->_Myval->getCollisionRect().x + it._Ptr->_Myval->getCollisionRect().w)
		{
			if (click.y <= it._Ptr->_Myval->getCollisionRect().y && click.y >= it._Ptr->_Myval->getCollisionRect().y + it._Ptr->_Myval->getCollisionRect().h)
			{
				if (it._Ptr->_Myval->civilization == civilization) {
					buildingSelect = it._Ptr->_Myval;
					it._Ptr->_Myval->SetSelected(true);
				}
				else {
					enemyBuildingSelect = it._Ptr->_Myval;
				}
			}
		}
	}
	if (listEntities.empty()) 
	{
		it = App->entityManager->entities[EntityType::UNIT].begin();
		for (it; it != App->entityManager->entities[EntityType::UNIT].end(); ++it)
		{
			if (click.x >= it._Ptr->_Myval->getCollisionRect().x && click.x <= it._Ptr->_Myval->getCollisionRect().x + it._Ptr->_Myval->getCollisionRect().w)
			{
				if (click.y <= it._Ptr->_Myval->getCollisionRect().y && click.y >= it._Ptr->_Myval->getCollisionRect().y + it._Ptr->_Myval->getCollisionRect().h)
				{
					if (it._Ptr->_Myval->civilization == civilization) 
					{
						it._Ptr->_Myval->SetSelected(true);
						listEntities.push_back(it._Ptr->_Myval);
						if (preClicked == postClicked)
							return;
					}
				}
			}
		}
	}
}

int Player::GetFaith() 
{
	return CurrencySystem::faith;
}

int Player::GetPrayers() 
{
	return CurrencySystem::prayers;
}

int Player::GetSacrifices() 
{
	return CurrencySystem::sacrifices;
}

void Player::SetFaith(int var)
{
	CurrencySystem::faith = var;
}

void Player::SetPrayers(int var)
{
	CurrencySystem::prayers = var;
}

void Player::SetSacrifices(int var)
{
	CurrencySystem::sacrifices = var;
}


