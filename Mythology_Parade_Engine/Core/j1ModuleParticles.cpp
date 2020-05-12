#include "j1App.h"
#include "j1ModuleParticles.h"
#include "j1Emiter.h"
#include "j1ParticleSystem.h"
#include "j1Textures.h"
#include "j1Input.h"

j1ModuleParticles::j1ModuleParticles(bool start_enabled) :

	j1Module(),
	particleSystem(nullptr),
	rect{ 0, 0, 0, 0 },
	rect2{ 0, 0, 0, 0 }
{}

j1ModuleParticles::~j1ModuleParticles()
{}


bool j1ModuleParticles::Start()
{
	particleSystem = new j1ParticleSystem();
	rect = { 0, 0, 10, 400 };

	rect2 = { 0, 0, 550, 10 };

	ClassicAnimation anim;
	anim.PushBack(SDL_Rect{ 0, 0, 10, 10 }, 1, 0, 0);

	//TODO 4: Create an emiter and push it to the particle system

	//Emiter emiter(80, 90, 2, 2, 15, -5, 0, 0, 2, 1, 5, 0, 50, 1, nullptr, App->textures->Load("particleTextures/redParticle.png"), anim, true);

	//particleSystem->PushEmiter(emiter);

	//TODO 5: Uncomment this :D

	SDL_Texture* snowball = App->tex->Load("particleTextures/bolaNieve.png");

	ClassicAnimation anim1;
	anim1.PushBack(SDL_Rect{ 0, 0, 10, 10 }, 1, 0, 0);

	ClassicAnimation anim2;
	anim2.PushBack(SDL_Rect{ 0, 12, 6, 6 }, 1, 0, 0);

	ClassicAnimation anim3;
	anim3.PushBack(SDL_Rect{ 7, 11, 8, 8 }, 1, 0, 0);

	j1Emiter emiterBigSnowBall(0, 20, 2, 2, 2, -3, 0, 0, 0, 1, 5, 0, 6, 3.5, &rect, snowball, anim1, true);
	j1Emiter emiterMidSnowBall(0, 20, 2, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect, snowball, anim2, true);
	j1Emiter emiterLittleSnowBall(0, 20, 3, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect, snowball, anim3, true);
	
	j1Emiter emiterBigSnowBall2(20, 0, 2, 2, 2, -3, 0, 0, 0, 1, 5, 0, 6, 3.5, &rect2, snowball, anim1, true);
	j1Emiter emiterMidSnowBall2(20, 0, 2, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect2, snowball, anim2, true);
	j1Emiter emiterLittleSnowBall2(20, 0, 3, 2, 3, -2, 0, 0, 0, 1, 5, 0, 8, 3.5, &rect2, snowball, anim3, true);

	particleSystem->PushEmiter(emiterBigSnowBall);
	particleSystem->PushEmiter(emiterMidSnowBall);
	particleSystem->PushEmiter(emiterLittleSnowBall);

	particleSystem->PushEmiter(emiterBigSnowBall2);
	particleSystem->PushEmiter(emiterMidSnowBall2);
	particleSystem->PushEmiter(emiterLittleSnowBall2);

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

	return true;
}