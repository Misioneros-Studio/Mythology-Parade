#include "CombatUnit.h"
#include "p2Log.h"
CombatUnit::CombatUnit(UnitType type): Unit(type), range(0),damage(0)
{
	//TODO 10: Change textures
	switch (unitType)
	{
	case ASSASSIN:
		//Change texture
		LevelSystem::Init(3500, 6500, 9500);
		damage = 15;
		range = 1;
		SetMaxHealth(100);
		SetMoveSpeed(4);
		break;
	case PIKEMAN:
		//Change Texture
		LevelSystem::Init(3000, 6000, 9500);
		range = 1;
		damage = 25;
		SetMaxHealth(110);
		SetMoveSpeed(2);
		break;
	}
}

CombatUnit::~CombatUnit()
{
}

void CombatUnit::Action()
{
	switch (unitType)
	{
	case ASSASSIN:
	LOG("I'm a assassin unit!");
		break;
	case PIKEMAN:
	LOG("I'm a pikeman unit!");
		break;
	
	}
	//Attack enemy
}
