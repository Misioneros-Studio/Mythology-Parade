#ifndef _IA_
#define _IA_

#include "p2Point.h"
#include "j1Module.h"
#include "PugiXml/src/pugixml.hpp"
#include"j1Input.h"
#include "EntityManager.h"

enum class GameBehaviour
{
	EARLY,
	MID,
	LATE
};

enum class EarlyGameBehaviour
{
	CREATION,
	BASIC_BUILDINGS_CREATION,
	RESEARCH_CLERIC,
	BASIC_UNITS_CREATION,
	EXPLORE1,
	CHECKEXPLORER1,
	FIND
};

enum class MidGameBehaviour
{
	ASSEMBLE,
	CREATE_ECONOMY,
	RESEARCH_ASSASSIN,
	CREATE_ASSASSIN,
};

enum class LateGameBehaviour
{
	ATACK,
	ECONOMY_FOCUS,
	DEFENSE,
	WIN,
	FINISH
};

class IA : public j1Module
{
public:
	IA();
	~IA() {};

	void Init();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	bool InitCiv();
	bool CreateBuilding(BuildingType, iPoint);
	Unit* CreateUnit(UnitType, iPoint);
	bool CheckExplore();
	bool Find();
	bool Defense();
	bool Atack();
	bool Win();
	bool MoveUnit(iPoint, std::string, Unit* u = nullptr, int number = 0);
	void DoThingsBefore(int, int);

public:
	void Explore1();
	void Explore2();
	void AssembleClerics();

public:
	void EarlyGame();
	void MidGame();
	void LateGame();


public:
	j1Timer timer_ia;
	int time_ia;

private:
	GameBehaviour gamePhase;
	EarlyGameBehaviour early;
	MidGameBehaviour mid;
	LateGameBehaviour late;

	CivilizationType civilization;

	std::list<Entity*> listEntities;

	j1Timer timer;

	std::vector<iPoint> positionViking;
	std::vector<iPoint> positionGreek;
	iPoint mouse;

	Building* enemyFortress;
	bool loading;

};

enum class EarlyMovements
{
	MONASTERY,
	TEMPLE,
	CLERIC1,
	CLERIC2,
	MONK1,
	MONK2,
	MONK3,
	CLERIC1POS,
	CLERIC2POS,
	CLERIC3POS,
	HOME,

	ENCAMPMENT,
	ASSASSIN1,
	ASSASSIN2,
	ASSASSIN3,
	ASSASSIN4,
	ASSASSIN5,
	ASSASSIN6,
	ASSASSIN7,
	
	MONASTERY2,
	MONK4,
	MONK5,

};


#endif // !_IA_
