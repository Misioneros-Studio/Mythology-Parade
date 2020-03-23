#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL.h"
#include "j1App.h"

enum MapTypes_v2
{
	MAPTYPE_UNKNOWN_v2 = 0,
	MAPTYPE_ORTHOGONAL_v2,
	MAPTYPE_ISOMETRIC_v2,
	MAPTYPE_STAGGERED_v2
};

//Object layer structs
struct ObjectLayer_v2
{
	std::string				name;
	std::list<SDL_Rect*>	rects;
};

struct Sprite {

	SDL_Rect rect;
	SDL_Rect AABB_rect;
	int frames = 0;
	int current_frame = 0;

};

struct Animation_char {
	bool loop = true;
	std::string name;

	int num_sprites = 0;
	Sprite* sprites = nullptr;
	bool finished = false;

	int current_sprite = 0;
};

struct CharacterTMXData {
	SDL_Texture* texture = nullptr;
	uint					width = 0u;
	uint					height = 0u;
	int						tile_width = 0u;
	int						tile_height = 0u;
	MapTypes_v2				type;
	std::list<ObjectLayer_v2*>	object_Layers;
};

class Animation :public j1Module {
public:

	Animation();

	// Destructor
	virtual ~Animation();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();
	bool Load(const char* path);
	bool LoadCharacterTMX(pugi::xml_node& character_node);
	virtual Animation_char* LoadAnimation(pugi::xml_node& obj_group);

private:
	CharacterTMXData character_tmx_data;
	Animation_char* ATCK_DOWN;
	Animation_char* ATCK_DOWN_LEFT;
	Animation_char* ATCK_DOWN_RIGHT;
	Animation_char* ATCK_UP;
	Animation_char* ATCK_UP_RIGHT;
	Animation_char* ATCK_UP_LEFT;
	Animation_char* ATCK_LEFT;
	Animation_char* ATCK_RIGHT;
};

#endif
