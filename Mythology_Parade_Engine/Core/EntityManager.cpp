#include "EntityManager.h"
#include "CombatUnit.h"
#include "Building.h"
#include "Player.h"

#include "p2Log.h"
EntityManager::EntityManager()
{
	name.append("entity_manager");
	buildingsData.reserve(MAX_BUILDING_TYPES);
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

	std::string path = "assets/buildings/";
	path.append(buildings.child("map").child("imagelayer").child("image").attribute("source").as_string());

	//Not working because renderer is not created yet ;-;
	//tempBuildingTexture = App->tex->Load(path.c_str());

	pugi::xml_node data = buildings.child("map").child("objectgroup");


	if (data != NULL)
	{
		pugi::xml_node obj;

		for (obj = data.child("object"); obj; obj = obj.next_sibling("object"))
		{

			BuildingInfo info;	
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
						info.civilization = (CivilizationType)prop.attribute("value").as_int();
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
			//If civilitzation == NONE means is a generic destructed/being built data type
			buildingsData.push_back(info);
		}
	}


	//INFO: This is a good way to itinerate all the map, to itinerate only items in one key, use only the second for loop
	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			Entity* ent = it._Ptr->_Myval;
			ent->Awake(a.child(ent->name.c_str()));
		}
	}
	return true;
}

// Called before the first frame
bool EntityManager::Start()
{
	tempBuildingTexture = App->tex->Load("assets/buildings/Buildings.png");
	for (int i = 0; i < buildingsData.size(); i++)
	{
		BuildingInfo* info = &buildingsData[i];
		int blitWidth = info->tileLenght * App->map->data.tile_width;
		info->blitSize.x = blitWidth;
		info->blitSize.y = (blitWidth * info->spriteRect.h) / info->spriteRect.w;
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

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		crPreview.active = !crPreview.active;
		BuildingInfo data = buildingsData[1];
		crPreview.height = data.tileLenght;
		crPreview.width = data.tileLenght;
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

		CreateBuildingEntity(App->map->MapToWorld(mouse.x, mouse.y) , BuildingType::FORTRESS, buildingsData[1]);

		
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
	App->tex->UnLoad(tempBuildingTexture);
	entities.clear();
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

	entities[EntityType::PLAYER].push_back(ret);

	return ret;
}

Entity* EntityManager::CreateUnitEntity(UnitType type)
{
	Entity* ret = nullptr;
	
	switch (type)
	{
	case UnitType::ASSASSIN:
		ret = new CombatUnit(UnitType::ASSASSIN);
		break;
	case UnitType::MONK:
		ret = new Unit(UnitType::MONK);
		break;
	case UnitType::PIKEMAN:
		ret = new CombatUnit(UnitType::PIKEMAN);
		break;
	}

	entities[EntityType::UNIT].push_back(ret);

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

	entities[EntityType::BUILDING].push_back(ret);

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

