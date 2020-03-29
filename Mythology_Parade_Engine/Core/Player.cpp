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

	return true;
}

bool Player::Start()
{
	return true;
}

bool Player::PreUpdate()
{
	currencySystem.faith++;


	faith = std::to_string(currencySystem.faith);

	return true;
}

bool Player::Update(float dt)
{
	App->scene->ui_text_ingame[0]->SetString(faith);
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
