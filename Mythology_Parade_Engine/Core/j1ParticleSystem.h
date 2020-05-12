#pragma once
#include "vector"

class j1Emiter;

class j1ParticleSystem
{
public:
	j1ParticleSystem();
	j1ParticleSystem(float x, float y);

	~j1ParticleSystem();

	void PushEmiter(j1Emiter& emiter);

	void Update(float dt);
	void PostUpdate(float dt);

	void Desactivate();
	void Activate();

	bool IsActive();

	void Move(int x, int y);

public:
	std::vector<j1Emiter> emiterVector;

private:

	std::vector<float>	position;

	bool active;
};