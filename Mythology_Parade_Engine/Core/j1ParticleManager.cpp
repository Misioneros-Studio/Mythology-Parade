#include "j1ParticleManager.h"
#include "p2Log.h"
#include "j1Textures.h"
j1ParticleManager::j1ParticleManager()
{
	texture = nullptr;
}

j1ParticleManager::~j1ParticleManager()
{
	texture = nullptr;
}

void j1ParticleManager::CreateParticle()
{
	ClassicAnimation anim1;

	anim1.PushBack(SDL_Rect{ 256, 193, 10, 10 }, 1, 0, 0);

	particleList.push_back(new j1Particle(100, 100, 0, 0, 0, 0, 0, 0, 10.f,texture , anim1, false));

}

bool j1ParticleManager::Start()
{
	LOG("j1ParticleManager Started");
	texture = App->tex->Load("assets/particles.png");
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
	return true;
}



