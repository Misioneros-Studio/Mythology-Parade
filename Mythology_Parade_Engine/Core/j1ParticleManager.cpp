#include "j1ParticleManager.h"
#include "p2Log.h"
#include "j1Textures.h"


j1ParticleManager::j1ParticleManager()
{
	current_animation = explosion_animation;
	texture = nullptr;
}

j1ParticleManager::~j1ParticleManager()
{

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
	}
}

bool j1ParticleManager::Start()
{
	LOG("j1ParticleManager Started");

	texture = App->tex->Load("assets/particles.png");
	CreateExplosionAnimation();

	return true;
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
				if (particleList.empty())
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



