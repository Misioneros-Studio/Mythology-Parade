#include "Unit.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Input.h"

Unit::Unit(UnitType type): unitType(type), state(IDLE), _isSelected(false), moveSpeed(1)
{
	

	//Init Units
	switch (type)
	{
	case UnitType::MONK:
		Init(1);
		break;
	}

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

	//Return
	return ret;
}

void Unit::SetMoveSpeed(int value)
{
	moveSpeed = value;
}

bool Unit::isSelected()
{
	return _isSelected;
}

//void Unit::MoveTo(p2Point<int>)
//{
//	if (!isSelected())
//		return;
//
//	//move function logic
//
//
//}

void Unit::Init(int maxHealth)
{
	SetMaxHealth(maxHealth);
	HealthSystem::Init();
	
}


bool Unit::Draw(float dt)
{
	bool ret = true;
	//App->render->Blit(texture, -90, 430, &position_rect);
	return ret;
}

void Unit::Action(Entity* entity)
{

	switch (unitType)
	{
	case UnitType::MONK:
	LOG("I'm a monk unit!");
	
		break;
	}
}

Direction Unit::getMovementDirection(iPoint target) 
{
	Direction dir = Direction::UP;

	iPoint temp = App->map->WorldToMap(position.x, position.y);

	target = App->map->MapToWorld(target.x, target.y);
	iPoint pos = App->map->MapToWorld(temp.x, temp.y);

	if (target.x > position.x)
	{
		flipState = SDL_FLIP_HORIZONTAL;
	}
	else
	{
		flipState = SDL_FLIP_NONE;
	}

	if (target.x == pos.x && target.y < pos.y)
	{
		dir = Direction::UP;
	}
	else if(target.x == pos.x && target.y > pos.y)
	{
		dir = Direction::DOWN;
	}
	else if (target.x != pos.x && target.y == pos.y)
	{
		dir = Direction::LATERAL;
	}
	else if (target.x != pos.x && target.y > pos.y)
	{
		dir = Direction::DIAGONAL_DOWN;
	}
	else if (target.x != pos.x && target.y < pos.y)
	{
		dir = Direction::DIAGONAL_UP;
	}
	else 
	{
		//Is the same place
		LOG("EHE");
	}

	return dir;
}



