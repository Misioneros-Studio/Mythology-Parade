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

	return ret;
}

bool IA::Update(float dt)
{
	bool ret = true;

	switch (gamePhase)
	{
	case GameBehaviour::EARLY:
		EarlyGame();
		break;
	case GameBehaviour::MID:
		MidGame();
		break;
	case GameBehaviour::LATE:
		LateGame();
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

void IA::EarlyGame()
{
	switch (early)
	{
	case EarlyGameBehaviour::CREATION:
		InitCiv();
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

void IA::MidGame()
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

void IA::LateGame()
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

bool IA::InitCiv()
{
	if (App->entityManager->playerCreated)
	{
		//CIVILIZATION
		if (App->entityManager->getPlayer()->civilization == CivilizationType::VIKING) civilization = CivilizationType::GREEK;
		else civilization = CivilizationType::VIKING;

		
		//UNIT
		for (int i = 1; i <= 2; ++i)
		{
			std::list<Entity*>::iterator it = App->entityManager->entities[static_cast<EntityType>(i)].begin();
			for (it; it != App->entityManager->entities[static_cast<EntityType>(i)].end(); ++it)
			{
				if (it._Ptr->_Myval->civilization == civilization) {
					listEntities.push_back(it._Ptr->_Myval);
				}
			}
		}

		early = EarlyGameBehaviour::BASIC_RESEARCH;
	}

	return true;
}

bool IA::ResearchFirstLv()
{
	return true;

}

bool IA::CreateReligiousBuilds()
{
	return true;

}

bool IA::CreateUnit(std::string, int)
{
	return true;

}

bool IA::WantToResearch(std::string)
{
	return true;
}

bool IA::InitExplore()
{
	return true;

}

bool IA::CheckExplore()
{
	return false;
}

bool IA::Assemble()
{
	return false;
}

bool IA::CreateMonastery(int)
{
	return true;

}

bool IA::Defense()
{
	return true;

}

bool IA::Atack()
{
	return true;

}

bool IA::Win()
{
	return true;

}


