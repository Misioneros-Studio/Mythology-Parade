#include "j1ParticleManager.h"
#include "p2Log.h"
#include "j1Textures.h"


j1ParticleManager::j1ParticleManager()
{
	name.append("ParticleManager");
	current_animation = explosion_animation;
	texture = nullptr;
}

j1ParticleManager::~j1ParticleManager()
{

}


bool j1ParticleManager::Start()
{
	LOG("j1ParticleManager Started");

	texture = App->tex->Load("assets/particles.png");
	CreateExplosionAnimation();
	CreateArrowsCursorAnimation();
	CreateSkullAnimation();
	CreateLevelUpAnimation();
	CreateTutorialArrowDownAnimation();
	CreateTutorialArrowUpAnimation();
	CreateTutorialArrowRightAnimation();
	CreateTutorialArrowLeftAnimation();

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

	}
	return true;
}

bool j1ParticleManager::PostUpdate()
{
	for each (j1Particle * particle in particleList)
	{
		particle->PostUpdate();
		if (!particle->IsActive()) {
			particleList.remove(particle);
			delete particle;
			break;
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
	texture = nullptr;

	return true;
}

void j1ParticleManager::CreateParticle(iPoint pos, fPoint speed, float life, ParticleAnimation animation, float blit_speed)
{
	UpdateParticleAnimation(animation);

	particleList.push_back(new j1Particle(pos.x, pos.y, speed.x, speed.y, 0, 0, 0, 0, life, texture, current_animation, blit_speed, false));
}


void j1ParticleManager::UpdateParticleAnimation(ParticleAnimation animation)
{
	switch (animation)
	{
	case ParticleAnimation::Explosion:
		current_animation = explosion_animation;
		break;
	case ParticleAnimation::Arrows_Cursor:
		current_animation = arrows_animation;
		break;
	case ParticleAnimation::Skull:
		current_animation = skull_animation;
		break;
	case ParticleAnimation::Level_Up:
		current_animation = levelup_animation;
		break;
	case ParticleAnimation::Tutorial_Arrow_Down:
		current_animation = tutorial_arrow_down_animation;
		break;
	case ParticleAnimation::Tutorial_Arrow_Up:
		current_animation = tutorial_arrow_up_animation;
		break;
	case ParticleAnimation::Tutorial_Arrow_Left:
		current_animation = tutorial_arrow_left_animation;
		break;
	case ParticleAnimation::Tutorial_Arrow_Right:
		current_animation = tutorial_arrow_right_animation;
		break;
	}
}

void j1ParticleManager::CreateTutorialArrowRightAnimation()
{
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 0, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 58, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 116, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 174, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 232, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 290, 97, 58 }, 2, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 232, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 174, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 116, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 58, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.PushBack(SDL_Rect{ 1279, 0, 97, 58 }, 1, 0, 0);
	tutorial_arrow_right_animation.speed = 15.f;
	tutorial_arrow_right_animation.loop = true;
}

void j1ParticleManager::CreateTutorialArrowLeftAnimation()
{
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 348, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 406, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 464, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 522, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 580, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 638, 97, 58 }, 2, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 580, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 522, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 464, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 406, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.PushBack(SDL_Rect{ 1279, 348, 97, 58 }, 1, 0, 0);
	tutorial_arrow_left_animation.speed = 15.f;
	tutorial_arrow_left_animation.loop = true;
}

void j1ParticleManager::CreateTutorialArrowUpAnimation()
{
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 348, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 406, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 464, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 522, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 580, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 638, 688, 58, 97 }, 2, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 580, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 522, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 464, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 406, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.PushBack(SDL_Rect{ 348, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_up_animation.speed = 15.f;
	tutorial_arrow_up_animation.loop = true;
}

void j1ParticleManager::CreateTutorialArrowDownAnimation()
{
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 0, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 58, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 116, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 174, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 232, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 290, 688, 58, 97 }, 2, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 232, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 174, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 116, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 58, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.PushBack(SDL_Rect{ 0, 688, 58, 97 }, 1, 0, 0);
	tutorial_arrow_down_animation.speed = 15.f;
	tutorial_arrow_down_animation.loop = true;
}

void j1ParticleManager::CreateExplosionAnimation()
{
	explosion_animation.PushBack(SDL_Rect{ 0, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 516, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 688, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 860, 0, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1032, 0, 172, 172 }, 2, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 0, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 516, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 688, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 860, 172, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1032, 172, 172, 172 }, 2, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 0, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 516, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 688, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 860, 344, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 1032, 344, 172, 172 }, 2, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 0, 516, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 172, 516, 172, 172 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 344, 516, 172, 172 }, 1, 0, 0);
	explosion_animation.speed = 15.f;
	explosion_animation.loop = false;
}


void j1ParticleManager::CreateArrowsCursorAnimation()
{
	arrows_animation.PushBack(SDL_Rect{ 687, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 751, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 815, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 879, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 943, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1007, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1071, 558, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 687, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 751, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 815, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 879, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 943, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1007, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.PushBack(SDL_Rect{ 1071, 590, 64, 32 }, 1, 0, 0);
	arrows_animation.speed = 300.f;
	arrows_animation.loop = false;
}

void j1ParticleManager::CreateSkullAnimation()
{
	skull_animation.PushBack(SDL_Rect{ 687, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 719, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 751, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 783, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 815, 622, 32, 29 }, 1, 0, 0);
	skull_animation.PushBack(SDL_Rect{ 847, 622, 32, 29 }, 1, 0, 0);
	skull_animation.speed = 20.f;
	skull_animation.loop = false;
}

void j1ParticleManager::CreateLevelUpAnimation()
{
	levelup_animation.PushBack(SDL_Rect{ 687, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 719, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 751, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 783, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 815, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.PushBack(SDL_Rect{ 847, 651, 32, 22 }, 1, 0, 0);
	levelup_animation.speed = 20.f;
	levelup_animation.loop = false;
}