#include "LevelSystem.h"
#include "p2Log.h"
void LevelSystem::Init(int lvl1, int lvl2, int lvl3)
{
	exp_to_lvl_1 = lvl1;
	exp_to_lvl_2 = lvl2;
	exp_to_lvl_3 = lvl3;

	experience = lvl1 + lvl2 +lvl3;
	level = 0;
	canLevelUp = false;
}

void LevelSystem::LevelUp()
{
	switch (level)
	{
	case 0:
		if (experience >= exp_to_lvl_1) 
		{
			level++;
			experience -= exp_to_lvl_1;
			LOG("LEVEL 1 UP");
		}
		break;
	case 1:
		if (experience >= exp_to_lvl_2)
		{
			level++;
			experience -= exp_to_lvl_2;
			LOG("LEVEL 2 UP");
		}
		break;
	case 2:
		if (experience >= exp_to_lvl_3)
		{
			level++;
			experience -= exp_to_lvl_3;
			canLevelUp = false;
			LOG("LEVEL 3 UP");
		}
		break;
	}	
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
		canLevelUp = true;
	}

	else if (experience >= exp_to_lvl_2)
	{
		canLevelUp = true;
	}

	else if (experience >= exp_to_lvl_3)
	{
		canLevelUp = true;
	}
	else {
		canLevelUp = false;
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

int LevelSystem::GetExperienceToLevel(int level) {
	if (level == 0) {
		return exp_to_lvl_1;
	}
	else if (level == 1) {
		return exp_to_lvl_2;
	}
	else if (level == 2) {
		return exp_to_lvl_3;
	}
}
