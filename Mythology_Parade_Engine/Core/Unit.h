#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"
#include "SDL/include/SDL_rect.h"
#include "HealthSystem.h"

enum State
{
	IDLE,
	MOVE
};

enum UnitType 
{
	ASSASSIN,
	MONK,
	PIKEMAN
};

class Unit : public Entity, public HealthSystem
{
private:

	//Move Speed
	int moveSpeed;

	//Conditions
	bool _isSelected;
	
	//state
	State state;

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

private:
	//Private Functions
	void MoveTo(p2Point<int>);

	void CheckState();

};

#endif // !__UNIT_H__
