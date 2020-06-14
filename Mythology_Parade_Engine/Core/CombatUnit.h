#ifndef __COMBATUNIT_H__
#define __COMBATUNIT_H__

#include "Unit.h"
#include "LevelSystem.h"

class CombatUnit :	public Unit, public LevelSystem
{
private:
	int damage;
	int range;
	int speed;

public:
	CombatUnit(UnitType, iPoint);
	~CombatUnit();

	bool Update(float);
	void Action(Entity*) override;
	void LevelUp();
	void SetDamage(int d);

	int realDamage;

private:
	void Init(int maxHealth, int damage, int range, int speed);


public:
	int GetDamageValue();
	int GetRangeValue();
	int GetSpeedValue();
	void IncreaseHealth(int);
	void IncreaseSpeed(int);
	void IncreaseDamage(int);

	void SetDefaultHealth();

	void SetLevel(int);
	void SetHealth(int);


	void DetectNearbyEnemies();

	std::list<Entity*> nearbyDetectedList;
	Entity* enemyTarget;

};

#endif // !__COMBATUNIT_H__
