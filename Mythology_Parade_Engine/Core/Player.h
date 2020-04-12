#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Entity.h"
#include "CurrencySystem.h"
#include "Unit.h"
#include "Building.h"
#include "j1Input.h"
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

private:
	CurrencySystem currencySystem;

	std::list<Unit*> unitsList;
	//std::list<Building*> buildingsList;
	std::string faith, sacrifice, prayer;

	int tick1 = 0, tick2 = 0;

public:
	bool player_win = false;
	bool player_lose = false;
	CivilizationType player_type;
<<<<<<< HEAD


=======
  
>>>>>>> parent of 5bdf825... Merge branch 'Feature/Units_v2' into Feature/Pathfinding
};



#endif // !__PLAYER_H__

