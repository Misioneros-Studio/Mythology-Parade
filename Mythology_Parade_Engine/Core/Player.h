#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Entity.h"
#include "CurrencySystem.h"
#include "Unit.h"
#include "Building.h"

class Player : public Entity, public CurrencySystem
{
public:
	Player();
	~Player();

private:
	CurrencySystem currencySystem;

	std::list<Unit*> unitsList;
	std::list<Building*> buildingsList;
};



#endif // !__PLAYER_H__

