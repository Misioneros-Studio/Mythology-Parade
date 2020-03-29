#include "Building.h"

Building::Building(BuildingType buildingType, iPoint pos)
{
	damage = 25;
	SetMaxHealth(500);
	defenses = 500;
	maxCap = 1;
	position = pos;

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
	//lengh = 4, lenght is the number of tiles this building uses
	//App->render->DrawQuad({position.x, position.y + (tileHeight /2) * (height + 1), texturewidth, -textureHeight}, 255, 250, 20);
	App->render->DrawQuad({position.x, position.y + 16 * 5, 64*4, -32*8}, 255, 250, 20);
}
