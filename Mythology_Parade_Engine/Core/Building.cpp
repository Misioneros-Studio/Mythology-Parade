#include "Building.h"

Building::Building(BuildingType buildingType)
{
	damage = 25;
	SetMaxHealth(500);
	defenses = 500;
	maxCap = 1;

	switch (buildingType)
	{
	case FORTRESS:
		break;
	case MONASTERY:
		break;
	case TEMPLE:
		break;
	case ENCAMPMENT:
		break;
	default:
		break;
	}
	description = "I'm a fortress";
	SetMaxHealth(300);
}

Building::~Building()
{
}

const char* Building::GetDescription()
{
	return description.c_str();
}

bool Building::Awake(pugi::xml_node& a)
{


	return true;
}

bool Building::Update(float dt)
{
	bool ret = true;

	Draw();

	return ret;
}

void Building::Draw()
{

}
