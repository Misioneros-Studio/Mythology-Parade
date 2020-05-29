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
	positionViking.push_back({ -224,624 }); //Monk1
	positionViking.push_back({ -384,736 }); //Monk2
	positionViking.push_back({ -352,752 }); //Monk3
	positionViking.push_back({ -3904,2464 }); //Cleric1Pos
	positionViking.push_back({ 3200,2304 }); //Cleric2Pos
	positionViking.push_back({ -192,4448 }); //Cleric3Pos
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
				MoveUnity(positionViking.at((int)EarlyMovements::MONK1), "monk");
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::MONK, positionViking.at((int)EarlyMovements::MONK2))));
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::MONK, positionViking.at((int)EarlyMovements::MONK3))));
			}
			else
			{
				MoveUnity(positionGreek.at((int)EarlyMovements::MONK1), "monk");
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::MONK, positionGreek.at((int)EarlyMovements::MONK2))));
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::MONK, positionGreek.at((int)EarlyMovements::MONK3))));
			}
			early = EarlyGameBehaviour::BASIC_UNITS_CREATION;
			timer.Start();
		}
		break;
	case EarlyGameBehaviour::BASIC_UNITS_CREATION:
		if (timer.ReadSec() >= 3)//150
		{
			if (civilization == CivilizationType::VIKING)
			{
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::CLERIC, positionViking.at((int)EarlyMovements::CLERIC2))));
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::CLERIC, positionViking.at((int)EarlyMovements::CLERIC1))));
			}
			else
			{
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::CLERIC, positionGreek.at((int)EarlyMovements::CLERIC2))));
				listEntities.push_back(static_cast<Entity*>(CreateUnit(UnitType::CLERIC, positionGreek.at((int)EarlyMovements::CLERIC1))));
			}
			early = EarlyGameBehaviour::EXPLORE1;
			timer.Start();
		}
		break;
	case EarlyGameBehaviour::EXPLORE1:
		Explore1();
		early = EarlyGameBehaviour::CHECKEXPLORER1;
		break;
	case EarlyGameBehaviour::CHECKEXPLORER1:
		if (CheckExplore())
		{
			Explore2();
			early = EarlyGameBehaviour::FIND;
		}
		break;
	case EarlyGameBehaviour::FIND:
		if (Find())
			gamePhase = GameBehaviour::MID;
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
		AssembleClerics();
		mid = MidGameBehaviour::CREATE_ECONOMY;
		break;
	case MidGameBehaviour::CREATE_ECONOMY:
		break;
	case MidGameBehaviour::RESEARCH_ASSASSIN:
		if(civilization==CivilizationType::VIKING)
			//CreateBuilding(BuildingType::ENCAMPMENT, );
		//else
			//CreateBuilding(BuildingType::ENCAMPMENT, );

		mid = MidGameBehaviour::CREATE_ASSASSIN;
		break;
	case MidGameBehaviour::CREATE_ASSASSIN:
		if (civilization == CivilizationType::VIKING) {
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
		}
		else {
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
			//CreateUnit(UnitType::ASSASSIN, );
		}
		mid = MidGameBehaviour::DISTRIBUTION;
		timer.Start();
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

		for (int i = 1; i <= 3; ++i)
		{
			std::list<Entity*>::iterator it = App->entityManager->entities[static_cast<EntityType>(i)].begin();
			for (it; it != App->entityManager->entities[static_cast<EntityType>(i)].end(); ++it)
			{
				if (it._Ptr->_Myval->civilization == civilization) {
					listEntities.push_back(it._Ptr->_Myval);
				}
				if (it._Ptr->_Myval->name == "fortress" && it._Ptr->_Myval->civilization != civilization)
				{
					enemyFortress = static_cast<Building*>(it._Ptr->_Myval);
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


Unit* IA::CreateUnit(UnitType type, iPoint pos)
{
	Entity* entity = App->entityManager->CreateUnitEntity(type, pos, civilization);

	return static_cast<Unit*>(entity);
}

bool IA::CheckExplore()
{
	bool ret = false;

	int units = 0;

	int i = 0;
	std::list<Entity*>::iterator it = listEntities.begin();
	for (it; it != listEntities.end(); ++it)
	{
		if (it._Ptr->_Myval->name == "cleric")
		{
			if (it._Ptr->_Myval->position == positionViking.at((int)EarlyMovements::CLERIC1POS + i))
			{
				units++;
			}
			i++;
		}
	}
	if (units == 2) {
		ret = true;
	}

	return ret;
}

bool IA::Find()
{
	bool ret = false;

	std::list<Entity*>::iterator it = listEntities.begin();
	for (it; it != listEntities.end(); ++it)
	{
		if (it._Ptr->_Myval->name == "cleric")
		{
			if (it._Ptr->_Myval->position.DistanceManhattan(enemyFortress->position)<=700)
			{
				ret = true;
			}
		}
	}

	return ret;
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

bool IA::MoveUnity(iPoint pos, std::string name, Unit* u)
{
	std::list<Entity*>::iterator it = listEntities.begin();
	std::list<Entity*> entities;
	if (u == nullptr)
	{
		for (it; it != listEntities.end(); ++it)
		{
			if (it._Ptr->_Myval->name == name)
			{
				iPoint origin = App->map->WorldToMap((int)it._Ptr->_Myval->position.x, (int)it._Ptr->_Myval->position.y);
				iPoint ending = App->map->WorldToMap(pos.x, pos.y);
				entities.push_back(it._Ptr->_Myval);
				App->pathfinding->RequestPath(origin, ending, entities);
				break;
			}
			entities.clear();
		}
	}
	else
	{
		for (it; it != listEntities.end(); ++it)
		{
			if (it._Ptr->_Myval == u)
			{
				iPoint origin = App->map->WorldToMap((int)it._Ptr->_Myval->position.x, (int)it._Ptr->_Myval->position.y);
				iPoint ending = App->map->WorldToMap(pos.x, pos.y);
				entities.push_back(it._Ptr->_Myval);
				App->pathfinding->RequestPath(origin, ending, entities);
				break;
			}
			entities.clear();
		}
	}
	
	return true;
}

void IA::Explore1()
{
	int i = 0;
	std::list<Entity*>::iterator it = listEntities.begin();
	for (it; it != listEntities.end(); ++it)
	{
		if (it._Ptr->_Myval->name == "cleric")
		{
			Unit* u = static_cast<Unit*>(it._Ptr->_Myval);
			if (civilization == CivilizationType::VIKING)
				MoveUnity(positionViking.at((int)EarlyMovements::CLERIC1POS + i), "cleric", u);
			else
				MoveUnity(positionGreek.at((int)EarlyMovements::CLERIC1POS + i), "cleric", u);
			i++;
		}
	}
}

void IA::Explore2()
{
	std::list<Entity*>::iterator it = listEntities.begin();
	for (it; it != listEntities.end(); ++it)
	{
		if (it._Ptr->_Myval->name == "cleric")
		{
			Unit* u = static_cast<Unit*>(it._Ptr->_Myval);
			if (civilization == CivilizationType::VIKING)
				MoveUnity(positionViking.at((int)EarlyMovements::CLERIC3POS), "cleric", u);
			else
				MoveUnity(positionGreek.at((int)EarlyMovements::CLERIC3POS), "cleric", u);
		}
	}
}

void IA::AssembleClerics()
{
	std::list<Entity*>::iterator it = listEntities.begin();
	for (it; it != listEntities.end(); ++it)
	{
		if (it._Ptr->_Myval->name == "cleric")
		{
			Unit* u = static_cast<Unit*>(it._Ptr->_Myval);
			if (civilization == CivilizationType::VIKING)
				MoveUnity(positionViking.at((int)EarlyMovements::HOME), "cleric", u);
			else
				MoveUnity(positionGreek.at((int)EarlyMovements::HOME), "cleric", u);
		}
	}
}


