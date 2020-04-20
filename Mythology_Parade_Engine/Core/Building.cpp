#include "Building.h"

Building::Building(BuildingType buildingType, iPoint pos, BuildingInfo info)
{
	damage = 25;
	SetMaxHealth(500);
	defenses = 500;
	maxCap = 1;
	position = pos;
	buildingStatus = CONSTRUCTING;
	percentage_constructing = 0;
	first_time_constructing = true;

	switch (buildingType)
	{
	case FORTRESS:
		time_construction = 0;
		break;
	case MONASTERY:
		time_construction = 60;
		break;
	case TEMPLE:
		time_construction = 90;
		break;
	case ENCAMPMENT:
		time_construction = 90;
		break;
	default:
		break;
	}
	description = "I'm a fortress";
	SetMaxHealth(300);


	civilization = info.civilization;
	original_spriteRect = spriteRect = info.spriteRect;
	blitRect = info.blitSize;
	//buildingType = info.buildingType;
	tileLenght = info.tileLenght;

	timer_construction.Start();
}

Building::~Building()
{
}

const char* Building::GetDescription()
{
	return description.c_str();
}

void Building::CreateUnit(BuildingType type)
{
	switch (type)
	{
	case FORTRESS:
		break;
	case MONASTERY:
		App->entityManager->CreateUnitEntity(UnitType::MONK, { position.x + 5,position.y });
		break;
	case TEMPLE:
		break;
	case ENCAMPMENT:
		App->entityManager->CreateUnitEntity(UnitType::ASSASSIN, { position.x + 5,position.y });
		break;
	}
}

bool Building::Awake(pugi::xml_node& a)
{


	return true;
}

bool Building::Update(float dt)
{
	bool ret = true;
	if (first_time_constructing == true && buildingStatus == CONSTRUCTING)
	{
		int actual_construction_time = timer_construction.ReadSec();
		if (actual_construction_time >= time_construction)
		{
			buildingStatus = FINISHED;
			percentage_constructing = 1;
			first_time_constructing = false;
		}
		else
		{
			percentage_constructing = (float)actual_construction_time / (float)time_construction;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		buildingStatus = DESTROYED;
		first_time_constructing = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		buildingStatus = CONSTRUCTING;
	}
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		buildingStatus = FINISHED;
		first_time_constructing = false;
	}
	int blitWidth;
	if(buildingStatus==DESTROYED)
	{
		spriteRect = App->entityManager->destructedSpriteRect;

		blitWidth = tileLenght * App->map->data.tile_width;
		blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	}
	else if (buildingStatus==CONSTRUCTING)
	{
		spriteRect = App->entityManager->constructorSpriteRect;

		blitWidth = tileLenght * App->map->data.tile_width;
		blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	}
	else if (buildingStatus==FINISHED)
	{
		spriteRect = original_spriteRect;

		blitWidth = tileLenght * App->map->data.tile_width;
		blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	}

	Draw();
	if (buildingStatus == CONSTRUCTING) {
		Draw_Construction_Bar(blitWidth);
	}

	return ret;
}

void Building::Draw()
{
	//lengh = 4, lenght is the number of tiles this building uses
	//App->render->DrawQuad({position.x, position.y + (tileHeight /2) * (height + 1), texturewidth, -textureHeight}, 255, 250, 20);
	App->render->Blit(texture, position.x, position.y + ((32 / 2) * tileLenght) - blitRect.y, {blitRect.x, blitRect.y}, &spriteRect);
	App->render->DrawQuad({position.x, position.y, 4, -4}, 255, 0, 0);
}

void Building::Draw_Construction_Bar(int blitWidth)
{
	SDL_Rect construction_spriteRect = App->entityManager->construction_bar_back;
	App->render->Blit(texture, position.x+ 0.15* blitWidth, position.y + ((32 / 2) * tileLenght) - 1.25*blitRect.y, &construction_spriteRect);
	construction_spriteRect = App->entityManager->construction_bar_front;
	int sprite_rect_width = percentage_constructing * construction_spriteRect.w;
	App->render->Blit(texture, position.x+ 0.15* blitWidth+3, position.y + ((32 / 2) * tileLenght) - 1.25*blitRect.y+3, { sprite_rect_width, construction_spriteRect.h },
		&construction_spriteRect);
}