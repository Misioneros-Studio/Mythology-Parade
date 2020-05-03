#include "CombatUnit.h"
#include "p2Log.h"

CombatUnit::CombatUnit(UnitType type, iPoint pos) : Unit(type, pos), range(0), damage(0)
{
	//TODO 10: Change textures
	unitType = type;
	position = {(float)pos.x, (float)pos.y};
	switch (unitType)
	{
	case UnitType::ASSASSIN:
		time_production = 90;
		time_research = 0;
		researched = true;
		//Change texture
		LevelSystem::Init(3500, 6500, 9500);
		CombatUnit::Init(100, 15, 1, 80);
		break;
	case UnitType::PIKEMAN:
		time_production = 90;
		time_research = 70;
		researched = false;
		//Change Texture
		LevelSystem::Init(3000, 6000, 9500);
		CombatUnit::Init(110, 25, 1, 40);
		break;
	case UnitType::EXPLORER:
		break;
	case UnitType::PRIEST:
		break;
	case UnitType::FOOTMAN:
		break;
	case UnitType::JOTNAR:
		time_production = 7;
		time_research = 210;
		researched = true;
		//Change texture
		LevelSystem::Init(-1, -1, -1);
		CombatUnit::Init(150, 0, 1, 80);
		break;
	case UnitType::DRAUGAR:
		time_production = 7;
		time_research = 210;
		researched = true;
		//Change texture
		LevelSystem::Init(-1, -1, -1);
		CombatUnit::Init(40, 0, 1, 80);
		break;
	case UnitType::CYCLOP:
		time_production = 7;
		time_research = 210;
		researched = true;
		//Change texture
		LevelSystem::Init(-1, -1, -1);
		CombatUnit::Init(150, 0, 1, 80);
		break;
	case UnitType::MINOTAUR:
		time_production = 7;
		time_research = 210;
		researched = true;
		//Change texture
		LevelSystem::Init(-1, -1, -1);
		CombatUnit::Init(40, 0, 1, 80);
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

