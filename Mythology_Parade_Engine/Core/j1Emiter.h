#pragma once

#include "vector"
#include "ClassicAnimation.h"

#define FPS 60
#define MSPF 16

class j1Particle;
struct SDL_Texture;

class j1Emiter
{
public:
	j1Emiter(std::vector<float>& position, std::vector<float>& particleSpeed, std::vector<int>& particleVariationSpeed, std::vector<float>& particleAcceleration, std::vector<int>& particleVariationAcceleration,
		float particleAngularSpeed, int particleVariableAngularSpeed, float particlesRate, float particlesLifeTime, SDL_Rect* areaOfSpawn, SDL_Texture* texture, ClassicAnimation particleAnimation, bool fade);

	j1Emiter(float positionX, float positionY, float particleSpeedX, float particleSpeedY, int particleVariationSpeedX, int particleVariationSpeedY, float particleAccelerationX,
		float particleAccelerationY, int particleVariationAccelerationX, int particleVariationAccelerationY, float particleAngularSpeed, int particleVariableAngularSpeed,
		float particlesRate, float particlesLifeTime, SDL_Rect* areaOfSpawn, SDL_Texture* texture, ClassicAnimation particleAnimation, bool fade);

	~j1Emiter();

	void Update(float dt);
	void PostUpdate(float dt);

	void GetPosition(int& x, int& y);
	void SetPosition(int x, int y);

	void Activate();
	void Desactivate();

private:
	void Start();
	void CreateParticle();
	void ThrowParticles();

	//Generate functions, used in ThrowParticles
	float GeneratePosX();
	float GeneratePosY();

	float GenerateSpeedX();
	float GenerateSpeedY();

	float GenerateAccelerationX();
	float GenerateAccelerationY();

	float GenerateAngularSpeed();

private:
	std::vector<float> position;
	std::vector<float> particleSpeed;
	std::vector<float> particleAcceleration;

	float particleAngularSpeed;

	std::vector<int> particleVariationSpeed;
	std::vector<int> particleVariationAcceleration;
	int particleVariationAngularSpeed;


	std::vector<j1Particle> particleVector;

	SDL_Rect* areaOfSpawn; //Optional
	SDL_Texture* particleTexture;
	ClassicAnimation particleAnimation;

	float particlesRate; //per second
	float particlesLifeTime;
	float particlesPerFrame;

	float particlesEmited;

	bool randomizePosX;
	bool randomizePosY;

	bool randomizeSpeedX;
	bool randomizeSpeedY;

	bool randomizeAccelerationX;
	bool randomizeAccelerationY;

	bool randomizeAngularSpeed;

	bool fadeParticles;

	bool active;
};