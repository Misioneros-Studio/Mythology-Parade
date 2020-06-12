#include "HealthSystem.h"
#include "j1App.h"
#include "j1Scene.h"

HealthSystem::HealthSystem() : health(0), maxHealth(0), defaultHealth(0), isDeath(false), damaged_now(false)
{

}

void HealthSystem::Init()
{
	isDeath = false;
	health = defaultHealth = maxHealth;
	damaged_now = false;
}

bool HealthSystem::RecieveDamage(int value)
{
	//if (!App->scene->godMode)
	//{
		if (!isDeath)
			health -= value;

		if (health <= 0)
			isDeath = true;
	//}
	damaged_now = true;
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

int HealthSystem::GetMaxHealth()
{
	return maxHealth;
}

void HealthSystem::IncreaseHealth(int value)
{
	health += value;
	maxHealth += value;
}


void HealthSystem::SetDefaultHealth()
{
	maxHealth = defaultHealth;
	health += 20;
}
void HealthSystem::SetHealth(int value)
{
	health = value;
}

void HealthSystem::SetMaxUnitHealth()
{
	health = defaultHealth;
}

void HealthSystem::DivideHealth()
{
	health = health / 2;
}

bool HealthSystem::IsDeath()
{
	return isDeath;
}


