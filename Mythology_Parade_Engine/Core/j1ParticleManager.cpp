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

void j1ParticleManager::CreateParticle(iPoint pos, fPoint speed, float life, ParticleAnimation animation)
{
	UpdateParticleAnimation(animation);

	particleList.push_back(new j1Particle(pos.x, pos.y, speed.x, speed.y, 0, 0, 0, 0, life, texture, current_animation, false));
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
	}
}

void j1ParticleManager::CreateExplosionAnimation()
{
	explosion_animation.PushBack(SDL_Rect{ 0, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 59, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 118, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 177, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 236, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 295, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 354, 0, 59, 59 }, 2, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 413, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.speed = 15.f;
	explosion_animation.loop = true;
}


void j1ParticleManager::CreateArrowsCursorAnimation()
{
	arrows_animation.PushBack(SDL_Rect{ 0, 256, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 64, 256, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 128, 256, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 192, 256, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 256, 256, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 320, 256, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 384, 256, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 0, 288, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 64, 288, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 128, 288, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 192, 288, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 256, 288, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 320, 288, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 384, 288, 64, 32 }, 1, 0, 0);
	arrows_animation.speed = 300.f;
	arrows_animation.loop = false;
}

void j1ParticleManager::CreateSkullAnimation()
{
	skull_animation.PushBack(SDL_Rect{ 0, 320, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 32, 320, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 64, 320, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 96, 320, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 128, 320, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 160, 320, 32, 29 }, 1, 0, 0);
	skull_animation.speed = 20.f;
	skull_animation.loop = false;
}

void j1ParticleManager::CreateLevelUpAnimation()
{
	levelup_animation.PushBack(SDL_Rect{ 0, 349, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 32, 349, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 64, 349, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 96, 349, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 128, 349, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 160, 349, 32, 22 }, 1, 0, 0);
	levelup_animation.speed = 20.f;
	levelup_animation.loop = false;
}