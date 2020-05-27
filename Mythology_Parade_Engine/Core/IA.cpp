#include "IA.h"
#include "j1App.h"

IA::IA()
{
	gamePhase = GameBehaviour::EARLY;
	early = EarlyGameBehaviour::CREATION;
	mid = MidGameBehaviour::ASSEMBLE;
	late = LateGameBehaviour::ATACK;



}

bool IA::PreUpdate()
{
	bool ret = true;

	switch (gamePhase)
	{
	case GameBehaviour::EARLY:
		EarlyGame_Pre();
		break;
	case GameBehaviour::MID:
		MidGame_Pre();
		break;
	case GameBehaviour::LATE:
		LateGame_Pre();
		break;
	default:
		break;
	}

	return ret;
}

bool IA::Update(float dt)
{
	bool ret = true;

	switch (gamePhase)
	{
	case GameBehaviour::EARLY:
		EarlyGame_Upd();
		break;
	case GameBehaviour::MID:
		MidGame_Upd();
		break;
	case GameBehaviour::LATE:
		LateGame_Upd();
		break;
	default:
		break;
	}

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

void IA::EarlyGame_Pre()
{
	switch (early)
	{
	case EarlyGameBehaviour::CREATION:
		break;
	case EarlyGameBehaviour::BASIC_RESEARCH:
		break;
	case EarlyGameBehaviour::BASIC_BUILDINGS_CREATION:
		break;
	case EarlyGameBehaviour::RESEARCH_CLERIC:
		break;
	case EarlyGameBehaviour::BASIC_UNITS_CREATION:
		break;
	case EarlyGameBehaviour::EXPLORE:
		break;
	default:
		break;
	}
}

void IA::MidGame_Pre()
{
	switch (mid)
	{
	case MidGameBehaviour::ASSEMBLE:
		break;
	case MidGameBehaviour::CREATE_ECONOMY:
		break;
	case MidGameBehaviour::RESEARCH_ASSASSIN:
		break;
	case MidGameBehaviour::CREATE_ASSASSIN:
		break;
	case MidGameBehaviour::DISTRIBUTION:
		break;
	default:
		break;
	}
}

void IA::LateGame_Pre()
{
	switch (late)
	{
	case LateGameBehaviour::ATACK:
		break;
	case LateGameBehaviour::ECONOMY_FOCUS:
		break;
	case LateGameBehaviour::DEFENSE:
		break;
	case LateGameBehaviour::WIN:
		break;
	default:
		break;
	}
}

void IA::EarlyGame_Upd()
{
	switch (early)
	{
	case EarlyGameBehaviour::CREATION:
		break;
	case EarlyGameBehaviour::BASIC_RESEARCH:
		break;
	case EarlyGameBehaviour::BASIC_BUILDINGS_CREATION:
		break;
	case EarlyGameBehaviour::RESEARCH_CLERIC:
		break;
	case EarlyGameBehaviour::BASIC_UNITS_CREATION:
		break;
	case EarlyGameBehaviour::EXPLORE:
		break;
	default:
		break;
	}
}

void IA::MidGame_Upd()
{
	switch (mid)
	{
	case MidGameBehaviour::ASSEMBLE:
		break;
	case MidGameBehaviour::CREATE_ECONOMY:
		break;
	case MidGameBehaviour::RESEARCH_ASSASSIN:
		break;
	case MidGameBehaviour::CREATE_ASSASSIN:
		break;
	case MidGameBehaviour::DISTRIBUTION:
		break;
	default:
		break;
	}
}

void IA::LateGame_Upd()
{
	switch (late)
	{
	case LateGameBehaviour::ATACK:
		break;
	case LateGameBehaviour::ECONOMY_FOCUS:
		break;
	case LateGameBehaviour::DEFENSE:
		break;
	case LateGameBehaviour::WIN:
		break;
	default:
		break;
	}
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


