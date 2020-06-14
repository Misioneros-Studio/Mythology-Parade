#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vector"
#include "ClassicAnimation.h"

struct SDL_Texture;

class j1Particle
{
public:
	j1Particle();
	j1Particle(float life, SDL_Texture* texture, ClassicAnimation animation, bool fade = false); //The emiter uses this constructor
	j1Particle(std::vector<float>& position, std::vector<float>& speed, std::vector<float>& acceleration, float angle, float angularSpeed, float life, SDL_Texture* texture, ClassicAnimation animation, bool fade = false);
	j1Particle(float positionX, float positionY, float speedX, float speedY, float accelerationX, float accelerationY, float angle, float angularSpeed, float life, SDL_Texture* texture, ClassicAnimation animation, float blit_speed, bool fade = false);
	~j1Particle();

	//Getters and setters
	std::vector<float> GetPosition();
	std::vector<float> GetSpeed();
	std::vector<float> GetAcceleration();

	float GetAngle();
	float GetAngularSpeed();

	float GetLife();
	SDL_Texture* GetTexture();

	void SetPosition(std::vector<float>&);
	void SetSpeed(std::vector<float>&);
	void SetAcceleration(std::vector<float>&);

	void SetAngle(float);
	void SetAngularSpeed(float);

	//the actual useful functions
	void Update(float dt);
	void PostUpdate();

	bool Activate();
	void Reset(float x, float y, float speedX, float speedY, float accX, float accY, float angularSpeed);
	void Desactivate();
	bool IsActive();

private:
	void Move(float dt);
	void Draw(float dt);

	void CheckLife(float dt);

private:
	SDL_Texture* texture;
	ClassicAnimation animation;

	std::vector<float> position; // 0 is the x axis, and 1 the y axis
	std::vector<float> speed;
	std::vector<float> acceleration;

	float b_speed;
	float angle;
	float angularSpeed;//positive = right, negative = left

	float life;
	float originalLife;
	float dt_particle;

	bool fade;
	bool active;
};

#endif //__PARTICLE_H__