#ifndef __LEVELSYSTEM_H__
#define __LEVELSYSTEM_H__

enum Action
{
	killEnemy,
	destroyEnemyBuilding,
	convertEnemyUnit,
	convertEnemyBuilding
};

struct LevelSystem
{

private:
	//reach experience
	int exp_to_lvl_1;
	int exp_to_lvl_2;
	int exp_to_lvl_3;

	int experience;
	int level;

public:
	void Init(int, int, int);

	void GainExperience(Action value);
	int GetLevel();
};

#endif // !__LEVELSYSTEM__H_