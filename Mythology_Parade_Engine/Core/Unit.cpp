#include "Unit.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Input.h"

Unit::Unit(UnitType type): unitType(type), state(IDLE), isSelected(false), moveSpeed(1)
{
	

	//Init Units
	switch (type)
	{
	case MONK:
		SetMaxHealth(1); 
		break;
	}

	HealthSystem::Init();

}

Unit::~Unit()
{
}

bool Unit::Start()
{
	bool ret = true;

	//pugi::xml_document	character_file;
	//pugi::xml_parse_result result = character_file.load_file("assets/units/Assassin.tmx");
	//pugi::xml_node	character_node = character_file.child("map");
	//std::string image_source("assets/units/");
	//image_source += character_node.child("tileset").child("image").attribute("source").as_string();
	//texture = App->tex->Load(image_source.c_str());
	//position_rect.w = 125;
	//position_rect.h = 112;
	//LOG("%s", image_source.c_str());
	return ret;
}

bool Unit::Update(float dt)
{
	bool ret = true;

	//STATE MACHINE
	switch (state)
	{
	case IDLE:
		break;
	case MOVE:
		break;

	}



	//Allawys blit the sprite at the end
	ret = Draw(dt);
	Action();
	//Return
	return ret;
}

void Unit::SetMoveSpeed(int value)
{
	moveSpeed = value;
}

void Unit::MoveTo(p2Point<int>)
{
	//move function logic
}


bool Unit::Draw(float dt)
{
	bool ret = true;
	//App->render->Blit(texture, -90, 430, &position_rect);
	//LOG("%u", healthSystem.GetHealth());
	return ret;
}

void Unit::Action()
{
	switch (unitType)
	{
	case MONK:
	LOG("I'm a monk unit!");
		break;
	}
}



