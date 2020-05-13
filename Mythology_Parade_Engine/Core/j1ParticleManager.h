#ifndef __j1PARTICLEMANAGER_H__
#define __j1PARTICLEMANAGER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "j1Particle.h"
#include <vector>

class j1ParticleManager : public j1Module
{
public:
	j1ParticleManager();
	~j1ParticleManager();

	void CreateParticle();

	bool Start() override;
	bool Update(float dt) override;
	//bool CleanUp() override;

private:

	std::vector<j1Particle> particleList;
	SDL_Texture* texture;

};



#endif // !__j1PARTICLEMANAGER_H__
