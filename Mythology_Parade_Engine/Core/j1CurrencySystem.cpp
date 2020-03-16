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

void j1CurrencySystem::decreaseFaith(int number)
{
	faith -= number;
}

void j1CurrencySystem::increaseSacrifice(ActionSacrifice action)
{
	switch (action)
	{
	case ActionSacrifice::RivalKilled:
		sacrifices++;
		prayers--;
		break;
	case ActionSacrifice::EliteKilled:
		sacrifices += 2;
		prayers -= 2;
		break;
	case ActionSacrifice::BeastKilled:
		sacrifices += 2;
		prayers -= 2;
		break;
	case ActionSacrifice::TempleDestroyed:
		sacrifices++;
		prayers--;
		break;
	case ActionSacrifice::EncampmentDestroyed:
		sacrifices += 2;
		prayers -= 2;
		break;
	case ActionSacrifice::MonasteriesSacrifice:
		sacrifices++;
		break;
	default:
		break;
	}
}

void j1CurrencySystem::increasePrayers(ActionPrayers action)
{
	switch (action)
	{
	case ActionPrayers::UnitConverted:
		prayers += 2;
		sacrifices -= 1;
		break;
	case ActionPrayers::TempleConverted:
		prayers += 1;
		sacrifices -= 2;
		break;
	case ActionPrayers::EncampmentConverted:
		prayers += 2;
		sacrifices -= 2;
		break;
	case ActionPrayers::MonasteryConverted:
		prayers += 1;
		sacrifices -= 2;
		break;
	case ActionPrayers::BeastConverted:
		prayers += 2;
		sacrifices -= 2;
		break;
	case ActionPrayers::MonasteriesPrayers:
		prayers += 1;
		break;
	default:
		break;
	}
}
