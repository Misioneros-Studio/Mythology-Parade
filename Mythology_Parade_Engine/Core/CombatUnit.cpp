#include "CombatUnit.h"
#include "p2Log.h"

CombatUnit::CombatUnit(UnitType type, iPoint pos): Unit(type), range(0),damage(0)
{
	//TODO 10: Change textures
	unitType = type;
	position = pos;
	state = AnimationType::IDLE;
	switch (unitType)
	{
	case UnitType::ASSASSIN:
		//Change texture
		LevelSystem::Init(3500, 6500, 9500);
		CombatUnit::Init(100, 15, 1, 4);
		break;
	case UnitType::PIKEMAN:
		//Change Texture
		LevelSystem::Init(3000, 6000, 9500);
		CombatUnit::Init(110, 25, 1, 2);
		break;
	}
}

CombatUnit::~CombatUnit()
{
}

void CombatUnit::Action(Entity* entity)
{
	switch (unitType)
	{
	case UnitType::ASSASSIN:
	LOG("I'm a assassin unit!");
		break;
	case UnitType::PIKEMAN:
	LOG("I'm a pikeman unit!");
		break;
	
	}
	//Attack enemy
	Unit* target = (Unit*)entity;
	target->RecieveDamage(GetDamageValue());
}



void CombatUnit::Init(int maxHealth, int damage, int range, int speed)
{
	Unit::Init(maxHealth);
	this->damage = damage;
	this->range = range;
	SetMoveSpeed(speed);

	//TODO: Convert to a get funtions
	targetPosition.ResetAsPosition();
	ChangeState(targetPosition, state);

}

bool CombatUnit::Update(float dt) 
{
	//This needs to be changed once pathfinding is implemented
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		iPoint pos = App->map->GetMousePositionOnMap();


		if (pos != App->map->WorldToMap(position.x, position.y)) 
		{
			targetPosition = pos;
			ChangeState(targetPosition, AnimationType::WALK);
		}
	}

	if (targetPosition != iPoint(-1,-1))
		MoveToTarget();


	int num_current_anim = currentAnim.GetSprite();
	blitRect = { (int)(currentAnim.sprites[num_current_anim].rect.w / 1.5f), (int)(currentAnim.sprites[num_current_anim].rect.h / 1.5f) };

	App->render->Blit(texture, position.x - blitRect.x / 2, position.y - blitRect.y, blitRect, &currentAnim.sprites[num_current_anim].rect, 1.f, flipState);
	//App->render->DrawQuad({position.x, position.y, 5, 5}, 0, 255, 0);

	return true;
}

int CombatUnit::GetDamageValue()
{
	return damage;
}

int CombatUnit::GetRangeValue()
{
	return range;
}
