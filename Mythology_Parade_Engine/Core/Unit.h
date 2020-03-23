#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"
#include "SDL/include/SDL_rect.h"


class Unit : public Entity
{
private:

	int health;
	bool isSelected;

public:
	Unit();
	~Unit();

	bool Start() override;
	bool Update(float dt) override;

private:
	//Private Functions
	bool Draw(float dt);


};

#endif // !__UNIT_H__
