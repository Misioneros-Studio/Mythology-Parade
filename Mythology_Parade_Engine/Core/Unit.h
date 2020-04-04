#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"
#include "SDL/include/SDL_rect.h"
#include "HealthSystem.h"
#include"EntityManager.h"
#include "Animation.h"

enum class ReligiousType
{
	MONK,
	CLERIC,
	MISSIONARY
};

enum class UnitType 
{
	PIKEMAN,
	ASSASSIN,
	EXPLORER,
	PRIEST,
	FOOTMAN,
	MONK,
	CLERIC,
	MISSIONARY
};

class Unit : public Entity, public HealthSystem
{
private:

	//Move Speed
	int moveSpeed;

	//Conditions
	bool _isSelected;
	
	//Description / Effect
	std::string description;

public: 
	//Unit Type
	UnitType unitType;

public:
	Unit(UnitType);
	virtual ~Unit();

	void Init(int maxHealth);

	bool Start() override;
	bool Update(float dt);

	void SetMoveSpeed(int);

	bool isSelected();

	virtual bool Draw(float dt);
	virtual void Action(Entity*);

protected:
	//Animation
	Direction currentDirection;
	Animation_char currentAnim;

	iPoint targetPosition;
	Direction getMovementDirection(iPoint);
	AnimationType state;
	//void SetTarget();


	//void CheckState();
	void MoveToTarget();

};

#endif // !__UNIT_H__
