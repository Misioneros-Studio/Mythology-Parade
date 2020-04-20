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
enum BuildingStatus
{
	CONSTRUCTING,
	FINISHED,
	DESTROYED
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
	void CreateUnit(BuildingType);
private:

	bool Awake(pugi::xml_node&);
	bool Update(float dt) override;
	void Draw_Construction_Bar(int blitWidth);
	void Draw();

	//Stats
	int defenses;
	int influence;
	int damage;
	int maxCap;
	int time_construction;

	std::string description;

	//Used when constructing
	float percentage_constructing;
	j1Timer timer_construction;
	bool first_time_constructing;
	

	//Settigns
	BuildingType buildingType;
	BuildingStatus buildingStatus;
	int tileLenght;
	SDL_Rect original_spriteRect;



};

#endif // !__BUILDING_H__
