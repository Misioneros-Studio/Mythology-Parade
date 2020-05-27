#include "IA.h"

IA::IA()
{
}

bool IA::PreUpdate()
{
	bool ret = true;
	return ret;
}

bool IA::Update(float dt)
{
	bool ret = true;
	return ret;
}

bool IA::PostUpdate()
{
	bool ret = true;
	return ret;
}

bool IA::CleanUp()
{
	bool ret = true;
	return ret;
}

void IA::InitCiv()
{
}

void IA::ResearchFirstLv()
{
}

void IA::CreateReligiousBuilds()
{
}

void IA::CreateUnit(std::string, int)
{
}

bool IA::WantToResearch(std::string)
{
	return false;
}

void IA::InitExplore()
{
}

bool IA::CheckExplore()
{
	return false;
}

bool IA::Assemble()
{
	return false;
}

void IA::CreateMonastery(int)
{
}

void IA::Defense()
{
}

void IA::Atack()
{
}

void IA::Win()
{
}
