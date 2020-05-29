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
	DISTRIBUTION
};

enum class LateGameBehaviour
{
	ATACK,
	ECONOMY_FOCUS,
	DEFENSE,
	WIN
};

class IA : public j1Module
{
public:
	IA();
	~IA() {};
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	bool InitCiv();
	bool CreateBuilding(BuildingType, iPoint);
	Unit* CreateUnit(UnitType, iPoint);
	bool CheckExplore();
	bool Find();
	bool Defense();
	bool Atack();
	bool Win();
	bool MoveUnity(iPoint, std::string, Unit* u = nullptr);
	
public:
	void Explore1();
	void Explore2();
	void AssembleClerics();

public:
	void EarlyGame();
	void MidGame();
	void LateGame();


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
};

enum class EarlyMovements
{
	MONASTERY,
	TEMPLE,
	CLERIC1,
	CLERIC2,
	CLERIC3,
	MONK1,
	MONK2,
	MONK3,
	CLERIC1POS,
	CLERIC2POS,
	CLERIC3POS,
	HOME
};


#endif // !_IA_
