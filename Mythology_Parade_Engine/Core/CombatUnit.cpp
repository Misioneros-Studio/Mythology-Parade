#include "CombatUnit.h"
#include "j1ParticleManager.h"
#include "p2Log.h"
#include "j1TutorialScene.h"
#include "Player.h"
#include "HUD.h"


CombatUnit::CombatUnit(UnitType type, iPoint pos) : Unit(type, pos), range(0), damage(0)
{
	//TODO 10: Change textures
	speed = 0;
	unitType = type;
	position = {(float)pos.x, (float)pos.y};
	canLevel = true;
	enemyTarget = nullptr;
	switch (unitType)
	{
	case UnitType::ASSASSIN:
		time_production = 5;
		name = "assassin";
		time_research = 10;
		researched = true;
		//Change texture
		LevelSystem::Init(350, 650, 950);
		CombatUnit::Init(100, 15, 1, 80);
		collisionRect = { 0, 0, 30, -55 };
		sizeMultiplier = 2;
		break;
	case UnitType::PIKEMAN:
		time_production = 90;
		time_research = 70;
		researched = false;
		name = "pikeman";
		//Change Texture
		LevelSystem::Init(3000, 6000, 9500);
		CombatUnit::Init(110, 25, 1, 40);
		collisionRect = { 0, 0, 30, -55 };
		break;
	case UnitType::EXPLORER:
		name = "explorer";
		break;
	case UnitType::PRIEST:
		name = "priest";
		sizeMultiplier = 2;
		break;
	case UnitType::FOOTMAN:
		name = "footman";
		break;
	}

	realDamage = damage;

	finish_atack = false;
	combat_unit = true;
	show_bar_for_damage = false;

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

void CombatUnit::LevelUp()
{
	LevelSystem::LevelUp();
	switch (unitType)
	{
	case UnitType::PIKEMAN:
		break;
	case UnitType::ASSASSIN:
		switch (GetLevel())
		{
		case 1:
			IncreaseDamage(5);
			break;
		case 2:
			IncreaseHealth(10);
			break;
		case 3:
			IncreaseSpeed(1);
		}
		break;
	case UnitType::EXPLORER:
		break;
	case UnitType::PRIEST:
		break;
	case UnitType::FOOTMAN:
		break;

	}
	App->particleManager->CreateParticle({ (int)position.x-20,(int)position.y-60 }, { 0,-1 }, 10, ParticleAnimation::Level_Up);
	App->entityManager->FxUnits(4, App->audio->UpgradeUnit, position.x, position.y);
	if (App->scene->isInTutorial == true) {
		App->tutorialscene->level_up = true;
	}
}

void CombatUnit::SetDamage(int d)
{
	damage = realDamage + d;
}



void CombatUnit::Init(int maxHealth, int damage, int range, int speed)
{
	Unit::Init(maxHealth);
	this->damage = damage;
	this->range = range;
	this->speed = speed;
	SetMoveSpeed(speed);

	//TODO: Convert to a get funtions
	targetPosition.ResetAsPosition();
	ChangeState(targetPosition, state);
}

bool CombatUnit::Update(float dt)
{
	Unit::Update(dt);
	if (!IsDeath())
	{

		if (enemyTarget != nullptr)
		{
			if (enemyTarget->IsDeath())
			{
				this->nearbyDetectedList.remove(enemyTarget);
				enemyTarget = nullptr;
				this->ChangeState(this->targetPosition, AnimationType::IDLE);

			}
		}

		DetectNearbyEnemies();

		if (enemyTarget && position.DistanceManhattan(enemyTarget->position) < 90)
		{
			ChangeState(App->map->WorldToMap(enemyTarget->position.x, enemyTarget->position.y), AnimationType::ATTACK);
			//position = App->map->MapToWorld((float)targetPosition.x, (float)targetPosition.y);
			//position += App->map->GetTilesHalfSizeFloat();

			//targetPosition.ResetAsPosition();
			entPath.clear();
		}

		if (state == AnimationType::ATTACK && enemyTarget != nullptr)
		{
			if (enemyTarget->civilization == civilization)
			{
				enemyTarget = nullptr;
				ChangeState(targetPosition, AnimationType::IDLE);
			}
			else {
				if (currentAnim.current_sprite == currentAnim.num_sprites - 8)
				{
					if (finish_atack == false) {
						finish_atack = true;
						App->entityManager->FxUnits(4, App->audio->hit_2, position.x, position.y);

						if (enemyTarget->type == EntityType::BUILDING)
						{
							if (RecieveDamage(static_cast<Building*>(enemyTarget)->GetDamage()))
							{
								if (enemyTarget->civilization != App->entityManager->getPlayer()->civilization)
								{
									App->entityManager->getPlayer()->listEntities.remove(this);
									App->scene->hud->HUDUpdateSelection(App->entityManager->getPlayer()->listEntities, nullptr);
								}
								Kill(App->map->WorldToMap(position.x, position.y));

								CombatUnit* unit = nullptr;
								for (std::list<Entity*>::iterator it = App->entityManager->entities[static_cast<EntityType>(1)].begin(); it != App->entityManager->entities[static_cast<EntityType>(1)].end(); ++it)
								{
									Unit* unit = static_cast<Unit*>((*it));
									if (unit->unitType == UnitType::ASSASSIN)
									{
										CombatUnit* combat_unit = static_cast<CombatUnit*>(*it);

										if (combat_unit->enemyTarget == this) {

											combat_unit->enemyTarget = nullptr;
											combat_unit->ChangeState({ -1,-1 }, AnimationType::IDLE);
										}
									}
								}
							}
						}
						if (enemyTarget->GetHealth() < 600 && enemyTarget->texture != nullptr) {
							if (enemyTarget != nullptr && enemyTarget->GetState() != AnimationType::DIE && enemyTarget->RecieveDamage(this->GetDamageValue()))
							{
								this->GainExperience(Action::killEnemy, App->scene->isInTutorial);
								if (enemyTarget->civilization == App->entityManager->getPlayer()->civilization)
								{
									App->entityManager->getPlayer()->listEntities.remove(enemyTarget);
									App->scene->hud->HUDUpdateSelection(App->entityManager->getPlayer()->listEntities, nullptr);
								}

								if (enemyTarget->type == EntityType::BUILDING) {
									switch (static_cast<Building*>(enemyTarget)->GetBuildingType())
									{
									case MONASTERY:
										App->entityManager->getPlayer()->num_monastery--;
										break;
									case TEMPLE:
										App->entityManager->getPlayer()->num_temple--;
										break;
									case ENCAMPMENT:
										App->entityManager->getPlayer()->num_encampment--;
										break;
									default:
										break;
									}
								}
								enemyTarget->Kill(App->map->WorldToMap(position.x, position.y));
								this->ChangeState(this->targetPosition, AnimationType::IDLE);

								//for (std::list<Entity*>::iterator it = App->entityManager->entities[static_cast<EntityType>(1)].begin(); it != App->entityManager->entities[static_cast<EntityType>(1)].end(); ++it)
								for (auto& it : App->entityManager->entities[static_cast<EntityType>(1)])
								{
									Unit* unit = static_cast<Unit*>(it);
									if (unit->unitType == UnitType::ASSASSIN)
									{
										CombatUnit* combat_unit = static_cast<CombatUnit*>(it);
										if (combat_unit != nullptr && combat_unit->enemyTarget == enemyTarget && combat_unit != this) {

											combat_unit->nearbyDetectedList.remove(enemyTarget);
											combat_unit->enemyTarget = nullptr;
											combat_unit->ChangeState(combat_unit->targetPosition, AnimationType::IDLE);
										}
									}
								}
								this->nearbyDetectedList.remove(enemyTarget);
								enemyTarget = nullptr;
							}
						}
					}
				}
				else {
					finish_atack = false;
				}
			}

		}

		if (isSelected()) {
			switch (GetLevel())
			{
			case (1):
				App->render->Blit(App->entityManager->level_tex, (position.x - 6), (position.y - 63), &App->entityManager->level_rect, 1, 0, 0, 0, 1);
				break;
			case (2):
				App->render->Blit(App->entityManager->level_tex, (position.x - 2), (position.y - 63), &App->entityManager->level_rect, 1, 0, 0, 0, 1);
				App->render->Blit(App->entityManager->level_tex, (position.x - 10), (position.y - 63), &App->entityManager->level_rect, 1, 0, 0, 0, 1);
				break;
			case (3):
				App->render->Blit(App->entityManager->level_tex, (position.x - 6), (position.y - 63), &App->entityManager->level_rect, 1, 0, 0, 0, 1);
				App->render->Blit(App->entityManager->level_tex, (position.x - 14), (position.y - 63), &App->entityManager->level_rect, 1, 0, 0, 0, 1);
				App->render->Blit(App->entityManager->level_tex, (position.x + 2), (position.y - 63), &App->entityManager->level_rect, 1, 0, 0, 0, 1);
				break;
			}
		}
	}

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


int CombatUnit::GetSpeedValue()
{
	return speed;
}

void CombatUnit::IncreaseHealth(int value)
{
	HealthSystem::IncreaseHealth(value);
}

void CombatUnit::IncreaseSpeed(int value)
{
	speed += value;
}

void CombatUnit::IncreaseDamage(int value)
{
	damage += value;
}


void CombatUnit::SetDefaultHealth()
{
	HealthSystem::SetDefaultHealth();
}
void CombatUnit::SetLevel(int i)
{
	for (int j = 0; j < i; j++)
	{
		LevelUp();
	}
}

void CombatUnit::SetHealth(int value)
{
	HealthSystem::SetHealth(value);
}

void CombatUnit::DetectNearbyEnemies()
{

	for (int i = 1; i < 3; ++i)
	{
		for (std::list<Entity*>::iterator it = App->entityManager->entities[static_cast<EntityType>(i)].begin(); it != App->entityManager->entities[static_cast<EntityType>(i)].end(); it++)
		{
			Entity* entity = it._Ptr->_Myval;
			if (!entity->IsDeath() && entity->civilization != civilization)
			{
				if (entity->position.DistanceTo(position) < 400) // Posar valor com a range variable
				{
					if (std::find(nearbyDetectedList.begin(), nearbyDetectedList.end(), entity) != nearbyDetectedList.end())
					{
						continue; 
					}
					else 
					{
						nearbyDetectedList.push_back(entity);

					}


					//enemyTarget = entity;
					////Request path 
					//if (GetTilePosition() != enemyTarget->GetTilePosition())
					//	App->pathfinding->RequestPath(this->GetTilePosition(), enemyTarget->GetTilePosition(), this);
					////Guardar enemy map position
					//oldEnemyPosition = enemyTarget->GetTilePosition();

					//LOG("Enemy detected: %i", entity->type);
					//return;
				}
			}
		}
	}
	if (!nearbyDetectedList.empty()) {
		Entity* closestEntity = nearbyDetectedList.front();
		for each (Entity * var in nearbyDetectedList)
		{
			if (position.DistanceManhattan(var->position) < position.DistanceManhattan(closestEntity->position)) {
				closestEntity = var;
			}
		}

		if (enemyTarget != closestEntity) {
			enemyTarget = closestEntity;
			//Request path 
			if (GetTilePosition() != enemyTarget->GetTilePosition() && !enemyTarget->IsDeath())
				App->pathfinding->RequestPath(this->GetTilePosition(), enemyTarget->GetTilePosition(), this);
			//Guardar enemy map position
			oldEnemyPosition = enemyTarget->GetTilePosition();
		}
	}


	
	//LOG("Detected units: %i", nearbyDetectedList.size());
	
	//else
	//{
	//	//Sha mogut de tile?
	//	//if (enemyTarget->GetTilePosition() != oldEnemyPosition) 
	//	//{
	//	//	//Updatear la tile on esta el enemic
	//	//	oldEnemyPosition = enemyTarget->GetTilePosition();
	//	//	//Request new path
	//	//	if (GetTilePosition() != enemyTarget->GetTilePosition() && position.DistanceManhattan(enemyTarget->position) >= 300)
	//	//		App->pathfinding->RequestPath(GetTilePosition(), enemyTarget->GetTilePosition(), App->entityManager->getPlayer()->GetEntitiesSelected());
	//	//}
	//}
}
