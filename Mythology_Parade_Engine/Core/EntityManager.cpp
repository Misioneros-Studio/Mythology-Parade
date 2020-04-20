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
}

//Destructor
EntityManager::~EntityManager()
{}

//Called before render is available
bool EntityManager::Awake(pugi::xml_node& a)
{

	//Load buildings info
	pugi::xml_document buildings;
	buildings.load_file(a.child("buildings").attribute("file").as_string());
	LoadBuildingsData(buildings.child("map").child("objectgroup"));
	construction_bar_back = { 1300,512,106,18 };
	construction_bar_front = { 1303,500,100,12 };

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

	for (int i = 0; i < buildingsData.size(); i++)
	{
		BuildingInfo* info = &buildingsData[i];
		int blitWidth = info->tileLenght * App->map->data.tile_width;
		info->blitSize = CalculateBuildingSize(blitWidth, info->spriteRect.w, info->spriteRect.h);

	}


	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			it._Ptr->_Myval->Start();
		}
	}

	return true;
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
			it._Ptr->_Myval->Update(dt);
		}
	}
  
	//TODO: Move this logic to the player
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		crPreview.active = !crPreview.active;
		UpdateBuildPreview(buildingTestIndex);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) 
	{
		if (buildingTestIndex < MAX_BUILDING_TYPES - 1) 
		{
			buildingTestIndex++;
			LOG("Building Index: %i", buildingTestIndex);
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
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && crPreview.active && crPreview.canBuild)
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		iPoint spawnPos = App->map->MapToWorld(mouse.x, mouse.y);
		spawnPos.y += App->map->data.tile_height / 2;
		switch (buildingTestIndex) {
		case 0:
		case 4:
			CreateBuildingEntity(spawnPos, BuildingType::FORTRESS, buildingsData[buildingTestIndex]);
			break;
		case 1:
		case 5:
			CreateBuildingEntity(spawnPos, BuildingType::MONASTERY , buildingsData[buildingTestIndex]);
			break;
		case 2:
		case 6:
			CreateBuildingEntity(spawnPos, BuildingType::TEMPLE, buildingsData[buildingTestIndex]);
			break;
		case 3:
		case 7:
			CreateBuildingEntity(spawnPos, BuildingType::ENCAMPMENT, buildingsData[buildingTestIndex]);
			break;
		}
		
		for (int y = mouse.y; y > mouse.y - crPreview.height; y--)
		{
			for (int x = mouse.x; x < mouse.x + crPreview.width; x++)
			{

				if (IN_RANGE(x, 0, App->map->data.width - 1) == 1 && IN_RANGE(y, 0, App->map->data.height - 1) == 1)
				{
					App->pathfinding->ChangeMapValue({x, y}, 0);
				}
			}
		}
	}

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
	for (int i = 0; i < entitySpriteSheets.size(); i++)
	{
		if(entitySpriteSheets[(SpriteSheetType)i])
			App->tex->UnLoad(entitySpriteSheets[(SpriteSheetType)i]);
	}
	entities.clear();

	for (int i = 0; i < animations.size(); i++)
	{
		for (int k = 0; k < animations[(UnitType)i].size(); k++)
		{
			for (int j = 0; j < animations[(UnitType)i][(AnimationType)k].size(); j++)
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
//bool EntityManager::Load(pugi::xml_node& n)
//{
//	pugi::xml_node n2 = n;
//	for (unsigned int i = 0; i < entities.count(); i++)
//	{
//		n2 = n.child(entities.At(i)->data->name.GetString());
//		while (n2.attribute("id").as_int() != i) {
//			n2 = n2.next_sibling(entities.At(i)->data->name.GetString());
//			assert(n2.attribute("id").as_int() != 0, "Load could not function properly");
//		};
//		entities.At(i)->data->Load(n2);
//	}
//	return true;
//}
//
////Called when saving the game
//bool EntityManager::Save(pugi::xml_node& s) const
//{
//	pugi::xml_node s2 = s;
//	for (unsigned int i = 0; i < entities.count(); i++)
//	{
//		s2 = s.append_child(entities.At(i)->data->name.GetString());
//		entities.At(i)->data->Save(s2);
//		s2.append_attribute("id") = i;
//	}
//	return true;
//}

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

Entity* EntityManager::CreatePlayerEntity()
{
	Entity* ret = nullptr;

	ret = new Player();

	ret->type = EntityType::PLAYER;
	entities[EntityType::PLAYER].push_back(ret);

	return ret;
}

Entity* EntityManager::CreateUnitEntity(UnitType type, iPoint pos)
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
	}
	ret->type = EntityType::UNIT;
	ret->texture = animationManager.charData[type].texture;

	entities[EntityType::UNIT].push_back(ret);

	//DELETE: THIS
	entities[EntityType::UNIT].sort(entity_Sort());

	return ret;
}

Entity* EntityManager::CreateBuildingEntity(iPoint pos, BuildingType type, BuildingInfo info)
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

	ret->type = EntityType::BUILDING;
	ret->texture = entitySpriteSheets[SpriteSheetType::BUILDINGS];

	//TODO load spritesheet when needed only? first call of constructor of entity?
	entities[EntityType::BUILDING].push_back(ret);
	//TODO: sort elements only inside the screen (QuadTree)
	entities[EntityType::BUILDING].sort(entity_Sort());
	//std::sort(entities[EntityType::BUILDING].begin(), entities[EntityType::BUILDING].end(), entity_Sort());

	return ret;
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

Player* EntityManager::getPlayer() 
{
	return (Player*)App->entityManager->entities[EntityType::PLAYER].begin()._Ptr->_Myval;
}