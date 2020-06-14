#include "j1ParticleManager.h"
#include "p2Log.h"
#include "j1Textures.h"


j1ParticleManager::j1ParticleManager()
{
	name.append("ParticleManager");
	current_animation = explosion_animation;
	texture = nullptr;
}

j1ParticleManager::~j1ParticleManager()
{

}


bool j1ParticleManager::Start()
{
	LOG("j1ParticleManager Started");

	texture = App->tex->Load("assets/particles.png");
	CreateExplosionAnimation();
	CreateArrowsCursorAnimation();
	CreateSkullAnimation();
	CreateLevelUpAnimation();
	CreateTutorialArrowAnimation();

	return true;
}

bool j1ParticleManager::Update(float dt)
{
	if (!particleList.empty()) 
	{

		for each (j1Particle* particle in particleList)
		{
			particle->Update(dt);
		}

		for each (j1Particle* particle in particleList)
		{
			particle->PostUpdate(dt);
			if (!particle->IsActive()) {
				particleList.remove(particle);
				delete particle;
					break;
			}
		}
	}
	return true;
}

bool j1ParticleManager::CleanUp()
{
	for each (j1Particle* particle in particleList)
	{
		delete particle;
	}
	particleList.clear();
	texture = nullptr;

	return true;
}

void j1ParticleManager::CreateParticle(iPoint pos, fPoint speed, float life, ParticleAnimation animation, float blit_speed)
{
	UpdateParticleAnimation(animation);

	particleList.push_back(new j1Particle(pos.x, pos.y, speed.x, speed.y, 0, 0, 0, 0, life, texture, current_animation, blit_speed, false));
}


void j1ParticleManager::UpdateParticleAnimation(ParticleAnimation animation)
{
	switch (animation)
	{
	case ParticleAnimation::Explosion:
		current_animation = explosion_animation;
		break;
	case ParticleAnimation::Arrows_Cursor:
		current_animation = arrows_animation;
		break;
	case ParticleAnimation::Skull:
		current_animation = skull_animation;
		break;
	case ParticleAnimation::Level_Up:
		current_animation = levelup_animation;
		break;
	case ParticleAnimation::Tutorial_Arrow:
		current_animation = tutorial_arrow_animation;
		break;
	}
}

void j1ParticleManager::CreateTutorialArrowAnimation()
{
	tutorial_arrow_animation.PushBack(SDL_Rect{ 0, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 58, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 116, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 174, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 232, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 290, 688, 58, 97 }, 2, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 232, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 174, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 116, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 58, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.PushBack(SDL_Rect{ 0, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_animation.speed = 15.f;
	tutorial_arrow_animation.loop = true;
}

void j1ParticleManager::CreateExplosionAnimation()
{
	explosion_animation.PushBack(SDL_Rect{ 0, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 516, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 688, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 860, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1032, 0, 172, 172 }, 2, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1204, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 0, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 516, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 688, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 860, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1032, 172, 172, 172 }, 2, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1204, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 0, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 516, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 688, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 860, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1032, 344, 172, 172 }, 2, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1204, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 0, 516, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 516, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 516, 172, 172 }, 1, 0, 0);
	explosion_animation.speed = 15.f;
	explosion_animation.loop = false;
}


void j1ParticleManager::CreateArrowsCursorAnimation()
{
	arrows_animation.PushBack(SDL_Rect{ 687, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 751, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 815, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 879, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 943, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1007, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1071, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 687, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 751, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 815, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 879, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 943, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1007, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1071, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.speed = 300.f;
	arrows_animation.loop = false;
}

void j1ParticleManager::CreateSkullAnimation()
{
	skull_animation.PushBack(SDL_Rect{ 687, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 719, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 751, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 783, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 815, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 847, 622, 32, 29 }, 1, 0, 0);
	skull_animation.speed = 20.f;
	skull_animation.loop = false;
}

void j1ParticleManager::CreateLevelUpAnimation()
{
	levelup_animation.PushBack(SDL_Rect{ 687, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 719, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 751, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 783, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 815, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 847, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.speed = 20.f;
	levelup_animation.loop = false;
}