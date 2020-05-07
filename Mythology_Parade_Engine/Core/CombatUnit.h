#ifndef __COMBATUNIT_H__
#define __COMBATUNIT_H__

#include "Unit.h"
#include "LevelSystem.h"
#include"Animation.h"

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
	
};

#endif // !__COMBATUNIT_H__
