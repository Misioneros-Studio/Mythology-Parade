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
	
	//Selection logics and drawing
	SelectionDraw_Logic(); 

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
	if (!App->input->GetMouseButtonDown(1))
	{
		App->input->GetMousePosition(preClicked.x, preClicked.y);
		preClicked = App->render->ScreenToWorld(preClicked.x, preClicked.y);
		listEntities.clear(); //we clear the list of entities selected to select again or just deselect
	}

	if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		App->input->GetMousePosition(postClicked.x, postClicked.y);
		postClicked = App->render->ScreenToWorld(postClicked.x, postClicked.y);

		App->render->DrawQuad({preClicked.x, preClicked.y, postClicked.x - preClicked.x, postClicked.y - preClicked.y}, 255, 255, 255, 255, false);
		//App->render->DrawQuad({preClicked.x + 1, preClicked.y + 1, postClicked.x - preClicked.x - 2, postClicked.y - preClicked.y  - 2}, 255, 255, 255, 255, false);

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
				listEntities.push_back(it._Ptr->_Myval);
			}
		}
	}
}
