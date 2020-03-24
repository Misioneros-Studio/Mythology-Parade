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
	Building(BuildingType);
	~Building();
	
	const char* GetDescription();
private:

	//Stats
	int defenses;
	int influence;
	int damage;
	int maxCap;

	std::string description;

	bool Update(float dt) override;
	void Draw();

};

#endif // !__BUILDING_H__
