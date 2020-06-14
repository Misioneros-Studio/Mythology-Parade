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
	Tutorial_Arrow
};

class j1ParticleManager : public j1Module
{
public:
	j1ParticleManager();
	~j1ParticleManager();

	void CreateParticle(iPoint pos, fPoint speed, float life, ParticleAnimation animation);

	void UpdateParticleAnimation(ParticleAnimation animation);

	bool Start() override;
	void CreateTutorialArrowAnimation();
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
	ClassicAnimation tutorial_arrow_animation;

	ClassicAnimation current_animation;

};



#endif // !__j1PARTICLEMANAGER_H__
