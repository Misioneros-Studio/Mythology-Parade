#include "j1ParticleManager.h"
#include "j1Textures.h"
j1ParticleManager::j1ParticleManager()
{
	texture = nullptr;
}

j1ParticleManager::~j1ParticleManager()
{
	texture = nullptr;
}

j1Particle j1ParticleManager::CreateParticle()
{
	//ClassicAnimation 
	//particleList.push_back(j1Particle(100,100,0,0,0,0,0,0,10,App->tex->Load("assets/particles.png"),)
}
