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
	active = false;
	return ret;
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


	int row = 0;
	std::string name;
	int sprite_num = 0;

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
			it = it.next_sibling("property");
		}
		ChooseAnimation(group, row, sprite_num, name);
		group = group.next_sibling("tile");
	}
}
	current_anim = WALK_DOWN;
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

Animation_char* Animation::LoadAnimation(pugi::xml_node& obj_group, int row, int sprite_num, std::string name)
{
	Animation_char* anim = new Animation_char();

	anim->name = name;
	anim->num_sprites = sprite_num;
	anim->sprites = new Sprite[anim->num_sprites];

	int rect_width = 0;
	int rect_height = 0;
	int rect_x = 0;
	int rect_y = character_tmx_data.tile_height * row;

	int i = 0;
	pugi::xml_node frame = obj_group.child("animation").child("frame");
	for (frame; frame; frame = frame.next_sibling("frame"))
	{

	/*	anim->sprites[i].rect.x = rect_x;
		anim->sprites[i].rect.y = rect_y;
		anim->sprites[i].rect.w = rect_x + character_tmx_data.tile_width;
		anim->sprites[i].rect.h = character_tmx_data.tile_height;*/

		anim->sprites[i].rect.x = rect_x;
		anim->sprites[i].rect.y = rect_y;
		anim->sprites[i].rect.w = character_tmx_data.tile_width;
		anim->sprites[i].rect.h = character_tmx_data.tile_height;
		anim->sprites[i].frames = frame.attribute("duration").as_int();
		anim->loop = true;
		rect_x += character_tmx_data.tile_width;
		i++;
	}
	
	return anim;
}

void Animation::ChooseAnimation(pugi::xml_node& group, int row, int sprite_num, std::string name)
{
	//Attacks
	if (strcmp(name.c_str(), "ATCK_DIAG_DOWN") == 0)	{	ATCK_DIAG_DOWN = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "ATCK_DIAG_UP") == 0) {	ATCK_DIAG_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "ATCK_LATERAL") == 0) {	ATCK_LATERAL = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "ATCK_UP") == 0)		{	ATCK_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "ATCK_DOWN") == 0)	{	ATCK_DOWN = LoadAnimation(group, row, sprite_num, name);}

	//Death
	if (strcmp(name.c_str(), "DIE_DIAG_DOWN") == 0)		{	DIE_DIAG_DOWN = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "DIE_DIAG_UP") == 0)	{	DIE_DIAG_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "DIE_LATERAL") == 0)	{	DIE_LATERAL = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "DIE_UP") == 0)		{	DIE_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "DIE_DOWN") == 0)		{	DIE_DOWN = LoadAnimation(group, row, sprite_num, name);}

	//Hit
	if (strcmp(name.c_str(), "HIT_DIAG_DOWN") == 0)		{	HIT_DIAG_DOWN = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "HIT_DIAG_UP") == 0)	{	HIT_DIAG_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "HIT_LATERAL") == 0)	{	HIT_LATERAL = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "HIT_UP") == 0)		{	HIT_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "HIT_DOWN") == 0)		{	HIT_DOWN = LoadAnimation(group, row, sprite_num, name);}

	//Idle
	if (strcmp(name.c_str(), "IDLE_DIAG_DOWN") == 0)	{	IDLE_DIAG_DOWN = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "IDLE_DIAG_UP") == 0)	{	IDLE_DIAG_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "IDLE_LATERAL") == 0)	{	IDLE_LATERAL = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "IDLE_UP") == 0)		{	IDLE_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "IDLE_DOWN") == 0)	{	IDLE_DOWN = LoadAnimation(group, row, sprite_num, name);}

	//Walk
	if (strcmp(name.c_str(), "WALK_DIAG_DOWN") == 0)	{	WALK_DIAG_DOWN = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "WALK_DIAG_UP") == 0)	{	WALK_DIAG_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "WALK_LATERAL") == 0)	{	WALK_LATERAL = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "WALK_UP") == 0)		{	WALK_UP = LoadAnimation(group, row, sprite_num, name);}
	else if (strcmp(name.c_str(), "WALK_DOWN") == 0)	{	WALK_DOWN = LoadAnimation(group, row, sprite_num, name);}
}

void Animation::Draw()
{
	num_current_anim = current_anim->GetSprite();
	App->render->Blit(character_tmx_data.texture, 0, 0, &current_anim->sprites[num_current_anim].rect);
}
