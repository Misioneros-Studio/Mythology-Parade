#include "EntityManager.h"
#include "CombatUnit.h"
#include "Building.h"
#include "Player.h"
#include "j1Gui.h"

#include "p2Log.h"
EntityManager::EntityManager()
{
	name.append("entity_manager");
	buildingsData.reserve(MAX_BUILDING_TYPES);
	buildingTestIndex = 0;
	initCivilizations = true;
}

//Destructor
EntityManager::~EntityManager()
{}

//Called before render is available
bool EntityManager::Awake(pugi::xml_node& a)
{
	App->fowManager->RequestMaskGeneration(10);
	//Load buildings info
	pugi::xml_document buildings;
	buildings.load_file(a.child("buildings").attribute("file").as_string());
	LoadBuildingsData(buildings.child("map").child("objectgroup"));
	life_bar_front = { 1310,503,115,10 };
	life_bar_front_enemy = { 1310,483,115,10 };
	research_bar_front = { 1310,543,115,10 };
	construction_bar_back = { 1299,560,125,17 };
	construction_bar_front = { 1310, 523, 115, 10 };
	construction_bar_empty = { 1299,582,125,17 };
	unit_life_bar_back = { 1406,481,75,10 };
	unit_life_bar_empty = { 1406,494,75,10 };
	unit_life_bar_front = { 1413,470,63,6 };
	unit_life_bar_front_enemy = { 1327,470,63,6 };


	//Not working because renderer is not created yet ;-;
	//std::string path = "assets/buildings/";
	//path.append(buildings.child("map").child("imagelayer").child("image").attribute("source").as_string());
	//tempBuildingTexture = App->tex->Load(path.c_str());


	//INFO: This is a good way to itinerate all the map, to itinerate only items in one key, use only the second for loop
	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			Entity* ent = it._Ptr->_Myval;
			ent->Awake(a.child(ent->name.c_str()));
		}
	}
	active = false;

	

	return true;
}

// Called before the first frame
bool EntityManager::Start()
{
	//TODO: NO HARDCODE BOY
	entitySpriteSheets[SpriteSheetType::BUILDINGS] = App->tex->Load("assets/buildings/Buildings.png");

	animations[UnitType::ASSASSIN] = animationManager.Load("assets/units/Assassin.tmx", UnitType::ASSASSIN);
	animations[UnitType::MONK] = animationManager.Load("assets/units/Monk.tmx", UnitType::MONK);
	animations[UnitType::PRIEST] = animationManager.Load("assets/units/Priest.tmx", UnitType::PRIEST);
	animations[UnitType::DRAUGAR] = animationManager.Load("assets/units/Draugar.tmx", UnitType::DRAUGAR);
	animations[UnitType::JOTNAR] = animationManager.Load("assets/units/Jotnar.tmx", UnitType::JOTNAR);
	animations[UnitType::CYCLOP] = animationManager.Load("assets/units/Cyclop.tmx", UnitType::CYCLOP);
	animations[UnitType::MINOTAUR] = animationManager.Load("assets/units/Minotaur.tmx", UnitType::MINOTAUR);


	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			it._Ptr->_Myval->Start();
		}
	}

	return true;
}

void EntityManager::LoadBuildingsBlitRect() 
{
	for (unsigned int i = 0; i < buildingsData.size(); i++)
	{
		BuildingInfo* info = &buildingsData[i];
		int blitWidth = info->tileLenght * App->map->data.tile_width;
		info->blitSize = CalculateBuildingSize(blitWidth, info->spriteRect.w, info->spriteRect.h);
	}
}

// Called each loop iteration
bool EntityManager::PreUpdate()
{
	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			it._Ptr->_Myval->PreUpdate();
		}
	}
	return true;
}

// Called each loop iteration
bool EntityManager::Update(float dt)
{
	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			(*it)->Update(dt);
		}
	}


	for (std::list<Entity*>::iterator it = entities[EntityType::UNIT].begin(); it != entities[EntityType::UNIT].end(); it++)
	{
		if ((*it)->type == EntityType::UNIT) 
		{
			Unit* tmp = (Unit*)it._Ptr->_Myval;
			if (tmp->toDelete) 
			{
				entities[EntityType::UNIT].erase(it);
				delete tmp;
			}

		}
	}
  
	//TODO: Move this logic to the player
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->scene->godMode==true)
	{
		EnterBuildMode();
	}
	
	if (crPreview.active == true && App->input->GetMouseButtonDown(3) == KEY_UP) {
		EnterBuildMode();
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->scene->godMode == true)
	{
		if (buildingTestIndex < MAX_BUILDING_TYPES - 1) 
		{
			buildingTestIndex++;
		}
		else
		{
			buildingTestIndex = 0;
		}
		UpdateBuildPreview(buildingTestIndex);
	}

	if (crPreview.active)
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		BuildingInfo build = buildingsData[buildingTestIndex];

		crPreview.canBuild = true;
		debugTex = App->scene->debugBlue_tex;
		
		for (int i = 0; i <= 1; i++)
		{
			for (int y = mouse.y; y > mouse.y - crPreview.height; y--)
			{
				for (int x = mouse.x; x < mouse.x + crPreview.width; x++)
				{
					if (i == 0)
					{
						if (crPreview.canBuild && App->pathfinding->IsWalkable({ x, y }) == false) 
						{
							debugTex = App->scene->debugRed_tex;
							crPreview.canBuild = false;
						}
					}
					else
					{
						if (IN_RANGE(x, 0, App->map->data.width - 1) == 1 && IN_RANGE(y, 0, App->map->data.height - 1) == 1)
						{
							iPoint p = App->map->MapToWorld(x, y);
							App->render->Blit(debugTex, p.x, p.y);
						}
					}
				}
			}
		}

		//Preview of build
		iPoint p = App->map->MapToWorld(mouse.x, mouse.y);
		App->render->Blit(entitySpriteSheets[SpriteSheetType::BUILDINGS], p.x, p.y + ((App->map->data.tile_height / 2) * build.tileLenght) - build.blitSize.y, { build.blitSize.x, build.blitSize.y }, &build.spriteRect);
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && crPreview.active && crPreview.canBuild)
	{
		int faithToDescrease = 0;
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->MapToWorld(mouse.x, mouse.y);
		spawnPos.y += App->map->data.tile_height / 2;
		switch (buildingTestIndex) 
		{
		case 0:
			CreateBuildingEntity(spawnPos, BuildingType::FORTRESS, buildingsData[buildingTestIndex], CivilizationType::VIKING);
			break;
		case 4:
			CreateBuildingEntity(spawnPos, BuildingType::FORTRESS, buildingsData[buildingTestIndex],CivilizationType::GREEK);
			break;
		case 1:
			CreateBuildingEntity(spawnPos, BuildingType::MONASTERY, buildingsData[buildingTestIndex], CivilizationType::VIKING);
			faithToDescrease = 200;
			break;
		case 5:
			CreateBuildingEntity(spawnPos, BuildingType::MONASTERY , buildingsData[buildingTestIndex],CivilizationType::GREEK);
			faithToDescrease = 200;
			break;
		case 2:
			CreateBuildingEntity(spawnPos, BuildingType::TEMPLE, buildingsData[buildingTestIndex], CivilizationType::VIKING);
			faithToDescrease = 200;
			break;
		case 6:
			CreateBuildingEntity(spawnPos, BuildingType::TEMPLE, buildingsData[buildingTestIndex], CivilizationType::GREEK);
			faithToDescrease = 200;
			break;
		case 3:
			CreateBuildingEntity(spawnPos, BuildingType::ENCAMPMENT, buildingsData[buildingTestIndex], CivilizationType::VIKING);
			faithToDescrease = 200;
			break;

		case 7:
			CreateBuildingEntity(spawnPos, BuildingType::ENCAMPMENT, buildingsData[buildingTestIndex], CivilizationType::GREEK);
			faithToDescrease = 200;
			break;
		}
		
		//Onces you build disable building mode
		App->entityManager->getPlayer()->DecreaseFaith(faithToDescrease);
		crPreview.active = false;
	}

	//Update FOW
	App->fowManager->MapNeedsUpdate();

	return true;
}



bool EntityManager::PostUpdate() 
{
	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	//Double for?
	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			Entity* ent = it._Ptr->_Myval;
			delete ent;
		}
		entities[(EntityType)i].clear();
	}
	for (unsigned int i = 0; i < entitySpriteSheets.size(); i++)
	{
		if(entitySpriteSheets[(SpriteSheetType)i])
			App->tex->UnLoad(entitySpriteSheets[(SpriteSheetType)i]);
	}
	entities.clear();

	for (unsigned int i = 0; i < animations.size(); i++)
	{
		for (unsigned int k = 0; k < animations[(UnitType)i].size(); k++)
		{
			for (unsigned int j = 0; j < animations[(UnitType)i][(AnimationType)k].size(); j++)
			{
				animations[(UnitType)i][(AnimationType)k][(Direction)j].Clean();
			}
			animations[(UnitType)i][(AnimationType)k].clear();
		}
		animations[(UnitType)i].clear();
	}
	animations.clear();

	return true;
}

////Called when loading the game
bool EntityManager::Load(pugi::xml_node& n)
{
	CivilizationType civ;
	entities.clear();

	//PLAYER LOADING
	pugi::xml_node p = n.child("players").first_child();
	if (p.name() == "viking") civ = CivilizationType::VIKING;
	else civ = CivilizationType::GREEK;
	Player* player = static_cast<Player*>(CreatePlayerEntity(p.name()));
	player->research_assassin = p.child("research").child("assassin").attribute("research").as_bool();
	player->research_chaotic_beast = p.child("research").child("chaotic_beast").attribute("research").as_bool();
	player->research_chaotic_miracle = p.child("research").child("chaotic_miracle").attribute("research").as_bool();
	player->research_chaotic_victory = p.child("research").child("chaotic_victory").attribute("research").as_bool();
	player->research_cleric = p.child("research").child("cleric").attribute("research").as_bool();
	player->research_encampment = p.child("research").child("encampment").attribute("research").as_bool();
	player->research_lawful_beast = p.child("research").child("lawful_beast").attribute("research").as_bool();
	player->research_lawful_miracle = p.child("research").child("lawful_miracle").attribute("research").as_bool();
	player->research_lawful_victory = p.child("research").child("lawful_victory").attribute("research").as_bool();
	player->research_temple = p.child("research").child("temple").attribute("research").as_bool();

	player->SetFaith(p.child("economy").attribute("faith").as_int());
	player->SetSacrifices(p.child("economy").attribute("sacrifices").as_int());
	player->SetPrayers(p.child("economy").attribute("prayers").as_int());


	//UNITS LOADING
	pugi::xml_node it = n.child("entities").child("unit");
	for (it; it; it = it.next_sibling("unit"))
	{
		iPoint pos;
		pos.x = it.attribute("position_x").as_int();
		pos.y = it.attribute("position_y").as_int();

		CivilizationType unit_civ;
		if (!strcmp(it.attribute("civilization").as_string(), "viking")) unit_civ = CivilizationType::VIKING;
		else unit_civ = CivilizationType::GREEK;

		if (!strcmp(it.attribute("type").as_string(), "monk")) {
			Unit* monk = static_cast<Unit*>(CreateUnitEntity(UnitType::MONK, pos, unit_civ));
			monk->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "assassin")) {
			CombatUnit* assassin = static_cast<CombatUnit*>(CreateUnitEntity(UnitType::ASSASSIN, pos, unit_civ));
			assassin->SetLevel(it.attribute("level").as_int());
			assassin->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "pikeman")) {
			CombatUnit* pikeman = static_cast<CombatUnit*>(CreateUnitEntity(UnitType::PIKEMAN, pos, unit_civ));
			pikeman->SetLevel(it.attribute("level").as_int());
			pikeman->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "explorer")) {
			CombatUnit* explorer = static_cast<CombatUnit*>(CreateUnitEntity(UnitType::EXPLORER, pos, unit_civ));
			explorer->SetLevel(it.attribute("level").as_int());
			explorer->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "priest")) {
			CombatUnit* priest = static_cast<CombatUnit*>(CreateUnitEntity(UnitType::PRIEST, pos, unit_civ));
			priest->SetLevel(it.attribute("level").as_int());
			priest->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "footman")) {
			CombatUnit* footman = static_cast<CombatUnit*>(CreateUnitEntity(UnitType::FOOTMAN, pos, unit_civ));
			footman->SetLevel(it.attribute("level").as_int());
			footman->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "cyclop")) {
			Unit* cyclop = static_cast<Unit*>(CreateUnitEntity(UnitType::CYCLOP, pos, unit_civ));
			cyclop->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "minotaur")) {
			Unit* minotaur = static_cast<Unit*>(CreateUnitEntity(UnitType::MINOTAUR, pos, unit_civ));
			minotaur->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "jotnar")) {
			Unit* jotnar = static_cast<Unit*>(CreateUnitEntity(UnitType::JOTNAR, pos, unit_civ));
			jotnar->SetHealth(it.attribute("health").as_int());
		}
		else if (!strcmp(it.attribute("type").as_string(), "draugar")) {
			Unit* draugar = static_cast<Unit*>(CreateUnitEntity(UnitType::DRAUGAR, pos, unit_civ));
			draugar->SetHealth(it.attribute("health").as_int());
		}
	}

	//BUILDINGS LOADING
	it = n.child("buildings").child("build");
	for (it; it; it = it.next_sibling("build"))
	{
		BuildingStatus status;
		BuildingAction action;
		CivilizationType build_civ;
		iPoint pos;
		int databuild;

		pos.x = it.attribute("position_x").as_int();
		pos.y = it.attribute("position_y").as_int();

		if (!strcmp(it.attribute("status").as_string(), "constructing")) status = BuildingStatus::CONSTRUCTING;
		else if (!strcmp(it.attribute("status").as_string(), "finished")) status = BuildingStatus::FINISHED;
		else status = BuildingStatus::DESTROYED;

		if (!strcmp(it.attribute("action").as_string(), "nothing")) action = BuildingAction::NOTHING;
		else if (!strcmp(it.attribute("action").as_string(), "producing")) action = BuildingAction::PRODUCING;
		else action = BuildingAction::RESEARCHING;


		if (!strcmp(it.attribute("type").as_string(), "monastery")) {
			if (!strcmp(it.attribute("civilization").as_string(), "viking")) { build_civ = CivilizationType::VIKING; databuild = 1; }
			else { build_civ = CivilizationType::GREEK; databuild = 5; }
			Building* monastery = static_cast<Building*>(CreateBuildingEntity(pos, BuildingType::MONASTERY, buildingsData[databuild], build_civ));
			monastery->SetHealth(it.attribute("health").as_int());
			if (status == BuildingStatus::CONSTRUCTING) {
				monastery->timer_construction.StartAt(it.attribute("time").as_float());
				monastery->SetPercentage(it.attribute("percentage").as_float());
			} //CARGAR PERCENTAGE CONSTRUCTING
			monastery->buildingStatus = status;
			monastery->buildingAction = action;
			if (action == BuildingAction::PRODUCING) { monastery->StartProducing(it.attribute("element").as_string()); monastery->timer_construction.StartAt(it.attribute("time").as_int()); }
			else if (action == BuildingAction::RESEARCHING) { monastery->StartResearching(it.attribute("element").as_string()); monastery->timer_construction.StartAt(it.attribute("time").as_int()); }

		}

		else if (!strcmp(it.attribute("type").as_string(), "temple")) {
			if (!strcmp(it.attribute("civilization").as_string(), "viking")) { build_civ = CivilizationType::VIKING; databuild = 2; }
			else { build_civ = CivilizationType::GREEK; databuild = 6; }
			Building* temple = static_cast<Building*>(CreateBuildingEntity(pos, BuildingType::TEMPLE, buildingsData[databuild], build_civ));
			temple->SetHealth(it.attribute("health").as_int());
			if (status == BuildingStatus::CONSTRUCTING) {
				temple->timer_construction.StartAt(it.attribute("time").as_float());
				temple->SetPercentage(it.attribute("percentage").as_float());
			}
			temple->buildingStatus = status;
			temple->buildingAction = action;
			if (action == BuildingAction::PRODUCING) { temple->StartProducing(it.attribute("element").as_string()); temple->timer_construction.StartAt(it.attribute("time").as_int()); }
			else if (action == BuildingAction::RESEARCHING) { temple->StartResearching(it.attribute("element").as_string()); temple->timer_construction.StartAt(it.attribute("time").as_int()); }

		}

		else if (!strcmp(it.attribute("type").as_string(), "encampment")) {
			if (!strcmp(it.attribute("civilization").as_string(), "viking")) { build_civ = CivilizationType::VIKING; databuild = 3; }
			else { build_civ = CivilizationType::GREEK; databuild = 7; }
			Building* encampment = static_cast<Building*>(CreateBuildingEntity(pos, BuildingType::ENCAMPMENT, buildingsData[databuild], build_civ));
			encampment->SetHealth(it.attribute("health").as_int());
			if (status == BuildingStatus::CONSTRUCTING) {
				encampment->timer_construction.StartAt(it.attribute("time").as_float());
				encampment->SetPercentage(it.attribute("percentage").as_float());
			}
			encampment->buildingStatus = status;
			encampment->buildingAction = action;
			if (action == BuildingAction::PRODUCING) { encampment->StartProducing(it.attribute("element").as_string()); encampment->timer_construction.StartAt(it.attribute("time").as_int()); }
			else if (action == BuildingAction::RESEARCHING) { encampment->StartResearching(it.attribute("element").as_string()); encampment->timer_construction.StartAt(it.attribute("time").as_int()); }

		}

		else if (!strcmp(it.attribute("type").as_string(), "fortress")) {

			if (!strcmp(it.attribute("civilization").as_string(), "viking")) { build_civ = CivilizationType::VIKING; databuild = 0; }
			else { build_civ = CivilizationType::GREEK; databuild = 4; }
			Building* fortress = static_cast<Building*>(CreateBuildingEntity(pos, BuildingType::FORTRESS, buildingsData[databuild], build_civ));
			fortress->SetHealth(it.attribute("health").as_int());
			if (status == BuildingStatus::CONSTRUCTING) {
				fortress->timer_construction.StartAt(it.attribute("time").as_float());
				fortress->SetPercentage(it.attribute("percentage").as_float());
			}
			fortress->buildingStatus = status;
			fortress->buildingAction = action;
			if (action == BuildingAction::PRODUCING) { fortress->StartProducing(it.attribute("element").as_string()); fortress->timer_construction.StartAt(it.attribute("time").as_int()); }
			else if (action == BuildingAction::RESEARCHING) { fortress->StartResearching(it.attribute("element").as_string()); fortress->timer_construction.StartAt(it.attribute("time").as_int()); }
		}
	}
	return true;
}

////Called when saving the game

bool EntityManager::Save(pugi::xml_node& s) const
{
	pugi::xml_node node = s.append_child("entities");
	std::list<Entity*> list = App->entityManager->entities[EntityType::UNIT];
	for each (Unit * var in list)
	{
		pugi::xml_node entity = node.append_child("unit");
		entity.append_attribute("type").set_value(var->name.c_str());
		entity.append_attribute("position_x").set_value(var->position.x);
		entity.append_attribute("position_y").set_value(var->position.y);


		if (var->civilization == CivilizationType::GREEK)
			entity.append_attribute("civilization").set_value("greek");
		else
			entity.append_attribute("civilization").set_value("viking");


		entity.append_attribute("health").set_value(var->GetHealth());

		if (var->canLevel)
		{
			CombatUnit* combatVar = (CombatUnit*)var;
			entity.append_attribute("level").set_value(combatVar->GetLevel());
		}
	}

	pugi::xml_node node2 = s.append_child("buildings");
	std::list<Entity*> list2 = App->entityManager->entities[EntityType::BUILDING];


	for each (Building * var2 in list2)
	{
		pugi::xml_node building = node2.append_child("build");

		building.append_attribute("type").set_value(var2->name.c_str());
		building.append_attribute("position_x").set_value(var2->position.x);
		building.append_attribute("position_y").set_value(var2->position.y);


		if (var2->civilization == CivilizationType::GREEK)
			building.append_attribute("civilization").set_value("greek");
		else
			building.append_attribute("civilization").set_value("viking");


		building.append_attribute("health").set_value(var2->GetHealth());


		if (var2->buildingStatus == BuildingStatus::CONSTRUCTING) {
			building.append_attribute("status").set_value("constructing");
			building.append_attribute("time").set_value(var2->timer_construction.ReadSec());
			building.append_attribute("progress").set_value(var2->GetPercentage());
		}
		else if(var2->buildingStatus == BuildingStatus::DESTROYED)
			building.append_attribute("status").set_value("destroyed");
		else
			building.append_attribute("status").set_value("finished");


		if(var2->buildingAction == BuildingAction::NOTHING)
			building.append_attribute("action").set_value("nothing");
		else if (var2->buildingAction == BuildingAction::PRODUCING) {
			building.append_attribute("action").set_value("producing");
			building.append_attribute("time").set_value(var2->GetTimeProducing());
			building.append_attribute("element").set_value(var2->GetElementProducing().c_str());
		}
		else {
			building.append_attribute("action").set_value("researching");
			building.append_attribute("time").set_value(var2->GetTimeProducing());
			building.append_attribute("element").set_value(var2->GetElementProducing().c_str());
		}
	}

	pugi::xml_node node3 = s.append_child("players");
	std::list<Entity*> list3 = App->entityManager->entities[EntityType::PLAYER];


	for each (Player * var3 in list3)
	{
		pugi::xml_node player;
		if(var3->civilization == CivilizationType::VIKING)
			 player = node3.append_child("viking");
		else if(var3->civilization == CivilizationType::GREEK) 
			player = node3.append_child("greek");

		pugi::xml_node economy = player.append_child("economy");
		economy.append_attribute("faith").set_value(var3->GetFaith());
		economy.append_attribute("prayers").set_value(var3->GetPrayers());
		economy.append_attribute("sacrifices").set_value(var3->GetSacrifices());


		pugi::xml_node research = player.append_child("research");
		research.append_child("temple").append_attribute("research").set_value(var3->research_temple);
		research.append_child("encampment").append_attribute("research").set_value(var3->research_encampment);
		research.append_child("cleric").append_attribute("research").set_value(var3->research_cleric);
		research.append_child("assassin").append_attribute("research").set_value(var3->research_assassin);
		research.append_child("lawful_beast").append_attribute("research").set_value(var3->research_lawful_beast);
		research.append_child("chaotic_beast").append_attribute("research").set_value(var3->research_chaotic_beast);
		research.append_child("lawful_miracle").append_attribute("research").set_value(var3->research_lawful_miracle);
		research.append_child("chaotic_miracle").append_attribute("research").set_value(var3->research_chaotic_miracle);
		research.append_child("lawful_victory").append_attribute("research").set_value(var3->research_lawful_victory);
		research.append_child("chaotic_victory").append_attribute("research").set_value(var3->research_chaotic_victory);
	}

	return true;
}

//Called when creating a new Entity
//Entity* EntityManager::CreateEntity(EntityType type, UnitType unitType)
//{
//	Entity* ret = nullptr;
	//pugi::xml_document	info_file;
	//pugi::xml_document info_file2;

	//switch (type)
	//{

	//case EntityType::PLAYER:
	//	ret = new Player();
	//	break;

	//case EntityType::UNIT:
	//	switch (unitType)
	//	{
	//	case ASSASSIN:
	//	ret = new CombatUnit(UnitType::ASSASSIN);
	//		break;
	//	case MONK:
	//	ret = new Unit(UnitType::MONK);
	//		break;
	//	case PIKEMAN:
	//	ret = new CombatUnit(UnitType::PIKEMAN);
	//		break;
	//	}
	//	break;

	//case EntityType::BUILDING:
	//	ret = new Building(BuildingType::FORTRESS);
	//	break;
	//}
	//entities[type].push_back(ret);

	//switch (type)
	//{
	//case Types::player:
	//	ret = new j1Player(Types::player);
	//	break;

	//case Types::enemy_ground:
	//	info_file.load_file("textures/Enemy_Sprites/snake.tmx");
	//	ret = new eSnakeEnemy(Types::enemy_ground, info_file.child("map"));
	//	break;

	//case Types::enemy_air:
	//	info_file.load_file("textures/Enemy_Sprites/bat.tmx");
	//	ret = new eBatEnemy(Types::enemy_air, info_file.child("map"));
	//	break;

	//case Types::healing_potion:
	//	//load healing potion
	//	ret = new ePotion(Types::healing_potion, info_file.child("map"));
	//	break;

	//case Types::coins:
	//	ret = new eCoins(Types::coins, info_file.child("map"));
	//	break;

	//}
	/*info_file2.load_file("config.xml");
	if (ret != nullptr)
	{
		entities.add(ret);
		if (type == Types::enemy_ground)
			ret->Awake(info_file2.child("config").child("entity_manager").child("enemy_info").child("ground_enemy"));
		else if (type == Types::enemy_air)
			ret->Awake(info_file2.child("config").child("entity_manager").child("enemy_info").child("fly_enemy"));
		else if (type == Types::healing_potion)
			ret->Awake(info_file2.child("config").child("entity_manager").child("pickups").child("potion"));
		else if (type == Types::coins)
			ret->Awake(info_file2.child("config").child("entity_manager").child("pickups").child("coins"));


	}*/
//	return ret;
//}

Entity* EntityManager::CreatePlayerEntity(std::string civilization_string)
{
	Entity* ret = nullptr;

	ret = new Player();
	ret->type = EntityType::PLAYER;
	Player* p = (Player*)ret;

	entities[EntityType::PLAYER].push_back(ret);
	entities[EntityType::PLAYER].begin()._Ptr->_Myval->Start();

	if (civilization_string == "viking") {
		ret->civilization = CivilizationType::VIKING;
		p->player_type = CivilizationType::VIKING;
		
	}
	else if (civilization_string == "greek") {
		ret->civilization = CivilizationType::GREEK;
		p->player_type = CivilizationType::GREEK;
	}
	else {
		ret->civilization = CivilizationType::VIKING;
		p->player_type = CivilizationType::VIKING;
	}

	return ret;
}

Entity* EntityManager::CreateUnitEntity(UnitType type, iPoint pos, CivilizationType civilization)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case UnitType::ASSASSIN:
		ret = new CombatUnit(UnitType::ASSASSIN, pos);
		break;
	case UnitType::MONK:
		ret = new Unit(UnitType::MONK, pos);
		//ret->texture = animationManager.character_tmx_data.texture;
		break;
	case UnitType::PIKEMAN:
		ret = new CombatUnit(UnitType::PIKEMAN, pos);
		break;
	case UnitType::JOTNAR:
		ret = new CombatUnit(UnitType::JOTNAR, pos);
		break;
	case UnitType::DRAUGAR:
		ret = new CombatUnit(UnitType::DRAUGAR, pos);
		break;
	case UnitType::CYCLOP:
		ret = new CombatUnit(UnitType::CYCLOP, pos);
		break;
	case UnitType::MINOTAUR:
		ret = new CombatUnit(UnitType::MINOTAUR, pos);
		break;
	}
	ret->civilization = civilization;
	ret->type = EntityType::UNIT;
	ret->texture = animationManager.charData[type].texture;

	entities[EntityType::UNIT].push_back(ret);

	//DELETE: THIS
	entities[EntityType::UNIT].sort(entity_Sort());

	return ret;
}

void EntityManager::DrawEverything() 
{

	float dt = App->GetDT();

	for (unsigned i = (int)EntityType::UNIT; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			orderedSprites.insert({(*it)->position.y, (*it)});
		}
	}

	auto range = orderedSprites.equal_range(0);
	for (auto i = orderedSprites.begin(); i != orderedSprites.end(); i = range.second)
	{
		// Get the range of the current key
		range = orderedSprites.equal_range(i->first);

		// Now print out that whole range
		for (auto d = range.first; d != range.second; ++d)
			(*d).second->Draw(dt);
	}

	orderedSprites.clear();

}

Entity* EntityManager::CreateBuildingEntity(iPoint pos, BuildingType type, BuildingInfo info, CivilizationType civilization)
{
	Entity* ret = nullptr;
	switch (type)
	{
	case FORTRESS:
		ret = new Building(BuildingType::FORTRESS, pos, info);
		break;
	case MONASTERY:
		ret = new Building(BuildingType::MONASTERY, pos, info);
		break;
	case TEMPLE:
		ret = new Building(BuildingType::TEMPLE, pos, info);
		break;
	case ENCAMPMENT:
		ret = new Building(BuildingType::ENCAMPMENT, pos, info);
		break;
	}


	iPoint iso = pos;
	iso += App->map->GetTilesHalfSize();
	iso = App->map->WorldToMap(iso.x, iso.y);

	for (int y = iso.y; y > iso.y - info.tileLenght; y--)
	{
		for (int x = iso.x; x < iso.x + info.tileLenght; x++)
		{

			if (IN_RANGE(x, 0, App->map->data.width - 1) == 1 && IN_RANGE(y, 0, App->map->data.height - 1) == 1)
			{
				App->pathfinding->ChangeMapValue({ x, y }, 0);
			}
		}
	}

	ret->civilization = civilization;
	ret->type = EntityType::BUILDING;
	ret->texture = entitySpriteSheets[SpriteSheetType::BUILDINGS];

	//TODO load spritesheet when needed only? first call of constructor of entity?
	entities[EntityType::BUILDING].push_back(ret);
	//TODO: sort elements only inside the screen (QuadTree)
	entities[EntityType::BUILDING].sort(entity_Sort());
	//std::sort(entities[EntityType::BUILDING].begin(), entities[EntityType::BUILDING].end(), entity_Sort());

	return ret;
}

void EntityManager::EnterBuildMode()
{
	crPreview.active = !crPreview.active;
	UpdateBuildPreview(buildingTestIndex);
}
void EntityManager::SetBuildIndex(int index)
{
	if (index < MAX_BUILDING_TYPES) {
		buildingTestIndex = index;
	}

	UpdateBuildPreview(buildingTestIndex);
}

//Called when deleting a new Entity
bool EntityManager::DeleteEntity(Entity* e)
{
	if (e != nullptr) 
	{
		entities[e->type].remove(e);
		delete e;
		return true;
	}
	return false;
}

void EntityManager::UpdateBuildPreview(int index) 
{
	BuildingInfo data = buildingsData[index];
	crPreview.height = data.tileLenght;
	crPreview.width = data.tileLenght;
}

void EntityManager::LoadBuildingsData(pugi::xml_node& node)
{
	if (node != NULL)
	{
		pugi::xml_node obj;

		for (obj = node.child("object"); obj; obj = obj.next_sibling("object"))
		{

			BuildingInfo info;
			bool push = true;
			info.spriteRect = { obj.attribute("x").as_int(), obj.attribute("y").as_int(), obj.attribute("width").as_int(), obj.attribute("height").as_int() };

			pugi::xml_node data = obj.child("properties");
			if (data != NULL)
			{
				pugi::xml_node prop;
				for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
				{
					//OPT: Not the best way but meh

					std::string name = prop.attribute("name").as_string();
					if (name == "civilization")
					{
						CivilizationType type = (CivilizationType)prop.attribute("value").as_int();
						info.civilization = type;
						if (IN_RANGE(type, VIKING, GREEK) == 0)
						{
							push = false;
						}
					}
					else if (push == false && name == "consType")
					{
						if (prop.attribute("value").as_int() == 0)
						{
							destructedSpriteRect = info.spriteRect;
						}
						else
						{
							constructorSpriteRect = info.spriteRect;
						}
						break;
					}
					else if (name == "tileSquareLenght")
					{
						info.tileLenght = prop.attribute("value").as_int();
					}
					else if (name == "type")
					{
						info.buildingType = (BuildingType)prop.attribute("value").as_int();
					}
				}
			}
			//TODO: Find a wat to mesure this with the tileLenght
			info.blitSize = { info.spriteRect.w, info.spriteRect.h };

			if (push)
				buildingsData.push_back(info);
		}
	}


}

iPoint EntityManager::CalculateBuildingSize(int bw, int w, int h) 
{
	return {bw , (bw * h) / w};
}

Player* EntityManager::getPlayer() const
{
	return (Player*)App->entityManager->entities[EntityType::PLAYER].begin()._Ptr->_Myval;
}

bool EntityManager::IsPointInsideQuad(SDL_Rect rect, int x, int y)
{

	if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y + rect.h && y <= rect.y)
		return true;

	return false;
}

void EntityManager::FxUnits(int channel, int fx, int posx, int posy) 
{
	if (Mix_Playing(channel) == 0) {
		Mix_HaltChannel(channel);

		iPoint distance = { posx - (-App->render->camera.x + App->render->camera.w / 2), posy - (-App->render->camera.y + App->render->camera.h / 2) };

		int distance_normalized = (distance.x * distance.x + distance.y * distance.y);
		distance_normalized = distance_normalized / 500;
		volume = (distance_normalized * 255) / App->render->camera.w;

		float angle = 90;
		if (App->render->camera.y == 0) {
			angle = atan(-App->render->camera.x);
		}
		else {
			angle = atan((-App->render->camera.x) / (App->render->camera.y));
		}
		angle = angle * 57 + 360;


		if (volume < 0) { volume = 0; }
		if (volume > 255) { volume = 255; }

		Mix_SetPosition(channel, angle, volume);
		App->audio->PlayFx(channel, fx, 0);
	}

	/*if (Mix_Playing(channel) == 0) {

		Mix_HaltChannel(channel);


		iPoint distance = { ((posx - App->render->camera.x * App->render->camera.x) + (posy - App->render->camera.y * App->render->camera.y)) };
		distance = distance;
		int volume = (distance * 2000) / App->render->camera.w;
		if (volume < 0) {
			volume = 0;
		}
		if (volume > 200) {
			volume = 200;
		}

		float angle = 90;
		if (App->render->camera.y == 0) {
			angle = atan(-App->render->camera.x);
		}
		else {
			angle = atan((-App->render->camera.x) / (App->render->camera.y));
		}
		angle = angle * 57 + 360;

		Mix_SetPosition(channel, angle, volume);
		App->audio->PlayFx(channel, fx, 0);
	}*/
}

