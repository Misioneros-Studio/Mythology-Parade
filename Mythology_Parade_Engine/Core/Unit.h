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
	int time_production;
	int time_research;
	bool researched;

public:
	Unit(UnitType, iPoint);
	virtual ~Unit();

	void Init(int maxHealth);

	bool Start() override;
	bool Update(float dt);

	void SetMoveSpeed(int);

	bool isSelected();

	virtual bool Draw(float dt);
	virtual void Action(Entity*);
	void ChangeState(iPoint, AnimationType);

	void MoveToTarget();
	Direction getMovementDirection(iPoint);
	void SetPath(const std::vector<iPoint>);

protected:
	//Animation
	Direction currentDirection;
	Animation_char currentAnim;

	iPoint targetPosition;
	AnimationType state;
	Unit* enemyTarget;

	std::vector<iPoint> entPath;
	//void SetTarget();
	//void CheckState();
	
};

#endif // !__UNIT_H__
