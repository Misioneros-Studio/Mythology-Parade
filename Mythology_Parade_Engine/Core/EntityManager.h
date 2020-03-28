#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include "j1Module.h"
#include <unordered_map>
#include "Entity.h"
#include"j1Input.h"
#include"j1Map.h"
#include"j1Pathfinding.h"

//Can delete
#include "j1Scene.h"

enum class UnitType;
enum BuildingType;

struct CreationPreview 
{
	bool active = false;
	int width = 4;
	int height = 4;
	bool canBuild = false;

};

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

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	////Called when loading the game
	//bool Load(pugi::xml_node&);

	////Called when saving the game
	//bool Save(pugi::xml_node&) const;

	bool DeleteEntity(Entity*);

	Entity* CreatePlayerEntity();
	Entity* CreateUnitEntity(UnitType);
	Entity* CreateBuildingEntity(BuildingType);

public:

	std::unordered_map<EntityType, std::list<Entity*>> entities;
	CreationPreview crPreview;
	SDL_Texture* debugTex;

	//The way to store the spritesheets (needs to be cleaned and spritesheets need to be unloaded)
	//std::unordered_map<SpriteSheetType, std::list<SDL_Texture*>> entitySpriteSheets;

};
#endif // !_ENTITYMANAGER_H