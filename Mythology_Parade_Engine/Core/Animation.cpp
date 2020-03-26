#include "Animation.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Render.h"

Animation::Animation()
{
	name.append("animation");
}

Animation::~Animation()
{
}

bool Animation::Awake(pugi::xml_node& conf)
{
	bool ret = true;
	return ret;
}

void Animation::Draw()
{
	SDL_Rect rect = {0,0,character_tmx_data.tile_width,character_tmx_data.tile_height};
	App->render->Blit(character_tmx_data.texture, 0, 0, &rect);
}

bool Animation::Update(float dt)
{
	Draw();
	return true;
}

bool Animation::CleanUp()
{
	return true;
}

bool Animation::Load(const char* path)
{
	bool ret = true;
	pugi::xml_document	character_file;
	pugi::xml_parse_result result = character_file.load_file(path);
	int rect_width = 0;
	int rect_height = 0;
	int iterator = 1;

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", path, result.description());
		ret = false;
	}

	//Load info
	if (ret == true) {

		pugi::xml_node	character_node = character_file.child("map");

		LoadCharacterTMX(character_node);

		pugi::xml_node group = character_node.child("tileset").child("tile");
		for (group; group; group.next_sibling("tile"))
		{
			iterator = 1;
			rect_width = 0;
			pugi::xml_node frame = group.child("animation").child("frame");
			for (frame; frame && ret; frame = frame.next_sibling("frame"))
			{
				if (strcmp(group.child("properties").child("property").attribute("value").as_string(), "ATCK_DIAG_DOWN") == 0)
				{
					LOG("ATCK_DIAG_DOWN");
					ATCK_DIAG_DOWN = LoadAnimation(group);
				}
				else if(strcmp(group.child("properties").child("property").attribute("value").as_string(), "ATCK_DIAG_UP") == 0)
				{
					LOG("ATCK_DIAG_UP");
				}
				else if (strcmp(group.child("properties").child("property").attribute("value").as_string(), "ATCK_LATERAL") == 0)
				{
					LOG("ATCK_LATERAL");
				}
				else if (strcmp(group.child("properties").child("property").attribute("value").as_string(), "ATCK_UP") == 0)
				{
					LOG("ATCK_UP");
				}
				else if (strcmp(group.child("properties").child("property").attribute("value").as_string(), "ATCK_DOWN") == 0)
				{
					LOG("ATCK_DOWN");
				}
				rect_width += character_tmx_data.tile_width * iterator;
				LOG("ID: %u", frame.attribute("tileid").as_int());
				LOG("Width: %u", rect_width);
				LOG("Height: %u", rect_height);
				rect_width = 0;
				iterator++;
			}
			rect_height += character_tmx_data.tile_height * (frame.attribute("tileid").as_int() + 1);
			group = group.next_sibling("tile");
		}
	}

	return ret;
}

bool Animation::LoadCharacterTMX(pugi::xml_node& character_node)
{
	bool ret = true;
	if (character_node == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		character_tmx_data.width = character_node.attribute("width").as_int();
		character_tmx_data.height = character_node.attribute("height").as_int();
		character_tmx_data.tile_width = character_node.attribute("tilewidth").as_int();
		character_tmx_data.tile_height = character_node.attribute("tileheight").as_int();

		std::string image_source("assets/units/");
		image_source += character_node.child("tileset").child("image").attribute("source").as_string();
		character_tmx_data.texture = App->tex->Load(image_source.c_str());

		if (character_tmx_data.texture) {

			LOG("Player Texture succesfull loaded");
		}
		else {
			LOG("Error loading player texture");
			ret = false;
		}
	}
	return ret;
}

Animation_char* Animation::LoadAnimation(pugi::xml_node& obj_group)
{
	Animation_char* anim = new Animation_char();
	anim->name = obj_group.attribute("name").as_string();

	if (strcmp(anim->name.c_str(), "DISARMED_IDLE") == 0) { ATCK_UP = anim; ATCK_UP->loop = false; }
	else if (strcmp(anim->name.c_str(), "DISARMED_JUMP") == 0) { ATCK_RIGHT = anim; ATCK_RIGHT->loop = false; }
	else if (strcmp(anim->name.c_str(), "DISARMED_DJUMP") == 0) { ATCK_LEFT = anim; ATCK_LEFT->loop = false; }
	else if (strcmp(anim->name.c_str(), "DISARMED_DJUMP") == 0) { ATCK_UP_LEFT = anim; ATCK_UP_LEFT->loop = false; }
	else if (strcmp(anim->name.c_str(), "DISARMED_DJUMP") == 0) { ATCK_UP_RIGHT = anim; ATCK_UP_RIGHT->loop = false; }
	else if (strcmp(anim->name.c_str(), "DISARMED_DJUMP") == 0) { ATCK_DOWN_LEFT = anim; ATCK_DOWN_LEFT->loop = false; }
	else if (strcmp(anim->name.c_str(), "DISARMED_DJUMP") == 0) { ATCK_DOWN_RIGHT = anim; ATCK_DOWN_RIGHT->loop = false; }

	anim->num_sprites = obj_group.child("properties").child("property").last_attribute().as_int();

	anim->sprites = new Sprite[anim->num_sprites];
	int i = 0;
	pugi::xml_node AABB_object = obj_group.next_sibling("objectgroup").child("object");

	for (pugi::xml_node object = obj_group.child("object"); object; object = object.next_sibling("object"))
	{
		anim->sprites[i].rect.x = object.attribute("x").as_int();
		anim->sprites[i].rect.y = object.attribute("y").as_int();
		anim->sprites[i].rect.w = object.attribute("width").as_int();
		anim->sprites[i].rect.h = object.attribute("height").as_int();
		anim->sprites[i].frames = object.child("properties").child("property").attribute("value").as_int();

		//No AABB
		i++;
	}
	return anim;
}

