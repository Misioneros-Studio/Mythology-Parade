#include "Animation.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Input.h"

Animation::Animation()
{
}

Animation::~Animation()
{
}

std::unordered_map<AnimationType, std::unordered_map<Direction, Animation_char>> Animation::Load(const char* path, UnitType s_type)
{
	bool ret = true;
	pugi::xml_document	character_file;
	pugi::xml_parse_result result = character_file.load_file(path);

	std::unordered_map<AnimationType, std::unordered_map<Direction, Animation_char>> animations;

	int row = 0;
	std::string name;
	int sprite_num = 0;
	AnimationType type;
	Direction dir;

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", path, result.description());
		ret = false;
	}

	//Load info
	if (ret == true) {

		pugi::xml_node	character_node = character_file.child("map");

		LoadCharacterTMX(character_node, &charData[s_type]);


		pugi::xml_node group = character_node.child("tileset").child("tile");
		for (group; group; group.next_sibling("tile"))
		{
			// This loop reads basic .tmx properties for iteration between frames
			pugi::xml_node it = group.child("properties").child("property");
			for (it; it; it.next_sibling("property"))
			{
				if (strcmp(it.attribute("name").as_string(), "sprites") == 0)
				{
					sprite_num = it.attribute("value").as_int();
				}
				if (strcmp(it.attribute("name").as_string(), "type") == 0)
				{
					name = it.attribute("value").as_string();
				}
				if (strcmp(it.attribute("name").as_string(), "row") == 0)
				{
					row = it.attribute("value").as_int();
				}
				if (strcmp(it.attribute("name").as_string(), "dicType") == 0)
				{
					type = (AnimationType)it.attribute("value").as_int();
				}
				if (strcmp(it.attribute("name").as_string(), "dir") == 0)
				{
					dir = (Direction)it.attribute("value").as_int();
				}
				it = it.next_sibling("property");
			}
			animations[type][dir] = LoadAnimation(group, row, sprite_num, name, &charData[s_type]);

			if (type == AnimationType::DIE || type == AnimationType::HIT) 
			{
				animations[type][dir].loop = false;
			}

			group = group.next_sibling("tile");
		}
	}
	//current_anim = animations[(AnimationType)0][(Direction)0];
	return animations;
}

bool Animation::LoadCharacterTMX(pugi::xml_node& character_node, CharacterTMXData* cData)
{
	bool ret = true;
	if (character_node == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		cData->width = character_node.attribute("width").as_int();
		cData->height = character_node.attribute("height").as_int();
		cData->tile_width = character_node.attribute("tilewidth").as_int();
		cData->tile_height = character_node.attribute("tileheight").as_int();

		std::string image_source("assets/units/");
		image_source += character_node.child("tileset").child("image").attribute("source").as_string();
		cData->texture = App->tex->Load(image_source.c_str());

		if (cData->texture) {

			LOG("Player Texture succesfull loaded");
		}
		else {
			LOG("Error loading player texture");
			ret = false;
		}
	}
	return ret;
}

Animation_char Animation::LoadAnimation(pugi::xml_node& obj_group, int row, int sprite_num, std::string name, CharacterTMXData* cData)
{
	Animation_char anim;

	anim.name = name;
	anim.num_sprites = sprite_num;
	anim.sprites.reserve(sprite_num);

	int rect_width = 0;
	int rect_height = 0;
	int rect_x = 0;
	int rect_y = cData->tile_height * row;

	int i = 0;
	pugi::xml_node frame = obj_group.child("animation").child("frame");
	for (frame; frame; frame = frame.next_sibling("frame"))
	{

		Sprite ret;

		ret.rect.x = rect_x;
		ret.rect.y = rect_y;
		ret.rect.w = cData->tile_width;
		ret.rect.h = cData->tile_height;
		ret.frames = frame.attribute("duration").as_int();
		anim.sprites.push_back(ret);

		rect_x += cData->tile_width;
		i++;
	}

	anim.loop = true;

	return anim;
}