#include "CombatUnit.h"
#include "p2Log.h"

CombatUnit::CombatUnit(UnitType type, iPoint pos): Unit(type), range(0),damage(0)
{
	//TODO 10: Change textures
	unitType = type;
	position = pos;
	state = AnimationType::IDLE;
	switch (unitType)
	{
	case UnitType::ASSASSIN:
		//Change texture
		LevelSystem::Init(3500, 6500, 9500);
		CombatUnit::Init(100, 15, 1, 4);
		break;
	case UnitType::PIKEMAN:
		//Change Texture
		LevelSystem::Init(3000, 6000, 9500);
		CombatUnit::Init(110, 25, 1, 2);
		break;
	}
}

CombatUnit::~CombatUnit()
{
}

void CombatUnit::Action(Entity* entity)
{
	switch (unitType)
	{
	case UnitType::ASSASSIN:
	LOG("I'm a assassin unit!");
		break;
	case UnitType::PIKEMAN:
	LOG("I'm a pikeman unit!");
		break;
	
	}
	//Attack enemy
	Unit* target = (Unit*)entity;
	target->RecieveDamage(GetDamageValue());
}



void CombatUnit::Init(int maxHealth, int damage, int range, int speed)
{
	Unit::Init(maxHealth);
	this->damage = damage;
	this->range = range;
	SetMoveSpeed(speed);

	//TODO: Convert to a get funtions
	currentDirection = getMovementDirection(targetPosition);
	currentAnim = App->entityManager->animations[unitType][state][currentDirection];

}

bool CombatUnit::Update(float dt) 
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		targetPosition = App->map->GetMousePositionOnMap();
		currentDirection = getMovementDirection(targetPosition);
		currentAnim = App->entityManager->animations[unitType][AnimationType::WALK][currentDirection];
	}

	if (targetPosition != iPoint(0, 0))
		MoveToTarget();


	int num_current_anim = currentAnim.GetSprite();
	blitRect = { currentAnim.sprites[num_current_anim].rect.w, currentAnim.sprites[num_current_anim].rect.h };

	App->render->Blit(texture, position.x - currentAnim.sprites[num_current_anim].rect.w / 3, position.y - currentAnim.sprites[num_current_anim].rect.h + 16, blitRect, &currentAnim.sprites[num_current_anim].rect, 1.f, flipState);
	
	iPoint draw = App->map->MapToWorld(targetPosition.x, targetPosition.y);
	App->render->DrawQuad({draw.x, draw.y, 64, 32}, 255, 0, 0);
	return true;
}

int CombatUnit::GetDamageValue()
{
	return damage;
}

int CombatUnit::GetRangeValue()
{
	return range;
}
