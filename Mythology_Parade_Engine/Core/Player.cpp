#include "Player.h"
#include <iostream>
#include "CurrencySystem.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Gui.h"

Player::Player()
{
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
	App->scene->ui_text_ingame[0]->SetString(faith);
	App->scene->ui_text_ingame[1]->SetString(sacrifice);
	App->scene->ui_text_ingame[2]->SetString(prayer);

	//LOGIC 
	if (!App->input->GetMouseButtonDown(1))
	{
		App->input->GetMousePosition(x, y);
	}

	if(App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		int x2, y2;
		App->input->GetMousePosition(x2, y2);


		vertical1 = { x, y, 2, y2 - y };
		vertical2 = { x2, y, 2, y2 - y };
		horizontal1 = { x, y, x2 - x, 2 };
		horizontal2 = { x, y2, x2 - x, 2 };

		App->render->DrawQuad(vertical1, 255, 255, 255, 255);
		App->render->DrawQuad(vertical2, 255, 255, 255, 255);
		App->render->DrawQuad(horizontal1, 255, 255, 255, 255);
		App->render->DrawQuad(horizontal2, 255, 255, 255, 255);
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
