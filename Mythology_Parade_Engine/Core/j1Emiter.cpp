#include "j1Emiter.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Particle.h"
#include "math.h"

j1Emiter::j1Emiter(std::vector<float>& position, std::vector<float>& particleSpeed, std::vector<int>& particleVariationSpeed,
	std::vector<float>& particleAcceleration, std::vector<int>& particleVariationAcceleration, float particleAngularSpeed,
	int particleVariableAngularSpeed, float particlesRate, float particlesLifeTime, SDL_Rect* areaOfSpawn, SDL_Texture* texture, ClassicAnimation particleAnimation, bool fade) :

	position(position),
	particleSpeed(particleSpeed),
	particleVariationSpeed(particleVariationSpeed),
	particleAcceleration(particleAcceleration),
	particleVariationAcceleration(particleVariationAcceleration),
	particleAngularSpeed(particleAngularSpeed),
	particleVariationAngularSpeed(particleVariableAngularSpeed),

	particlesRate(particlesRate),
	particlesLifeTime(particlesLifeTime),

	areaOfSpawn(areaOfSpawn),
	particleTexture(texture),
	particleAnimation(particleAnimation),

	randomizePosX(true),
	randomizePosY(true),

	randomizeSpeedX(true),
	randomizeSpeedY(true),

	randomizeAccelerationX(true),
	randomizeAccelerationY(true),

	randomizeAngularSpeed(true),

	active(true),

	fadeParticles(fade)

{
	Start();
}



j1Emiter::j1Emiter(float positionX, float positionY, float particleSpeedX, float particleSpeedY, int particleVariationSpeedX, int particleVariationSpeedY,
	float particleAccelerationX, float particleAccelerationY, int particleVariationAccelerationX, int particleVariationAccelerationY, float particleAngularSpeed,
	int particleVariableAngularSpeed, float particlesRate, float particlesLifeTime, SDL_Rect* areaOfSpawn, SDL_Texture* texture, ClassicAnimation particleAnimation, bool fade, int time) : position{ positionX, positionY },
	particleSpeed{ particleSpeedX, particleSpeedY },
	particleVariationSpeed{ particleVariationSpeedX, particleVariationSpeedY },
	particleAcceleration{ particleAccelerationX, particleAccelerationY },
	particleVariationAcceleration{ particleVariationAccelerationX, particleVariationAccelerationY },
	particleAngularSpeed(particleAngularSpeed),
	particleVariationAngularSpeed(particleVariableAngularSpeed),

	particlesRate(particlesRate),
	particlesLifeTime(particlesLifeTime),

	areaOfSpawn(areaOfSpawn),
	particleTexture(texture),
	particleAnimation(particleAnimation),

	randomizePosX(true),
	randomizePosY(true),

	randomizeSpeedX(true),
	randomizeSpeedY(true),

	randomizeAccelerationX(true),
	randomizeAccelerationY(true),

	randomizeAngularSpeed(true),

	active(true),

	fadeParticles(fade),
	
	total_time(time)
{
	Start();
}

void j1Emiter::Start()
{
	int maxParticles = particlesRate * particlesLifeTime + 1;

	if (particlesRate == 1 || maxParticles==1) {
		CreateParticle();
	}
	else if (particlesRate > 1) {
		particlesPerFrame = particlesRate * 16 / 1000;

		particleVector.reserve(maxParticles);

		for (int i = 0; i < maxParticles; i++)
		{
			CreateParticle();
		}
	}

	if (areaOfSpawn == nullptr)
	{
		randomizePosX = false;
		randomizePosY = false;
	}

	else
	{
		if (areaOfSpawn->w == 0)
		{
			randomizePosX = false;
		}

		if (areaOfSpawn->h == 0)
		{
			randomizePosY = false;
		}
	}

	if (particleVariationSpeed[0] == NULL)
	{
		randomizeSpeedX = false;
	}

	if (particleVariationSpeed[1] == NULL)
	{
		randomizeSpeedY = false;
	}

	if (particleVariationAcceleration[0] == NULL)
	{
		randomizeAccelerationX = false;
	}

	if (particleVariationAcceleration[1] == NULL)
	{
		randomizeAccelerationY = false;
	}

	if (particleVariationAngularSpeed == 0)
	{
		randomizeAngularSpeed = false;
	}

	if (total_time != 0) {
		timer_particles.Start();
		do_it_one = true;
	}
}


void j1Emiter::CreateParticle()
{
	particleVector.push_back(j1Particle(particlesLifeTime, particleTexture, particleAnimation, fadeParticles));
}


j1Emiter::~j1Emiter() {

	particleVector.clear();

	position.clear();
	particleSpeed.clear();
	particleAcceleration.clear();

	particleVariationSpeed.clear();
	particleVariationAcceleration.clear();

	areaOfSpawn = nullptr;
	particleTexture = nullptr;
}


void j1Emiter::Update(float dt) {

	if (active)
	{
		ThrowParticles();
	}

	int numParticles = particleVector.size();

	if (do_it_one == true) {
		if (timer_particles.ReadSec() >= total_time) {
			this->Desactivate();
		}
	}

	for (int i = 0; i < numParticles; i++)
	{
		particleVector[i].Update(dt);
	}
}


void j1Emiter::PostUpdate(float dt) {

	int numParticles = particleVector.size();

	for (int i = 0; i < numParticles; i++)
	{
		particleVector[i].PostUpdate(dt);
	}

}


void j1Emiter::Desactivate()
{
	active = false;
}

void j1Emiter::Activate()
{
	active = true;
}


//This funtions activates the necesary particles and gives them the initial values
void j1Emiter::ThrowParticles() {

	//You could use delta time instead of particlesPerFrame, but i dont recommend it
	particlesEmited += particlesPerFrame;

	if (particlesEmited >= 1)
	{
		int emited = 0;

		for (int i = 0; i < particleVector.size(); i++)
		{
			if (particleVector[i].Activate())
			{
				particleVector[i].Reset(GeneratePosX(), GeneratePosY(), GenerateSpeedX(), GenerateSpeedY(), GenerateAccelerationX(), GenerateAccelerationY(), GenerateAngularSpeed());
				emited++;
			}

			if ((int)particlesEmited == emited)
				break;

		}

		particlesEmited -= emited;
	}
}


float j1Emiter::GeneratePosX()
{
	if (randomizePosX == true)
	{
		float x = (rand() % areaOfSpawn->w) + position[0];
		return x;
	}

	else
		return position[0];
}


float j1Emiter::GeneratePosY()
{
	if (randomizePosY == true)
	{
		float y = (rand() % areaOfSpawn->h) + position[1];
		return y;
	}

	else
		return position[1];
}


float j1Emiter::GenerateSpeedX()
{
	if (randomizeSpeedX == true)
	{
		float speedX = (rand() % particleVariationSpeed[0]) + particleSpeed[0];
		return speedX;
	}

	else
		return particleSpeed[0];
}


float j1Emiter::GenerateSpeedY()
{
	if (randomizeSpeedY == true)
	{
		float speedY = (rand() % particleVariationSpeed[1]) + particleSpeed[1];
		return speedY;
	}

	else
		return particleSpeed[1];

}


float j1Emiter::GenerateAccelerationX()
{
	if (randomizeAccelerationX == true)
	{
		float accX = (rand() % particleVariationAcceleration[0]) + particleAcceleration[0];
		return accX;
	}

	else
		return particleAcceleration[0];
}


float j1Emiter::GenerateAccelerationY()
{
	if (randomizeAccelerationY == true)
	{
		float accY = (rand() % particleVariationAcceleration[1]) + particleAcceleration[1];
		return accY;
	}

	else
		return particleAcceleration[1];
}


float j1Emiter::GenerateAngularSpeed()
{
	if (randomizeAngularSpeed == true)
	{
		float angularSpeed = (rand() % particleVariationAngularSpeed) + particleAngularSpeed;
		return angularSpeed;
	}

	else
		return particleAngularSpeed;
}


void j1Emiter::GetPosition(int& x, int& y)
{
	x = position[0];
	y = position[1];
}

void j1Emiter::SetPosition(int x, int y)
{
	position[0] = x;
	position[1] = y;
}