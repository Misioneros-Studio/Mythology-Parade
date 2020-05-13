#ifndef __j1PARTICLEMANAGER_H__
#define __j1PARTICLEMANAGER_H__

#include "SDL/include/SDL.h"
#include "j1Particle.h"
#include <vector>

class j1ParticleManager
{
public:
	j1ParticleManager();
	~j1ParticleManager();

	j1Particle CreateParticle();

private:

	std::vector<j1Particle> particleList;
	SDL_Texture* texture;

};



#endif // !__j1PARTICLEMANAGER_H__
