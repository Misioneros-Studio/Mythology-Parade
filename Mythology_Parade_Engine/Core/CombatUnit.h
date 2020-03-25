#ifndef __COMBATUNIT_H__
#define __COMBATUNIT_H__

#include "Unit.h"
#include "LevelSystem.h"


class CombatUnit :	public Unit, public LevelSystem
{
private:
	int damage;
	int range;

public:
	CombatUnit(UnitType);
	~CombatUnit();

	void Action(Entity*) override;

private:
	void Init(int maxHealth, int damage, int range, int speed);
	int GetDamageValue();
	int GetRangeValue();
};

#endif // !__COMBATUNIT_H__