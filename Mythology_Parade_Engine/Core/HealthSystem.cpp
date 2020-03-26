#include "HealthSystem.h"



void HealthSystem::Init()
{
	isDeath = false;
	health = maxHealth;
}

void HealthSystem::RecieveDamage(int value)
{
	if (!isDeath)
		health -= value;

	if (health <= 0)
		isDeath = true;
}
int HealthSystem::GetHealth()
{
	return health;
}

void HealthSystem::SetMaxHealth(int value)
{	
	maxHealth = value;
}

