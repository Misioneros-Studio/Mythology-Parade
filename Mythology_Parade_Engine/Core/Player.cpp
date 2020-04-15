#include "Player.h"
#include <iostream>
#include "CurrencySystem.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "EntityManager.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Awake()
{
	player_type = CivilizationType::VIKING;
	return true;
}

bool Player::Start()
{
	tick2 = SDL_GetTicks();
	player_win = player_lose = false;
	currencySystem.faith = 0;
	currencySystem.prayers = 0;
	currencySystem.sacrifices = 0;
	return true;
}

bool Player::PreUpdate()
{
	//Logic Faith Increase
	tick1 = SDL_GetTicks();
	if (tick1 - tick2 >= 2000) 
	{
		currencySystem.faith += 2;
		currencySystem.sacrifices += 3;
		currencySystem.prayers += 5;
		tick2 = SDL_GetTicks();
	}


	//Sending all numbers to strings to print
	faith = std::to_string(currencySystem.faith);
	sacrifice = std::to_string(currencySystem.sacrifices);
	prayer = std::to_string(currencySystem.prayers);

	return true;
}

bool Player::Update(float dt)
{
	App->scene->ui_text_ingame[0]->SetString(faith);
	App->scene->ui_text_ingame[1]->SetString(sacrifice);
	App->scene->ui_text_ingame[2]->SetString(prayer);
	
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN && !App->entityManager->crPreview.active) 
	{
		//Unit spawn
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->TileCenterPoint(mouse);

		//Todo change assassin for the type of unit
		App->entityManager->CreateUnitEntity(UnitType::MONK, spawnPos);
	}
  
	//Selection logics and drawing
	SelectionDraw_Logic(); 

	playerInputs();

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
		preClicked = App->render->ScreenToWorld(preClicked.x, preClicked.y);
		//listEntities.clear(); //we clear the list of entities selected to select again or just deselect
	}

	if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		App->input->GetMousePosition(postClicked.x, postClicked.y);
		postClicked = App->render->ScreenToWorld(postClicked.x, postClicked.y);

		App->render->DrawQuad({preClicked.x, preClicked.y, postClicked.x - preClicked.x, postClicked.y - preClicked.y}, 255, 255, 255, 255, false);
		//App->render->DrawQuad({preClicked.x + 1, preClicked.y + 1, postClicked.x - preClicked.x - 2, postClicked.y - preClicked.y  - 2}, 255, 255, 255, 255, false);
	}

	if (App->input->GetMouseButtonDown(1) == KEY_UP)
	{
		SeeEntitiesInside(); //We iterate the list of entities to see if someone is in there
	}

}

std::list<Entity*> Player::GetEntitiesSelected()
{
	return listEntities;
}

void Player::SeeEntitiesInside()
{
	//ALERT MAYK
	std::list<Entity*>::iterator it = App->entityManager->entities[EntityType::UNIT].begin();
	for (it; it != App->entityManager->entities[EntityType::UNIT].end(); ++it)
	{
		if (it._Ptr->_Myval->position.x >= preClicked.x && it._Ptr->_Myval->position.x <= postClicked.x)
		{
			if (it._Ptr->_Myval->position.y >= preClicked.y && it._Ptr->_Myval->position.y <= postClicked.y)
			{
				//if (it._Ptr->_Myval->civilization == CivilizationType::VIKING)
				//{
					listEntities.push_back(it._Ptr->_Myval);
				//}
			}
		}
	}
}

void Player::playerInputs()
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->scene->godMode = !App->scene->godMode;
		App->input->drawDebug = !App->input->drawDebug;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT && App->scene->godMode)
	{
		currencySystem.IncreaseAll(10);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && App->scene->godMode)
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->TileCenterPoint(mouse);
		App->entityManager->CreateUnitEntity(UnitType::ASSASSIN, spawnPos);
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && App->scene->godMode)
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->TileCenterPoint(mouse);
		App->entityManager->CreateUnitEntity(UnitType::MONK, spawnPos);
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && App->scene->godMode)
	{
		std::list<Entity*>::iterator it = listEntities.begin();
		for (it; it != listEntities.end(); ++it)
		{
			App->entityManager->DeleteEntity(it._Ptr->_Myval);
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
}
