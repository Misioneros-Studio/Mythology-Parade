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

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	CurrencySystem currencySystem;

	std::list<Unit*> unitsList;
	std::list<Building*> buildingsList;
	std::string faith, sacrifice, prayer;

	int tick1 = 0, tick2 = 0;

	int x, y;
	SDL_Rect vertical1, vertical2, horizontal1, horizontal2;
};



#endif // !__PLAYER_H__

