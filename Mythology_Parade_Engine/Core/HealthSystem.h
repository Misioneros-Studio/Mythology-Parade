#ifndef __HEALTHSYSTEM_H__
#define __HEALTHSYSTEM_H__

struct HealthSystem
{
private:

	int health;
	int maxHealth;

	bool isDeath;

public:

	//Initilization
	void Init();

	//Function to substract health from enemy attack
	void RecieveDamage(int value);

	//Getters
	int GetHealth();

	//Setters
	void SetMaxHealth(int value);
	void IncreaseHealth(int value);
};

#endif // !__HEALTHSYSTEM_H__
