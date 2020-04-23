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
	position = {(float)pos.x, (float)pos.y};
	state = AnimationType::IDLE;
	flipState = SDL_FLIP_NONE;
	directionToTarget = {0, 0};
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

	fPoint increment = { 0, 0 };

	float speed = moveSpeed * App->GetDT();

	//Fast fix for ft increasing bug
	if (App->GetDT() >= 0.5f) 
	{
		speed = 0.f;
	}

  
	state = AnimationType::WALK;
	if (Mix_Playing(3) == 0) 
  {
		App->entityManager->FxUnits(3, App->entityManager->Walking_troops, position.x, position.y);
	}
	
	iPoint currentIso = position + increment;

	iPoint targetIso = App->map->MapToWorld(targetPosition.x, targetPosition.y);
	targetIso += App->map->GetTilesHalfSize();

	App->render->DrawLine(position.x, position.y, targetIso.x, targetIso.y, 255, 0, 0);

	fPoint cast = { (float)targetIso.x, (float)targetIso.y };

	increment = { directionToTarget.x * speed,  directionToTarget.y * speed };

	position = position + increment;

	//state = AnimationType::WALK;

	if (position.DistanceManhattan(cast) <= 5)
	{
		position = App->map->MapToWorld((float)targetPosition.x, (float)targetPosition.y);
		position += App->map->GetTilesHalfSizeFloat();

		targetPosition.ResetAsPosition();
		if (entPath.size() <= 0)
		{
			ChangeState(targetPosition, AnimationType::IDLE);
		}
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

		iPoint rest = {(int)position.x, (int)position.y};

		iPoint fTarget = App->map->MapToWorld(targetPosition.x, targetPosition.y);
		fTarget += App->map->GetTilesHalfSize();

		directionToTarget = fTarget - rest;

		ChangeState(targetPosition, AnimationType::WALK);
		entPath.erase(entPath.begin(), entPath.begin() + 1);
	}


	if (targetPosition != iPoint(-1, -1))
		MoveToTarget();

	int num_current_anim = currentAnim.GetSprite();
	blitRect = { (int)(currentAnim.sprites[num_current_anim].rect.w / 1.5f), (int)(currentAnim.sprites[num_current_anim].rect.h / 1.5f) };

	//Collider update
	collisionRect.x = position.x - (collisionRect.w / 2);
	collisionRect.y = position.y;

	App->render->Blit(texture, position.x - blitRect.x / 2, position.y - blitRect.y, blitRect, &currentAnim.sprites[num_current_anim].rect, 1.f, flipState);

	if (displayDebug) 
	{
		if (entPath.size() > 0)
		{
			for (uint i = 0; i < entPath.size(); ++i)
			{
				iPoint pos = App->map->MapToWorld(entPath.at(i).x, entPath.at(i).y);
				App->render->Blit(App->scene->debugBlue_tex, pos.x, pos.y);
			}
		}
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


