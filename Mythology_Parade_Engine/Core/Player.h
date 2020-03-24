#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Entity.h"
#include "j1CurrencySystem.h"
#include "Unit.h"
#include "Building.h"

class Player : public Entity, public j1CurrencySystem
{
public:
	Player();
	~Player();

private:
	j1CurrencySystem currencySystem;

	std::list<Unit*> unitsList;
	std::list<Building*> buildingsList;
};



#endif // !__PLAYER_H__

