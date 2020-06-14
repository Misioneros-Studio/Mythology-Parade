#include "j1Particle.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"


j1Particle::j1Particle()
{}


j1Particle::j1Particle(std::vector<float>& position, std::vector<float>& speed, std::vector<float>& acceleration,
	float angle, float angularSpeed, float life, SDL_Texture* texture, ClassicAnimation animation, bool fade) :

	position(position),
	speed(speed),
	acceleration(acceleration),
	angle(angle),
	angularSpeed(angularSpeed),

	life(life),
	originalLife(life),

	texture(texture),
	animation(animation),

	fade(fade),
	active(false)

{}


j1Particle::j1Particle(float life, SDL_Texture* texture, ClassicAnimation animation, bool fade) :

	position{ NULL, NULL },
	speed{ NULL, NULL },
	acceleration{ NULL, NULL },
	angle(0),
	angularSpeed(0),

	life(life),
	originalLife(life),

	texture(texture),
	animation(animation),

	fade(fade),
	active(false)
{}



j1Particle::j1Particle(float positionX, float positionY, float speedX, float speedY, float accelerationX, float accelerationY,
	float angle, float angularSpeed, float life, SDL_Texture* texture, ClassicAnimation animation, float blit_speed, bool fade) :

	position{ positionX, positionY },
	speed{ speedX, speedY },
	acceleration{ accelerationX, accelerationY },
	angle(angle),
	angularSpeed(angularSpeed),

	life(life),
	originalLife(life),

	texture(texture),
	animation(animation),

	fade(fade),
	active(true),
	b_speed(blit_speed)

{}


j1Particle::~j1Particle()
{
	position.clear();
	speed.clear();
	acceleration.clear();

	texture = nullptr;
}


std::vector<float> j1Particle::GetPosition() {
	return position;
}


std::vector<float> j1Particle::GetSpeed() {
	return speed;
}


std::vector<float> j1Particle::GetAcceleration() {
	return acceleration;
}


float j1Particle::GetAngle() {
	return angle;
}


float j1Particle::GetAngularSpeed() {
	return angularSpeed;
}


float j1Particle::GetLife() {
	return life;
}


SDL_Texture* j1Particle::GetTexture() {
	return texture;
}


void j1Particle::SetPosition(std::vector<float>& pos) {
	position = pos;
}


void j1Particle::SetSpeed(std::vector<float>& spd) {
	speed = spd;
}


void j1Particle::SetAcceleration(std::vector<float>& acc) {
	acceleration = acc;
}


void j1Particle::SetAngle(float ang) {
	angle = ang;
}


void j1Particle::SetAngularSpeed(float aspd) {
	angularSpeed = aspd;
}


void j1Particle::Update(float dt)
{
	if (active)
	{
		Move(dt);
		CheckLife(dt);
	}
}


void j1Particle::PostUpdate(float dt)
{
	if (active)
		Draw(dt);
}


void j1Particle::Draw(float dt)
{
	bool last = false;
	App->render->Blit(texture, position[0], position[1], &animation.GetCurrentFrameBox(dt,last),b_speed);
	if (last)
		Desactivate();
}

void j1Particle::Move(float dt)
{
	speed[0] += acceleration[0] * dt * 50;
	speed[1] += acceleration[1] * dt * 50;

	//TODO 1: Do the same with position and angles
	position[0] += speed[0] * dt * 50;
	position[1] += speed[1] * dt * 50;

	angle += angularSpeed * dt * 50;
}


void j1Particle::CheckLife(float dt)
{
	life -= dt;

	if (life < 0)
	{
		Desactivate();
	}
}


void j1Particle::Desactivate()
{
	active = false;
}

bool j1Particle::IsActive()
{
	return active;
}


bool j1Particle::Activate()
{
	return !active;
}


void j1Particle::Reset(float x, float y, float speedX, float speedY, float accX, float accY, float angSpeed)
{
	life = originalLife;

	position[0] = x;
	position[1] = y;

	speed[0] = speedX;
	speed[1] = speedY;

	acceleration[0] = accX;
	acceleration[1] = accY;

	angularSpeed = angSpeed;

	active = true;
}