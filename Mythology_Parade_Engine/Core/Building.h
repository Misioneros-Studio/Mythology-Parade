#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "HealthSystem.h"
#include"EntityManager.h"

enum BuildingType 
{
	FORTRESS,
	MONASTERY,
	TEMPLE,
	ENCAMPMENT
};

class Building: public Entity, public HealthSystem
{
public:
	Building(BuildingType, iPoint, BuildingInfo);
	~Building();
	
	const char* GetDescription();
private:

	bool Awake(pugi::xml_node&);
	bool Update(float dt) override;
	void Draw();

	//Stats
	int defenses;
	int influence;
	int damage;
	int maxCap;

	std::string description;

	//Settigns
	BuildingType buildingType;
	int tileLenght;



};

#endif // !__BUILDING_H__
