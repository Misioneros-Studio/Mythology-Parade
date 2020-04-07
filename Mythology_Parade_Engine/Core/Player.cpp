#include "Player.h"
#include <iostream>
#include "CurrencySystem.h"
#include "j1Scene.h"
#include "j1Gui.h"

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
	return true;
}

bool Player::PreUpdate()
{
	//Logic Faith Increase
	tick1 = SDL_GetTicks();
	if (tick1 - tick2 >= 2000) 
	{
		currencySystem.faith += 2;
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
	App->scene->ui_text_ingame[2]->SetString(faith);
	App->scene->ui_text_ingame[1]->SetString(sacrifice);
	App->scene->ui_text_ingame[0]->SetString(prayer);

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) 
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->MapToWorld(mouse.x, mouse.y);

		spawnPos.y += (App->map->data.tile_height / 2);
		spawnPos.x += (App->map->data.tile_width / 2);
		App->entityManager->CreateUnitEntity(UnitType::ASSASSIN, spawnPos);
	}


	return true;
}

bool Player::PostUpdate()
{
	return true;
}

bool Player::CleanUp()
{
	return true;
}
