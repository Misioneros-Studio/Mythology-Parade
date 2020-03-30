#include "EntityManager.h"
#include "CombatUnit.h"
#include "Building.h"
#include "Player.h"

#include "p2Log.h"
EntityManager::EntityManager()
{
	name.append("entity_manager");
}

//Destructor
EntityManager::~EntityManager()
{}

//Called before render is available
bool EntityManager::Awake(pugi::xml_node& a)
{
	//INFO: This is a good way to itinerate all the map, to itinerate only items in one key, use only the second for loop
	for (unsigned i = 0; i < entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = entities[(EntityType)i].begin(); it != entities[(EntityType)i].end(); it++)
		{
			Entity* ent = it._Ptr->_Myval;
			//ent->Awake(a.child(ent->name.GetString()));
		}
	}
	active = false;

	return true;
}

// Called before the first frame
bool EntityManager::Start()
{

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
	return true;
}

bool EntityManager::PostUpdate() 
{

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT) 
	{
		iPoint mouse = App->map->GetMousePositionOnMap();
		bool isOn = App->pathfinding->IsWalkable(mouse);

		if (isOn) 
		{

		}
	}


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

Entity* EntityManager::CreateBuildingEntity(BuildingType type)
{
	Entity* ret = nullptr;
	switch (type)
	{
	case FORTRESS:
		ret = new Building(BuildingType::FORTRESS);
		break;
	case MONASTERY:
		ret = new Building(BuildingType::MONASTERY);
		break;
	case TEMPLE:
		ret = new Building(BuildingType::TEMPLE);
		break;
	case ENCAMPMENT:
		ret = new Building(BuildingType::ENCAMPMENT);
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

