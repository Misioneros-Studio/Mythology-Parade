#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Entity.h"
#include "CurrencySystem.h"
#include "Unit.h"
#include "Building.h"
#include"p2Log.h"
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
	Building* GetSelectedBuild();
	Building* GetEnemySelectedBuild();
	void ActionToUnit();
	void ActionToBuilding();

	int GetFaith();
	int GetPrayers();
	int GetSacrifices();

	void SetFaith(int var);
	void SetPrayers(int var);
	void SetSacrifices(int var);

private:
	std::string faith, sacrifice, prayer;

	int tick1 = 0, tick2 = 0;
	iPoint preClicked;
	iPoint postClicked;
	std::list<Entity*> listEntities;
	Entity* buildingSelect;
	Entity* enemyBuildingSelect;

	bool oneTime;
	iPoint click;


public:
	bool dontSelect;
	bool player_win = false;
	bool player_lose = false;
	int num_monastery;
	int num_temple;
	int num_encampment;
	int time_production_victory;
	CivilizationType player_type;
	bool research_temple;
	bool research_encampment;
	bool research_cleric;
	bool research_assassin;
	bool research_lawful_beast;
	bool research_chaotic_beast;
	bool research_lawful_miracle;
	bool research_chaotic_miracle;
	bool research_lawful_victory;
	bool research_chaotic_victory;

};



#endif // !__PLAYER_H__

