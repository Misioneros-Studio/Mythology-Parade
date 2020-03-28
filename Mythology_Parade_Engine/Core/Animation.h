#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL.h"
#include "j1App.h"
#include "j1Textures.h"


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
	Sprite* sprites = nullptr;
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
		else if (current_sprite == num_sprites) {
			current_sprite = num_sprites;
			finished = true;
		}
		return current_sprite;
	}

	void Clean() 
	{
		if (sprites)
			delete sprites;
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

class Animation :public j1Module 
{

	Animation();
	// Destructor
	virtual ~Animation();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	bool Update(float dt) override;
	// Called before quitting
	bool CleanUp();
	bool Load(const char* path);
	bool LoadCharacterTMX(pugi::xml_node& character_node);
	void ChooseAnimation(pugi::xml_node& obj_group, int row, int sprite_num, std::string name);
	virtual Animation_char* LoadAnimation(pugi::xml_node& obj_group, int row, int sprite_num, std::string name);
	void Draw();

private:
	CharacterTMXData character_tmx_data;

	Animation_char* ATCK_DIAG_DOWN;
	Animation_char* ATCK_DIAG_UP;
	Animation_char* ATCK_LATERAL;
	Animation_char* ATCK_UP;
	Animation_char* ATCK_DOWN;

	Animation_char* DIE_DIAG_DOWN;
	Animation_char* DIE_DIAG_UP;
	Animation_char* DIE_LATERAL;
	Animation_char* DIE_UP;
	Animation_char* DIE_DOWN;

	Animation_char* HIT_DIAG_DOWN;
	Animation_char* HIT_DIAG_UP;
	Animation_char* HIT_LATERAL;
	Animation_char* HIT_UP;
	Animation_char* HIT_DOWN;

	Animation_char* IDLE_DIAG_DOWN;
	Animation_char* IDLE_DIAG_UP;
	Animation_char* IDLE_LATERAL;
	Animation_char* IDLE_UP;
	Animation_char* IDLE_DOWN;

	Animation_char* WALK_DIAG_DOWN;
	Animation_char* WALK_DIAG_UP;
	Animation_char* WALK_LATERAL;
	Animation_char* WALK_UP;
	Animation_char* WALK_DOWN;

	Animation_char* current_anim = nullptr;
	int num_current_anim;
};

#endif
