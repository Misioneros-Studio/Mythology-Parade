#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "HealthSystem.h"
#include"EntityManager.h"
#include "j1Timer.h"

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

class Building: public Entity, public HealthSystem
{
public:
	Building(BuildingType, iPoint, BuildingInfo);
	~Building();
	
	const char* GetDescription();
	BuildingType GetBuildingType() { return buildingType; }
	int GetInfluence() { return influence; }
	int GetDamage() { return damage; }
	int GetMaxCap() { return maxCap; }

	void StartProducing(int time, std::string thing_producing);
	void StartResearching(int time, std::string thing_producing);

	void SetTimeProducing(int time);

	void CreateUnit();

	void CreateUnitQueue(int time, std::string thing_producing);

private:

	bool Awake(pugi::xml_node&);
	bool Update(float dt) override;

	void Draw_Building_Bar(int blitWidth, int bar_used = 0, bool building_active = false, bool enemy = false);
	bool Draw(float dt);

	void FinishProduction(std::string thing_produced);

	//Stats
	int defenses;
	int max_defenses;
	int influence;
	int damage;
	int maxCap;
	int time_construction;

	int time_research;
	int time_producing;
	bool researched;

	int unitsToCreate;

	int nearbyMonks;
	std::string description;

	//Used when constructing/producing
	float percentage_constructing;
	j1Timer timer_construction;
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

public:
	BuildingStatus buildingStatus;
	BuildingAction buildingAction;



};

#endif // !__BUILDING_H__
