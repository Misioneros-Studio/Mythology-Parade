#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "EntityManager.h"
#include "j1Timer.h"
#include <queue>

enum BuildingType
{
	FORTRESS,
	MONASTERY,
	TEMPLE,
	ENCAMPMENT
};
enum class BuildingStatus
{
	CONSTRUCTING,
	FINISHED,
	DESTROYED
};
enum class BuildingAction
{
	RESEARCHING,
	PRODUCING,
	NOTHING
};

struct BuildingInfo;
class Building: public Entity
{
public:
	Building(BuildingType, iPoint, BuildingInfo);
	~Building();

	const char* GetDescription();
	BuildingType GetBuildingType() { return buildingType; }
	int GetInfluence() { return influence; }
	int GetDamage() { return damage; }
	int GetMaxCap() { return maxCap; }
	std::queue<std::string> GetProduction() { return queuedResearch; }

	void StartProducing(const std::string &thing_producing);
	void CancelProduction(int);
	void StartResearching(const std::string &thing_producing);

	void SetTimeProducing(int time);

	void CreateUnit();


	bool GetResearched();

	int GetTimeResearch();
	int GetTimeProducing();

	float GetPercentage();
	void SetPercentage(float var);

	std::string GetElementProducing();
	void ProduceQueue(std::string thing_producing, bool = false);
	void Convert();
private:

	bool Awake(pugi::xml_node&);
	bool Update(float dt) override;

	void Draw_Building_Bar(int blitWidth, int bar_used = 0, bool building_active = false, bool enemy = false);
	bool Draw(float dt);


	void Kill(iPoint) override;

	void FinishProduction(const std::string &thing_produced, bool cancelled = false);

	//Stats
	int influence;
	int damage;
	int maxCap;
	int time_construction;

	int time_research;
	int time_producing;
	bool researched;

	std::queue<std::string> queuedResearch;

	int nearbyMonks;
	int nearbyBeasts;
	std::string description;
	int mainDef;

	//Used when constructing/producing
	float percentage_constructing;
	bool first_time_constructing;
	std::string element_producing;

	//Used to show life
	float percentage_life;
	bool show_bar_for_damage;
	j1Timer damage_timer;

	//Settigns
	BuildingType buildingType;
	int tileLenght;
	SDL_Rect original_spriteRect;
	int blitWidth;

public:
	j1Timer timer_construction;
	BuildingStatus buildingStatus;
	BuildingAction buildingAction;
	bool researching;
	int index_researching;
};

#endif // !__BUILDING_H__
