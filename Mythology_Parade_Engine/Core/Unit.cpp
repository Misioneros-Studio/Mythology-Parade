#include "Unit.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Input.h"

Unit::Unit(UnitType type): unitType(type), _isSelected(false), moveSpeed(1)
{
	
	state = AnimationType::IDLE;
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

void Unit::MoveToTarget()
{
	//if (!isSelected())
	//	return;

	////move function logic
	iPoint increment = { 0, 0 };

	switch (currentDirection)
	{
	case Direction::UP:
		increment = { 0, -2 };
		break;
	case Direction::LATERAL:
		increment = { 2, 0};
		break;
	case Direction::DOWN:
		increment = { 0, 2 };
		break;
	case Direction::DIAGONAL_DOWN:
		increment = { 2, 1 };
		break;
	case Direction::DIAGONAL_UP:
		increment = { 2, -1 };
		break;
	}

	if (flipState == SDL_FLIP_NONE) 
	{
		increment.x *= -1;
	}

	state = AnimationType::WALK;

	iPoint currentIso = position + increment;
	iPoint targetIso = App->map->MapToWorld(targetPosition.x, targetPosition.y);
	targetIso += App->map->GetTilesHalfSize();

	if (currentIso.y > targetIso.y) 
	{
		currentIso.x += App->map->data.tile_width / 2;
		currentIso.y += App->map->data.tile_height / 2;
	}
	
	currentIso = App->map->WorldToMap(currentIso.x, currentIso.y);

	if (currentIso == targetPosition) 
	{
		position = App->map->MapToWorld(targetPosition.x, targetPosition.y);
		position += App->map->GetTilesHalfSize();

		targetPosition.ResetAsPosition();
		ChangeState(targetPosition, AnimationType::IDLE);
	}
	else
	{
		position += increment;
	}
}

void Unit::Init(int maxHealth)
{
	SetMaxHealth(maxHealth);
	HealthSystem::Init();
	
}

void Unit::ChangeState(iPoint isoLookPosition, AnimationType newState) 
{
	if (targetPosition == iPoint(-1, -1)) 
	{
		currentAnim = App->entityManager->animations[unitType][AnimationType::IDLE][currentDirection];
	}
	else
	{
		currentDirection = getMovementDirection(isoLookPosition);
		currentAnim = App->entityManager->animations[unitType][newState][currentDirection];
	}
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

	if (target.x >= position.x)
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
		targetPosition.ResetAsPosition();
	}

	return dir;
}



