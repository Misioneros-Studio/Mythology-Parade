#ifndef __COMBATUNIT_H__
#define __COMBATUNIT_H__

#include "Unit.h"
#include "LevelSystem.h"


class CombatUnit :	public Unit, public LevelSystem
{
public:
	int damage;
	int range;

public:
	CombatUnit(UnitType);
	~CombatUnit();

	void Action() override;

};

#endif // !__COMBATUNIT_H__
