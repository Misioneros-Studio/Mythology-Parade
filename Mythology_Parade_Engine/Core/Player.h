#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Entity.h"
#include "CurrencySystem.h"
#include "Unit.h"
#include "Building.h"
#include "EntityManager.h"

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

	void SelectionDraw_Logic();
	std::vector<Entity> entitiesInside();

private:
	CurrencySystem currencySystem;

	std::list<Unit*> unitsList;
	std::list<Building*> buildingsList;
	std::string faith, sacrifice, prayer;

	int tick1 = 0, tick2 = 0;

	iPoint preClicked;
	iPoint postClicked;
	SDL_Rect vertical1, vertical2, horizontal1, horizontal2;

	std::vector<Entity> listEntities;
};



#endif // !__PLAYER_H__

