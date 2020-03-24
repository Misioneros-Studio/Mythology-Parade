#include "Building.h"

Building::Building(BuildingType buildingType)
{
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

	SetMaxHealth(300);
	description = "hola";
}

Building::~Building()
{
}
