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
	BASIC_RESEARCH,
	BASIC_BUILDINGS_CREATION,
	RESEARCH_CLERIC,
	BASIC_UNITS_CREATION,
	EXPLORE
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
	bool ResearchFirstLv();
	bool CreateReligiousBuilds();
	bool CreateUnit(std::string, int);
	bool WantToResearch(std::string);
	bool InitExplore();
	bool CheckExplore();
	bool Assemble();
	bool CreateMonastery(int);
	bool Defense();
	bool Atack();
	bool Win();

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

};


#endif // !_IA_
