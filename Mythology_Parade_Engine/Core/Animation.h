#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL.h"
#include "j1App.h"
#include "j1Textures.h"
#include <unordered_map>
//#include "Unit.h"

enum class UnitType;
enum class AnimationType
{
	ATTACK,
	DIE,
	HIT,
	IDLE,
	WALK,
};
enum class Direction
{
	UP,
	LATERAL,
	DOWN,
	DIAGONAL_DOWN,
	DIAGONAL_UP,
};

struct Sprite
{
	SDL_Rect rect;
	SDL_Rect AABB_rect;
	int frames = 0;
	int current_frame = 0;
};

struct Animation_char
{
	bool loop = true;
	std::string name;
	int num_sprites = 0;
	std::vector<Sprite> sprites;
	bool finished = false;
	int current_sprite = 0;

public:
	//We use ResetAnim when changing to another animation in order to put the previous one back to the starting frame
	void ResetAnim() {
		current_sprite = 0;
		finished = false;
		for (int i = 0; i < num_sprites; i++)
		{
			sprites[i].current_frame = 0;
		}
	}

	int GetSprite()
	{
		if (!finished) 
		{
			if (sprites[current_sprite].current_frame != sprites[current_sprite].frames)
			{
				sprites[current_sprite].current_frame = sprites[current_sprite].current_frame + 1;
			}
			else {
				sprites[current_sprite].current_frame = 0;
				current_sprite++;
			}
			if (loop) {
				if (current_sprite == num_sprites) {
					current_sprite = 0;
				}
			}
			else if (current_sprite == num_sprites - 1)
			{
				current_sprite = num_sprites - 1;
				finished = true;
			}
		}
		return current_sprite;
	}

	void Clean()
	{
		sprites.clear();
	}

};




struct CharacterTMXData
{
	SDL_Texture* texture = nullptr;
	uint					width = 0u;
	uint					height = 0u;
	int						tile_width = 0u;
	int						tile_height = 0u;

	void Clean()
	{
		App->tex->UnLoad(texture);
	}

};

class Animation
{
public:
	Animation();
	// Destructor
	virtual ~Animation();

	// Called before quitting
	std::unordered_map<AnimationType, std::unordered_map<Direction, Animation_char>> Load(const char* path, UnitType s_type);
	bool LoadCharacterTMX(pugi::xml_node& character_node, CharacterTMXData* cData);
	virtual Animation_char LoadAnimation(pugi::xml_node& obj_group, int row, int sprite_num, std::string name, CharacterTMXData* cData);

public:
	std::unordered_map<UnitType, CharacterTMXData> charData;
};

#endif
