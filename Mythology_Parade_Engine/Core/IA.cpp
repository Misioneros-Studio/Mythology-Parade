#include "IA.h"
#include "j1App.h"

IA::IA()
{
	gamePhase = GameBehaviour::EARLY;
	early = EarlyGameBehaviour::CREATION;
	mid = MidGameBehaviour::ASSEMBLE;
	late = LateGameBehaviour::ATACK;

	positionViking.push_back({ -384,672 }); //Monastery
	positionViking.push_back({ 224,688 }); //Temple
	positionViking.push_back({ 160,816 }); //Cleric1
	positionViking.push_back({ 224,848 }); //Cleric2
	positionViking.push_back({ 192,832 }); //Cleric3
	positionViking.push_back({ -416,720 }); //Monk1
	positionViking.push_back({ -384,736 }); //Monk2
	positionViking.push_back({ -352,752 }); //Monk3
	positionViking.push_back({ -3904,2464 }); //Cleric1Pos
	positionViking.push_back({ 3200,2304 }); //Cleric2Pos
	positionViking.push_back({ -192,4352 }); //Cleric3Pos
	positionViking.push_back({ -128,576 }); //Home

	positionGreek.push_back({ -576,4256 }); //Monastery
	positionGreek.push_back({ 64,4224 }); //Temple
	positionGreek.push_back({ -256,4416 }); //Cleric1
	positionGreek.push_back({ -192,4416 }); //Cleric2
	positionGreek.push_back({ -128,4416 }); //Cleric3
	positionGreek.push_back({ -576,4192 }); //Monk1
	positionGreek.push_back({ -640,4288 }); //Monk2
	positionGreek.push_back({ -608,4304 }); //Monk3
	positionGreek.push_back({ -3904,2464 }); //Cleric1Pos
	positionGreek.push_back({ 3200,2304 }); //Cleric2Pos
	positionGreek.push_back({ -128,576 }); //Cleric3Pos
	positionGreek.push_back({ -192,4352 }); //Home

}

bool IA::PreUpdate()
{
	bool ret = true;

	if (App->input->GetMouseButtonDown(1))
	{
		mouse = App->map->GetMousePositionOnMap();
		mouse = App->map->TileCenterPoint(mouse);
		LOG("%i %i", mouse.x, mouse.y);
	}

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
	case EarlyGameBehaviour::BASIC_BUILDINGS_CREATION:
		if (timer.ReadSec() >= 3)//45
		{
			if (civilization == CivilizationType::VIKING)
			{
				CreateBuilding(BuildingType::MONASTERY, positionViking.at((int)EarlyMovements::MONASTERY));
				CreateBuilding(BuildingType::TEMPLE, positionViking.at((int)EarlyMovements::TEMPLE));
			}
			else
			{
				CreateBuilding(BuildingType::MONASTERY, positionGreek.at((int)EarlyMovements::MONASTERY));
				CreateBuilding(BuildingType::TEMPLE, positionGreek.at((int)EarlyMovements::TEMPLE));
			}
			early = EarlyGameBehaviour::RESEARCH_CLERIC;
			timer.Start();
		}
		break;
	case EarlyGameBehaviour::RESEARCH_CLERIC:
		if (timer.ReadSec() >= 5)//50
		{
			if (civilization == CivilizationType::VIKING)
			{
				//existence monk
				CreateUnit(UnitType::MONK, positionViking.at((int)EarlyMovements::MONK2));
				CreateUnit(UnitType::MONK, positionViking.at((int)EarlyMovements::MONK3));
			}
			else
			{
				//existence monk
				CreateUnit(UnitType::MONK, positionGreek.at((int)EarlyMovements::MONK2));
				CreateUnit(UnitType::MONK, positionGreek.at((int)EarlyMovements::MONK3));
			}
		}
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
		timer.Start();
		early = EarlyGameBehaviour::BASIC_BUILDINGS_CREATION;
	}

	return true;
}

bool IA::CreateBuilding(BuildingType type, iPoint pos)
{
	if (type == BuildingType::MONASTERY)
	{
		if(civilization==CivilizationType::GREEK)
			App->entityManager->CreateBuildingEntity(pos, type, App->entityManager->buildingsData[5], civilization);
		else
			App->entityManager->CreateBuildingEntity(pos, type, App->entityManager->buildingsData[1], civilization);
	}
	else if (type == BuildingType::TEMPLE)
	{
		if (civilization == CivilizationType::GREEK)
			App->entityManager->CreateBuildingEntity(pos, type, App->entityManager->buildingsData[6], civilization);
		else
			App->entityManager->CreateBuildingEntity(pos, type, App->entityManager->buildingsData[2], civilization);
	}

	return true;
}


bool IA::CreateUnit(UnitType type, iPoint pos)
{
	if (type == UnitType::MONK)
	{
		App->entityManager->CreateUnitEntity(type, pos, civilization);
	}

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


