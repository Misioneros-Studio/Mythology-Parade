#include "CombatUnit.h"
#include "p2Log.h"

CombatUnit::CombatUnit(UnitType type, iPoint pos) : Unit(type, pos), range(0), damage(0)
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
	case UnitType::EXPLORER:
		break;
	case UnitType::PRIEST:
		break;
	case UnitType::FOOTMAN:
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

void CombatUnit::LevelUp()
{
	LevelSystem::LevelUp();
	switch (unitType)
	{
	case UnitType::PIKEMAN:
		break;
	case UnitType::ASSASSIN:
		switch (GetLevel())
		{
		case 1:
			IncreaseDamage(5);
			break;
		case 2:
			IncreaseHealth(10);
			break;
		case 3:
			IncreaseSpeed(1);
		}
		break;
	case UnitType::EXPLORER:
		break;
	case UnitType::PRIEST:
		break;
	case UnitType::FOOTMAN:
		break;

	}
}



void CombatUnit::Init(int maxHealth, int damage, int range, int speed)
{
	Unit::Init(maxHealth);
	this->damage = damage;
	this->range = range;
	this->speed = speed;
	SetMoveSpeed(speed);

	//TODO: Convert to a get funtions
	targetPosition.ResetAsPosition();
	ChangeState(targetPosition, state);
}

bool CombatUnit::Update(float dt) 
{
	Unit::Update(dt);
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


int CombatUnit::GetSpeedValue()
{
	return speed;
}

void CombatUnit::IncreaseHealth(int value)
{
	HealthSystem::IncreaseHealth(value);
}

void CombatUnit::IncreaseSpeed(int value)
{
	speed += value;
}

void CombatUnit::IncreaseDamage(int value)
{
	damage += value;
}

