#include "j1CurrencySystem.h"

j1CurrencySystem::j1CurrencySystem()
{
	faith = 0;
	sacrifices = 0;
	prayers = 0;
}

j1CurrencySystem::~j1CurrencySystem()
{
}

void j1CurrencySystem::increaseFaith(int number)
{
	faith += number;
}

void j1CurrencySystem::increaseSacrifice(actionSacrifice action)
{
	switch (action)
	{
	case actionSacrifice::RivalKilled:
		sacrifices++;
		prayers--;
		break;
	case actionSacrifice::EliteKilled:
		sacrifices += 2;
		prayers -= 2;
		break;
	case actionSacrifice::BeastKilled:
		sacrifices += 2;
		prayers -= 2;
		break;
	case actionSacrifice::TempleDestroyed:
		sacrifices++;
		prayers--;
		break;
	case actionSacrifice::EncampmentDestroyed:
		sacrifices += 2;
		prayers -= 2;
		break;
	case actionSacrifice::MonasteriesSacrifice:
		sacrifices++;
		break;
	default:
		break;
	}
}

void j1CurrencySystem::increasePrayers(actionPrayers action)
{
	switch (action)
	{
	case actionPrayers::UnitConverted:
		prayers += 2;
		sacrifices -= 1;
		break;
	case actionPrayers::TempleConverted:
		prayers += 1;
		sacrifices -= 2;
		break;
	case actionPrayers::EncampmentConverted:
		prayers += 2;
		sacrifices -= 2;
		break;
	case actionPrayers::MonasteryConverted:
		prayers += 1;
		sacrifices -= 2;
		break;
	case actionPrayers::BeastConverted:
		prayers += 2;
		sacrifices -= 2;
		break;
	case actionPrayers::MonasteriesPrayers:
		prayers += 1;
		break;
	default:
		break;
	}
}
