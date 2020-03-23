#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"
#include "SDL/include/SDL_rect.h"

enum State
{
	IDLE,
	MOVE,
	ATTACK
};

struct HealthSystem
{
	int health;
	int maxHealth;

	bool isDeath;

	//Function to substract health from enemy attack
	void RecieveDamage(int value);

	//Returns current health
	int GetHealth();
};

class Unit : public Entity
{
private:
	
	HealthSystem healthSystem;


	bool isSelected;


	//state
	State state;

public:
	Unit();
	~Unit();

	bool Start() override;
	bool Update(float dt) override;

	void Move(p2Point<int>);

private:
	//Private Functions
	bool Draw(float dt);



};

#endif // !__UNIT_H__
