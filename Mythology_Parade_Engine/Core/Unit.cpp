#include "Unit.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Input.h"

Unit::Unit(UnitType type, iPoint pos): unitType(type), state(AnimationType::IDLE), _isSelected(false), moveSpeed(1)
{
	
	if (App->entityManager->getPlayer())
	{
		displayDebug = App->entityManager->getPlayer()->displayDebug;
	}
	else
	{
		displayDebug = false;
	}

	collisionRect = { 0, 0, 30, -55 };
	unitType = type;
	position = pos;
	state = AnimationType::IDLE;
	//Init Units
	switch (type)
	{
	case UnitType::MONK:
		time_production = 90;
		time_research = 0;
		researched = true;
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
	//ret = Draw(dt);

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
		increment = { 2, 0 };
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
		if (entPath.size() <= 0)
		{
			ChangeState(targetPosition, AnimationType::IDLE);
		}
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

	targetPosition.ResetAsPosition();
	ChangeState(targetPosition, state);
}

void Unit::ChangeState(iPoint isoLookPosition, AnimationType newState)
{
	if (targetPosition == iPoint(-1, -1) && entPath.size() == 0)
	{
		currentAnim = App->entityManager->animations[unitType][AnimationType::IDLE][currentDirection];
	}
	else
	{
		currentDirection = getMovementDirection(isoLookPosition);
		if (App->entityManager->animations[unitType][newState][currentDirection].name != currentAnim.name)
			currentAnim = App->entityManager->animations[unitType][newState][currentDirection];
	}
}

bool Unit::Draw(float dt)
{
	if (entPath.size() > 0 && targetPosition == iPoint(-1, -1))
	{
		targetPosition.x = entPath[0].x;
		targetPosition.y = entPath[0].y;
		ChangeState(targetPosition, AnimationType::WALK);
		entPath.erase(entPath.begin(), entPath.begin() + 1);
	}


	if (targetPosition != iPoint(-1, -1))
		MoveToTarget();


	int num_current_anim = currentAnim.GetSprite();
	blitRect = { (int)(currentAnim.sprites[num_current_anim].rect.w / 1.5f), (int)(currentAnim.sprites[num_current_anim].rect.h / 1.5f) };

	App->render->Blit(texture, position.x - blitRect.x / 2, position.y - blitRect.y, blitRect, &currentAnim.sprites[num_current_anim].rect, 1.f, flipState);

	collisionRect.x = position.x - (collisionRect.w / 2);
	collisionRect.y = position.y;

	if (displayDebug) 
	{

		App->render->DrawQuad(collisionRect, 0, 255, 0, 50);
	}

	return true;
}

void Unit::Action(Entity* entity)
{

	switch (unitType)
	{
		//Monks action are controled by Monastery so, its functionality is there ( on Monastery Building )
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
	else if (target.x == pos.x && target.y > pos.y)
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


void Unit::SetPath(const std::vector<iPoint> s_path)
{
	entPath = s_path;
}

