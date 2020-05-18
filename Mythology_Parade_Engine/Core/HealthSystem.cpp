#include "HealthSystem.h"
#include "j1App.h"
#include "j1Scene.h"

void HealthSystem::Init()
{
	isDeath = false;
	health = maxHealth;
}

bool HealthSystem::RecieveDamage(int value)
{
	if (!App->scene->godMode)
	{
		if (!isDeath)
			health -= value;

		if (health <= 0)
			isDeath = true;
	}
	return isDeath;
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

void HealthSystem::SetHealth(int value)
{
	health = value;
}

