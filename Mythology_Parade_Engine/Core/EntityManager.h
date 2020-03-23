#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include "j1Module.h"
#include <unordered_map>
#include "Entity.h"

enum UnitType;
class Entity;
class EntityManager : public j1Module
{
public:

	EntityManager();

	// Destructor
	~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	////Called when loading the game
	//bool Load(pugi::xml_node&);

	////Called when saving the game
	//bool Save(pugi::xml_node&) const;

	bool DeleteEntity(Entity*);

	Entity* CreateEntity(EntityType, UnitType);

public:

	std::unordered_map<EntityType, std::list<Entity*>> entities;

};
#endif // !_ENTITYMANAGER_H