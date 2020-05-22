#include "Building.h"
#include "p2Log.h"
Building::Building(BuildingType type, iPoint pos, BuildingInfo info)
{
	//default inits with none value
	damage = 0;
	defenses = 0;
	max_defenses = 0;
	influence = 0;
	maxCap = 0;
	nearbyMonks = 0;
	researched = false;
	time_construction = 0;
	time_research = 0;
	percentage_constructing = 0;
	time_producing = 0;
	first_time_constructing = true;
	unitsToCreate = 0;
	percentage_life = 0.f;
	/*---------------*/

	//inits with some values
	position = { (float)pos.x, (float)pos.y};
	buildingStatus = BuildingStatus::CONSTRUCTING;
	buildingAction = BuildingAction::NOTHING;

	buildingType = type;
	original_spriteRect = spriteRect = info.spriteRect;
	blitRect = info.blitSize;

	tileLenght = info.tileLenght;
	int blitWidth = tileLenght * App->map->data.tile_width;

	switch (buildingType)
	{
	case FORTRESS:
		this->buildingType = BuildingType::FORTRESS;
		time_construction =time_research = 0;
		damage = 25;
		SetMaxHealth(500);
		defenses = max_defenses = 500;
		influence = 20;
		maxCap = 1;
		description = "I'm a fortress";
		researched = true;
		name = "fortress";
		break;
	case MONASTERY:
		this->buildingType = BuildingType::MONASTERY;
		time_research = 60;
		time_construction = 180;
		damage = 15;
		SetMaxHealth(250);
		defenses = max_defenses = 250;
		influence = 10;
		maxCap = 5;
		description = "I'm a monastery";
		App->entityManager->getPlayer()->num_monastery++;
		researched = false;
		name = "monastery";
		break;
	case TEMPLE:
		this->buildingType = BuildingType::TEMPLE;
		time_research = 90;
		time_construction = 150;
		damage = 15;
		SetMaxHealth(200);
		defenses = max_defenses = 200;
		influence = 10;
		maxCap = 8;
		description = "I'm a temple";
		App->entityManager->getPlayer()->num_temple++;
		researched = false;
		name = "temple";
		break;
	case ENCAMPMENT:
		this->buildingType = BuildingType::ENCAMPMENT;
		time_research = 90;
		time_construction = 180;
		damage = 20;
		SetMaxHealth(350);
		defenses = max_defenses = 350;
		influence = 10;
		maxCap = 7;
		description = "I'm an encampment";
		App->entityManager->getPlayer()->num_encampment++;
		researched = false;
		name = "encampment";
		break;
	default:
		break;
	}
	HealthSystem::Init();
	collisionRect = { (int)position.x, (int)position.y + ((App->map->data.tile_height / 2) * tileLenght), blitRect.x, -blitRect.y };
	spriteRect = App->entityManager->constructorSpriteRect;
	blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	switch (buildingType)
	{
	case BuildingType::FORTRESS:
		spriteRect = original_spriteRect;
		blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
		break;
	}
	show_bar_for_damage = false;

	mainDef = defenses;
	original_spriteRect = spriteRect = info.spriteRect;
	blitRect = info.blitSize;


	//buildingType = info.buildingType;
	tileLenght = info.tileLenght;

	collisionRect = { (int)position.x, (int)position.y + ((App->map->data.tile_height / 2) * tileLenght), blitRect.x, -blitRect.y};

	timer_construction.Start();
}

Building::~Building()
{
}

const char* Building::GetDescription()
{
	return description.c_str();
}

void Building::CreateUnit()
{
	switch (buildingType)
	{
	case FORTRESS:
		break;
	case MONASTERY:
		App->entityManager->CreateUnitEntity(UnitType::MONK, { (int)position.x - 30, (int)position.y },civilization);
		if (Mix_Playing(4) == 0)
    {
			App->entityManager->FxUnits(6, App->entityManager->CreateMonk_sound, position.x, position.y);
		}
		break;
	case TEMPLE:
		break;
	case ENCAMPMENT:
		App->entityManager->CreateUnitEntity(UnitType::ASSASSIN, { (int)position.x - 20, (int)position.y },civilization);
		if(Mix_Playing(4) == 0)
    {
			App->entityManager->FxUnits(7, App->entityManager->CreateAssasin_sound, position.x, position.y);
		}

		break;
	}
}


bool Building::GetResearched()
{
	return researched;
}

int Building::GetTimeResearch()
{
	return time_research;
}

int Building::GetTimeProducing()
{
	return time_producing;
}

float Building::GetPercentage()
{
	return percentage_constructing;
}

void Building::SetPercentage(float var)
{
	percentage_constructing = var;
}

std::string Building::GetElementProducing()
{
	return element_producing;
}
void Building::CreateUnitQueue(int time, std::string thing_producing)
{
	unitsToCreate++;
	time_producing = time;
	element_producing = thing_producing;

}

bool Building::Awake(pugi::xml_node& a)
{

	return true;
}

bool Building::Update(float dt)
{
	bool ret = true;

	if (App->entityManager->getPlayer())
	{
		displayDebug = App->entityManager->getPlayer()->displayDebug;
	}
	else
	{
		displayDebug = false;
  }
	if (unitsToCreate > 0 && buildingAction == BuildingAction::NOTHING) {
		StartProducing(element_producing);
		unitsToCreate--;
	}

	if (App->scene->paused_game == true && timer_construction.isPaused() == false)
		timer_construction.Pause();
	else if (App->scene->paused_game == false && timer_construction.isPaused() == true)
		timer_construction.Resume();
	if (App->scene->paused_game && show_bar_for_damage == true && damage_timer.isPaused() == false)
		damage_timer.Pause();
	else if (damage_timer.isPaused() == true && App->scene->paused_game == false)
		damage_timer.Resume();
	if (show_bar_for_damage == true && damage_timer.ReadSec() > 2)
		show_bar_for_damage = false;
	percentage_life = (float)defenses / (float)max_defenses;
	if (percentage_life < 0)
		percentage_life = 0;
	if (damaged_now == true) {
		damage_timer.Start();
		damaged_now = false;
		show_bar_for_damage = true;
	}
	if (first_time_constructing == true && buildingStatus == BuildingStatus::CONSTRUCTING)
	{
		int actual_construction_time = timer_construction.ReadSec();
		if (actual_construction_time >= time_construction)
		{
			buildingStatus = BuildingStatus::FINISHED;
			percentage_constructing = 1;
			first_time_constructing = false;

		}
		else
		{
			percentage_constructing = (float)actual_construction_time / (float)time_construction;
		}
	}
	else if (buildingAction != BuildingAction::NOTHING) {
		int actual_construction_time = timer_construction.ReadSec();
		if (actual_construction_time >= time_producing)
		{

			if (buildingAction == BuildingAction::PRODUCING) {
				FinishProduction(element_producing);
			}
			else if(buildingAction== BuildingAction::RESEARCHING){
				App->scene->FinishResearching(element_producing);
			}
			buildingAction = BuildingAction::NOTHING;
			percentage_constructing = 1;

		}
		else
		{
			percentage_constructing = (float)actual_construction_time / (float)time_producing;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		buildingStatus = BuildingStatus::DESTROYED;
		first_time_constructing = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		buildingStatus = BuildingStatus::CONSTRUCTING;
	}
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		buildingStatus = BuildingStatus::FINISHED;
		first_time_constructing = false;
	}
	int blitWidth = tileLenght * App->map->data.tile_width;
	if(buildingStatus== BuildingStatus::DESTROYED)
	{
		spriteRect = App->entityManager->destructedSpriteRect;

		blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	}
	else if (buildingStatus== BuildingStatus::CONSTRUCTING)
	{
		spriteRect = App->entityManager->constructorSpriteRect;

		blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	}
	else if (buildingStatus== BuildingStatus::FINISHED)
	{
		spriteRect = original_spriteRect;

		blitRect = App->entityManager->CalculateBuildingSize(blitWidth, spriteRect.w, spriteRect.h);
	}

	//Draw();
	bool active_building = false;
	if (buildingStatus == BuildingStatus::CONSTRUCTING || buildingAction== BuildingAction::PRODUCING)
	{
		Draw_Building_Bar(blitWidth);
		active_building = true;
	}
	else if (buildingAction == BuildingAction::RESEARCHING)
	{
		Draw_Building_Bar(blitWidth, 2);
		active_building = true;
	}
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint point = App->render->ScreenToWorld(x, y);
	if ((isSelected() || (point.x >= collisionRect.x && point.x <= collisionRect.x + collisionRect.w && point.y <= collisionRect.y && point.y >= collisionRect.y + collisionRect.h) ||
		show_bar_for_damage == true) && buildingStatus != BuildingStatus::CONSTRUCTING) {
		bool enemy = false;
		if (civilization != App->entityManager->getPlayer()->civilization)
			enemy = true;
		Draw_Building_Bar(blitWidth, 1, active_building, enemy);

	}
	//IF MONASTERY DETECTS NEARBY MONKS,INCREASE FAITH
	if (buildingType == BuildingType::MONASTERY)
	{
		std::list<Entity*> list =  App->entityManager->entities[EntityType::UNIT];
		int count = 0;
		for each (Unit* var in list)
		{
			if (var->unitType == UnitType::MONK) {
				if(position.DistanceManhattan(var->position) < 300)
				count++;
			}
		}
		if (nearbyMonks != count)
		{
			nearbyMonks = count;
			App->entityManager->getPlayer()->IncreaseFaithRatio(nearbyMonks);
		}
	}

	//IF BUILDING DETECTS CYCLOP
	if (civilization == CivilizationType::GREEK)
	{
		std::list<Entity*> list = App->entityManager->entities[EntityType::UNIT];
		int count = 0;
		for each (Unit * var in list)
		{
			if (var->unitType == UnitType::CYCLOP) {
				if (position.DistanceManhattan(var->position) < 100)
					count++;
			}
		}
		defenses = mainDef;
		defenses += 50 * count;
	}

	//IF BUILDING DETECTS JOTNAR
	if (civilization == CivilizationType::VIKING)
	{
		std::list<Entity*> list = App->entityManager->entities[EntityType::UNIT];
		int count = 0;
		for each (Unit * var in list)
		{
			if (var->unitType == UnitType::JOTNAR) {
				if (position.DistanceManhattan(var->position) < 100)
					count++;
			}
		}
		defenses = mainDef;
		defenses += 50 * count;
	}

	std::vector<iPoint> tiles;
	App->fowManager->GetTilesInsideRadius(fowRadius, position, { collisionRect.w / 2, collisionRect.h / 4}, tiles);
	App->fowManager->ApplyMaskToTiles(fowRadius, tiles);

	return ret;
}

bool Building::Draw(float dt)
{
	//lengh = 4, lenght is the number of tiles this building uses
	//App->render->DrawQuad({position.x, position.y + (tileHeight /2) * (height + 1), texturewidth, -textureHeight}, 255, 250, 20);
	App->render->Blit(texture, position.x, position.y + ((App->map->data.tile_height / 2) * tileLenght) - blitRect.y, {blitRect.x, blitRect.y}, &spriteRect);

	if (displayDebug)
	{
		App->render->DrawQuad(collisionRect, 255, 0, 0, 50);
	}

	return true;
}

void Building::Draw_Building_Bar(int blitWidth, int bar_used, bool building_active, bool enemy)
{
	SDL_Rect construction_spriteRect = App->entityManager->construction_bar_back;
	iPoint pos;
	if (blitWidth == 128)
		pos.x = (int)position.x + 1;
	else if (blitWidth == 192)
		pos.x = (int)position.x + 33;
	else
		pos.x = (int)position.x;
	if (buildingStatus == BuildingStatus::CONSTRUCTING)
		pos.y = (int)position.y + (int)(((32 / 2) * tileLenght) - 1.25 * blitRect.y);
	else if (building_active == false)
		pos.y = (int)collisionRect.y + (int)collisionRect.h - 27;
	else
		pos.y = (int)collisionRect.y + (int)collisionRect.h - 54;

	App->render->Blit(texture, pos.x, pos.y, &construction_spriteRect);
	if (bar_used == 0)
		construction_spriteRect = App->entityManager->construction_bar_front;
	else if (bar_used == 1) {
		if(enemy==true)
			construction_spriteRect = App->entityManager->life_bar_front_enemy;
		else
			construction_spriteRect = App->entityManager->life_bar_front;
	}
	else if(bar_used==2)
		construction_spriteRect = App->entityManager->research_bar_front;
	int sprite_rect_width;
	if (bar_used != 1)
		sprite_rect_width = percentage_constructing * construction_spriteRect.w;
	else
		sprite_rect_width = percentage_life * construction_spriteRect.w;
	App->render->Blit(texture, pos.x + 11, pos.y + 3, { sprite_rect_width, construction_spriteRect.h },
		&construction_spriteRect);
	construction_spriteRect = App->entityManager->construction_bar_empty;
	App->render->Blit(texture, pos.x,pos.y, &construction_spriteRect);
}


void Building::StartProducing(std::string thing_producing) {
	buildingAction = BuildingAction::PRODUCING;
	percentage_constructing = 0;
	if (thing_producing == "Prayers") time_producing = App->entityManager->getPlayer()->time_prayers;
	else if (thing_producing == "Sacrifices") time_producing = App->entityManager->getPlayer()->time_sacrifices;
	else if (thing_producing == "Victory") time_producing = App->entityManager->getPlayer()->time_production_victory;
	else if (thing_producing == "Monk") time_producing = 10;
	else if (thing_producing == "Assasin") time_producing = 10;
	element_producing = thing_producing;
	timer_construction.Start();

}

void Building::FinishProduction(std::string thing_produced)
{
	if (thing_produced == "Victory")
	{
		if (civilization == CivilizationType::VIKING)
		{
			App->entityManager->getPlayer()->player_win = true;
		}
		else
		{
			App->entityManager->getPlayer()->player_lose = true;
		}
	}
	else if(thing_produced == "Sacrifices")
	{
		App->entityManager->getPlayer()->sacrifices += 1;
	}
	else if(thing_produced == "Prayers")
	{
		App->entityManager->getPlayer()->prayers += 1;
	}
	else
	{
	CreateUnit();
	}
}

void Building::StartResearching(std::string thing_producing) {
	buildingAction = BuildingAction::RESEARCHING;
	percentage_constructing = 0;

	if (thing_producing == "Chaotic Miracle") time_producing = 240;
	else if (thing_producing == "Lawful Miracle") time_producing = 240;
	else if (thing_producing == "Chaotic Victory") time_producing = 420;
	else if (thing_producing == "Lawful Victory") time_producing = 420;
	else if (thing_producing == "Assassin") time_producing = 70;
	else if (thing_producing == "Cleric") time_producing = 70;
	else if (thing_producing == "Encampment") time_producing = 90;
	else if (thing_producing == "Temple") time_producing = 90;
	else if (thing_producing == "Chaotic Beast") time_producing = 210;
	else if (thing_producing == "Lawful Beast") time_producing = 210;

	element_producing = thing_producing;
	timer_construction.Start();
}

void Building::SetTimeProducing(int time)
{
	time_producing = time_construction = time;
}
