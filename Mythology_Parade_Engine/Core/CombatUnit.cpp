#include "CombatUnit.h"
#include "p2Log.h"

CombatUnit::CombatUnit(UnitType type, iPoint pos): Unit(type), range(0),damage(0)
{
	//TODO 10: Change textures
	unitType = type;
	position = pos;
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
	targetPosition = { 9, 16 };
	currentDirection = getMovementDirection(targetPosition);
	currentAnim = App->entityManager->animations[unitType][(AnimationType)3][currentDirection];

}

bool CombatUnit::Update(float dt) 
{
	int num_current_anim = currentAnim.GetSprite();

	App->render->Blit(texture, position.x - currentAnim.sprites[num_current_anim].rect.w / 3, position.y - currentAnim.sprites[num_current_anim].rect.h + 16, &currentAnim.sprites[num_current_anim].rect);
	App->render->DrawQuad({position.x, position.y, 2, 2}, 255, 0, 0);
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
