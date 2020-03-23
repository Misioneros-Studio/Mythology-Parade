#include "Unit.h"
#include "p2Log.h"
#include "j1Textures.h"

Unit::Unit()
{
}

Unit::~Unit()
{
}

bool Unit::Start()
{
	bool ret = true;

	std::string image_source("animations/");
	//image_source += character_node.child("imagelayer").child("image").attribute("source").as_string();
	//texture = App->tex->Load(image_source.c_str);
	
	return ret;
}

bool Unit::Update(float dt)
{
	bool ret = true;
	Draw(dt);
	return ret;
}

bool Unit::Draw(float dt)
{
	bool ret = true;
	std::string image_source("animations/");
	LOG("%s", image_source.c_str());
//	App->render->Blit(texture, position.x, position.y, &position_rect);
	return ret;
}
