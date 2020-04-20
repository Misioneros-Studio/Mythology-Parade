#include "HealthSystem.h"
#include "j1App.h"
#include "j1Scene.h"

void HealthSystem::Init()
{
	isDeath = false;
	health = maxHealth;
}

void HealthSystem::RecieveDamage(int value)
{
	if (!App->scene->godMode)
	{
		if (!isDeath)
			health -= value;

		if (health <= 0)
			isDeath = true;
	}
}
int HealthSystem::GetHealth()
{
	return health;
}

void HealthSystem::SetMaxHealth(int value)
{	
	maxHealth = value;
}

void HealthSystem::IncreaseHealth(int value)
{
	health += value;
	maxHealth += value;
}

