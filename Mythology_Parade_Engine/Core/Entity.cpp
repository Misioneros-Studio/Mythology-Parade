#include "Entity.h"
#include"EntityManager.h"

Entity::Entity() : fowRadius(10), canLevel(false),
displayDebug(false), _isSelected(false), shown_minimap(false), flipState(SDL_FLIP_NONE), texture(nullptr),
type(EntityType::PLAYER), civilization(CivilizationType::NONE)
{
	name = "";
	collisionRect = { 0, 0, 0, 0 };
	spriteRect = { 0, 0, 0, 0 };
}

iPoint Entity::GetTilePosition()
{
	return App->map->WorldToMap(position.x, position.y);
}

bool Entity::isSelected()
{
	return _isSelected;
}

void Entity::SetSelected(bool value)
{
	_isSelected = value;
}


AnimationType Entity::GetState()
{
	return AnimationType::IDLE;
}