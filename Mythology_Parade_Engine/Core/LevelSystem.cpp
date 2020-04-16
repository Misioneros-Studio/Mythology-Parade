#include "LevelSystem.h"

void LevelSystem::Init(int lvl1, int lvl2, int lvl3)
{
	exp_to_lvl_1 = lvl1;
	exp_to_lvl_2 = lvl2;
	exp_to_lvl_3 = lvl3;

	experience = 0;
	level = 0;
}

void LevelSystem::GainExperience(Action action)
{
	if (level >= 3)
		return;

	//Continue
	int value = 0;
	switch (action)
	{
	case killEnemy:				value = 300;	break;
	case destroyEnemyBuilding:	value = 500;	break;
	case convertEnemyUnit:		value = 300;	break;
	case convertEnemyBuilding:	value = 500;	break;
	}

	experience += value;

	
	if (experience >= exp_to_lvl_1) {
		level++;
	}

	else if (experience >= exp_to_lvl_2)
	{
		level++;
	}

	else if (experience >= exp_to_lvl_3)
	{
		level++;
	}

	
}

int LevelSystem::GetLevel()
{
	return level;
}

int LevelSystem::GetExperience()
{
	return experience;
}
