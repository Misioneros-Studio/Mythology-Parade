#include "CurrencySystem.h"

CurrencySystem::CurrencySystem()
{
	faith = 0;
	sacrifices = 0;
	prayers = 0;
}

CurrencySystem::~CurrencySystem()
{
}

void CurrencySystem::increaseFaith(int number)
{
	if(number>0)
	{
		faith += number;
	}
}

void CurrencySystem::decreaseFaith(int number)
{
	if (number > 0)
	{
		faith -= number;
	}
}

void CurrencySystem::increaseSacrifice(ActionSacrifice action)
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

void CurrencySystem::increasePrayers(ActionPrayers action)
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
void CurrencySystem::Miracle(Miracles action)
{
	switch (action)
	{
	case Miracles::ClericEnlightment:
		if (prayers >= 30) {
			prayers += 3;
			sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::BordersPressure:
		if (prayers >= 30) {
			prayers += 2;
			sacrifices -= 2;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::BeastTame:
		if (prayers >= 60) {
			prayers += 1;
			sacrifices -= 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::MassConversion:
		if (prayers >= 60) {
			prayers += 3;
			sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::ForcedTraition:
		if (prayers >= 60) {
			prayers += 3;
			sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::WeakenedBorders:
		if (prayers >= 60) {
			prayers += 1;
			sacrifices -= 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::CallToArms:
		if (prayers >= 60) {
			prayers += 3;
			sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::GoldenExpansion:
		if (prayers >= 60) {
			prayers += 3;
			sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::Victory:
		if (prayers >= 60) {
			//FUNCTION OF THE EFFECT
		}
		break;
	default:
		break;
	}
}

void CurrencySystem::Disaster(Disasters action)
{
	switch (action)
	{
	case Disasters::MilitaryPressure:
		if (sacrifices >= 30) {
			prayers -= 3;
			sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::TroopsInspiration:
		if (sacrifices >= 30) {
			prayers -= 2;
			sacrifices += 2;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::HolyMeteor:
		if (sacrifices >= 60) {
			prayers -= 3;
			sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::StrikingFear:
		if (sacrifices >= 60) {
			prayers -= 1;
			sacrifices += 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::BloodFlow:
		if (sacrifices >= 60) {
			prayers -= 3;
			sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::TheresRoom:
		if (sacrifices >= 60) {
			prayers -= 1;
			sacrifices += 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::ColdAssassination:
		if (sacrifices >= 60) {
			prayers -= 3;
			sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::AnimalShelter:
		if (sacrifices >= 60) {
			prayers -= 3;
			sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::Victory:
		if (sacrifices >= 60) {
			//FUNCTION OF THE EFFECT
		}
		break;
	default:
		break;
	}
}