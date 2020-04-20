#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Entity.h"
#include "CurrencySystem.h"
#include "Unit.h"
#include "Building.h"

#include "j1Input.h"
#include "EntityManager.h"

class Unit;
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
	void SeeEntitiesInside();
	void PlayerInputs();
	void ClickLogic();
	std::list<Entity*> GetEntitiesSelected();

private:
	std::string faith, sacrifice, prayer;

	int tick1 = 0, tick2 = 0;
	iPoint preClicked;
	iPoint postClicked;
	std::list<Entity*> listEntities;
	Entity* buildingSelect;

	bool dontSelect;

public:
	bool player_win = false;
	bool player_lose = false;
	CivilizationType player_type;
  
};



#endif // !__PLAYER_H__

