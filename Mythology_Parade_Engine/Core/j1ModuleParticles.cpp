#include "j1App.h"
#include "j1ModuleParticles.h"
#include "j1Emiter.h"
#include "j1ParticleSystem.h"
#include "j1Particle.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"

j1ModuleParticles::j1ModuleParticles(bool start_enabled) :

	j1Module(),
	particleSystem(nullptr),
	rect{ 0, 0, 0, 0 },
	rect2{ 0, 0, 0, 0 }
{
	particleSystem = new j1ParticleSystem();

}

j1ModuleParticles::~j1ModuleParticles()
{}


bool j1ModuleParticles::Start()
{
	explosion_animation.PushBack(SDL_Rect{ 0, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 59, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 118, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 177, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 236, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 295, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 354, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.PushBack(SDL_Rect{ 413, 0, 59, 59 }, 1, 0, 0);
	explosion_animation.speed = 20;
	explosion_animation.loop = false;

	arrow_animation.PushBack(SDL_Rect{ 256, 193, 10, 10 }, 1, 0, 0);
	arrow_animation.PushBack(SDL_Rect{ 256, 207, 6, 6 }, 1, 0, 0);
	arrow_animation.PushBack(SDL_Rect{ 264, 205, 8, 8 }, 1, 0, 0);

	particles_tex = App->tex->Load("assets/particles.png");
	arrow_rect = { 0,0,64,32 };

	
	return true;
}

bool j1ModuleParticles::PreUpdate()
{
	/*
	if (App->input->GetKey(SDL_SCANCODE_1))
	{
		particleSystem->Activate();
	}

	if (App->input->GetKey(SDL_SCANCODE_2))
	{
		particleSystem->Desactivate();
	}

	if (App->input->GetKey(SDL_SCANCODE_3))
	{
		particleSystem->Move(App->input->GetMouseX(), App->input->GetMouseY());
	}
	*/

	return true;
}


bool j1ModuleParticles::Update(float dt)
{
	particleSystem->Update(dt);

	dtime = dt;

	return true;
}


bool j1ModuleParticles::PostUpdate()
{
	particleSystem->PostUpdate(dtime);

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		DeleteAllParticles();

	return true;
}

bool j1ModuleParticles::CleanUp()
{

	particleSystem->emiterVector.clear();

	return true;
}

void j1ModuleParticles::DeleteAllParticles()
{
	//TODO 10 revise list
	particleSystem->emiterVector.clear();
}

void j1ModuleParticles::DoUnitsPathParticles(int pos_x, int pos_y)
{
	fPoint pos_global= App->map->MapToWorld(pos_x, pos_y);

	j1Emiter emiter(pos_global.x, pos_global.y,0,-1,0,2,0,0,0,0,0,0,30,0.2f, &arrow_rect, particles_tex, arrow_animation, true, 1);
	j1Emiter explosion(pos_global.x, pos_global.y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, &arrow_rect, particles_tex, explosion_animation, true, 1);

	particleSystem->PushEmiter(emiter);
	particleSystem->PushEmiter(explosion);
}

void j1ModuleParticles::DoLogoScreenParticles()
{
	rect = { 0, 0, 10, 400 };
	rect2 = { 0, 0, 550, 10 };

	ClassicAnimation anim1;
	ClassicAnimation anim2;
	ClassicAnimation anim3;

	anim1.PushBack(SDL_Rect{ 256, 193, 10, 10 }, 1, 0, 0);
	anim2.PushBack(SDL_Rect{ 256, 207, 6, 6 }, 1, 0, 0);
	anim3.PushBack(SDL_Rect{ 264, 205, 8, 8 }, 1, 0, 0);

	j1Emiter emiterBigSnowBall(0, 20, 2, 2, 2, -3, 0, 0, 0, 1, 5, 0, 6, 3.5, &rect, App->tex->Load("assets/particles.png"), anim1, true, 0);
	j1Emiter emiterMidSnowBall(0, 20, 2, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect, App->tex->Load("assets/particles.png"), anim2, true, 0);
	j1Emiter emiterLittleSnowBall(0, 20, 3, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect, App->tex->Load("assets/particles.png"), anim3, true, 0);

	j1Emiter emiterBigSnowBall2(20, 0, 2, 2, 2, -3, 0, 0, 0, 1, 5, 0, 6, 3.5, &rect2, App->tex->Load("assets/particles.png"), anim1, true, 0);
	j1Emiter emiterMidSnowBall2(20, 0, 2, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect2, App->tex->Load("assets/particles.png"), anim2, true, 0);
	j1Emiter emiterLittleSnowBall2(20, 0, 3, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect2, App->tex->Load("assets/particles.png"), anim3, true, 0);

	particleSystem->PushEmiter(emiterBigSnowBall);
	particleSystem->PushEmiter(emiterMidSnowBall);
	particleSystem->PushEmiter(emiterLittleSnowBall);

	particleSystem->PushEmiter(emiterBigSnowBall2);
	particleSystem->PushEmiter(emiterMidSnowBall2);
	particleSystem->PushEmiter(emiterLittleSnowBall2);

}

void j1ModuleParticles::DoExplosion(int pos_x, int pos_y)
{
	fPoint pos_global = App->map->MapToWorld(pos_x, pos_y);

	j1Emiter explosion(pos_global.x, pos_global.y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, &arrow_rect, particles_tex, explosion_animation, true, 1);

	particleSystem->PushEmiter(explosion);
}