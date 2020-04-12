#include "Building.h"

Building::Building(BuildingType buildingType, iPoint pos, BuildingInfo info)
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


	civilization = info.civilization;
	spriteRect = info.spriteRect;
	blitRect = info.blitSize;
	//buildingType = info.buildingType;
	tileLenght = info.tileLenght;
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

	//Dont delete, we need this to change the state to "building" or "destructed"
	//if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) 
	//{
	//	spriteRect = App->entityManager->destructedSpriteRect;

	//	int blitWidth = tileLenght * App->map->data.tile_width;
	//	blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	//}
	//if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	//{
	//	spriteRect = App->entityManager->constructorSpriteRect;

	//	int blitWidth = tileLenght * App->map->data.tile_width;
	//	blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	//}

	Draw();

	return ret;
}

void Building::Draw()
{
	App->render->Blit(texture, position.x, position.y + ((App->map->data.tile_height / 2) * tileLenght) - blitRect.y, {blitRect.x, blitRect.y}, &spriteRect);
	//App->render->DrawQuad({position.x, position.y, 4, -4}, 255, 0, 0);
}
