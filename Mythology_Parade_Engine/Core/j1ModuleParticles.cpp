#include "j1App.h"
#include "j1ModuleParticles.h"
#include "j1Emiter.h"
#include "j1ParticleSystem.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"

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




	arrow_animation.PushBack(SDL_Rect{ 0, 0, 10, 10 }, 1, 0, 0);
	particles_tex = App->tex->Load("assets/particles.png");
	arrow_rect = { 0,0,64,32 };
	//TODO 4: Create an emiter and push it to the particle system



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

	j1Emiter emiter(pos_global.x, pos_global.y,0,-1,0,1,0,0,0,0,0,0,25,3, &arrow_rect, particles_tex, arrow_animation, true);

	particleSystem->PushEmiter(emiter);
}