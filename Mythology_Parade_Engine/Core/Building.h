#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "HealthSystem.h"

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
	Building(BuildingType, iPoint);
	~Building();
	
	const char* GetDescription();
private:

	//Stats
	int defenses;
	int influence;
	int damage;
	int maxCap;

	std::string description;

	bool Awake(pugi::xml_node&);
	bool Update(float dt) override;
	void Draw();

};

#endif // !__BUILDING_H__
