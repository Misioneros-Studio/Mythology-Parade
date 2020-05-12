#include "j1ParticleSystem.h"
#include "j1Emiter.h"
#include "j1Particle.h"

j1ParticleSystem::j1ParticleSystem() : position{ 0, 0 }, active(true){

}

j1ParticleSystem::j1ParticleSystem(float x, float y) : position{ x, y }, active(true) {

}

j1ParticleSystem::~j1ParticleSystem()
{
	emiterVector.clear();
}


void j1ParticleSystem::Update(float dt)
{
	int numEmiters = emiterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].Update(dt);
	}

}


void j1ParticleSystem::PostUpdate(float dt)
{
	int numEmiters = emiterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].PostUpdate(dt);
	}
}


void j1ParticleSystem::PushEmiter(j1Emiter& emiter)
{
	emiterVector.push_back(emiter);
}


void j1ParticleSystem::Desactivate()
{
	int numEmiters = emiterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].Desactivate();
	}

	active = false;
}


void j1ParticleSystem::Activate()
{
	int numEmiters = emiterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].Activate();
	}

	active = true;
}


bool j1ParticleSystem::IsActive()
{
	return active;
}

//Move the particle system, and its emiters in relation to the particle system
void j1ParticleSystem::Move(int x, int y)
{
	int previousX = position[0];
	int previousY = position[1];

	int xPos, yPos;

	if (active)
	{
		int numEmiters = emiterVector.size();

		for (int i = 0; i < numEmiters; i++)
		{
			emiterVector[i].GetPosition(xPos, yPos);

			xPos += x - previousX;
			yPos += y - previousY;

			emiterVector[i].SetPosition(xPos, yPos);
		}
	}

	position[0] = x;
	position[1] = y;
}