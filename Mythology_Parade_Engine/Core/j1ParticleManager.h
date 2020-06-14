#ifndef __j1PARTICLEMANAGER_H__
#define __j1PARTICLEMANAGER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "j1Particle.h"
#include <vector>
#include "p2Point.h"

enum class ParticleAnimation {
	Explosion,
	Arrows_Cursor,
	Skull,
	Level_Up,
	Tutorial_Arrow_Down,
	Tutorial_Arrow_Up,
	Tutorial_Arrow_Left,
	Tutorial_Arrow_Right
};

class j1ParticleManager : public j1Module
{
public:
	j1ParticleManager();
	~j1ParticleManager();

	void CreateParticle(iPoint pos, fPoint speed, float life, ParticleAnimation animation, float blit_speed = 1.0f);

	void UpdateParticleAnimation(ParticleAnimation animation);

	bool Start() override;
	void CreateTutorialArrowDownAnimation();
	void CreateTutorialArrowUpAnimation();
	void CreateTutorialArrowRightAnimation();
	void CreateTutorialArrowLeftAnimation();
	void CreateLevelUpAnimation();
	void CreateSkullAnimation();
	void CreateArrowsCursorAnimation();
	bool Update(float dt) override;
	bool CleanUp() override;

private:

	void CreateExplosionAnimation();
	std::list<j1Particle*> particleList;
	SDL_Texture* texture;
	ClassicAnimation explosion_animation;
	ClassicAnimation arrows_animation;
	ClassicAnimation skull_animation;
	ClassicAnimation levelup_animation;
	ClassicAnimation tutorial_arrow_down_animation;
	ClassicAnimation tutorial_arrow_up_animation;
	ClassicAnimation tutorial_arrow_left_animation;
	ClassicAnimation tutorial_arrow_right_animation;

	ClassicAnimation current_animation;

};



#endif // !__j1PARTICLEMANAGER_H__
