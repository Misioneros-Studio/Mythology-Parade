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
	faith += number;
}

void CurrencySystem::decreaseFaith(int number)
{
	faith -= number;
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
void CurrencySystem::Miracle(Miracles action, CurrencySystem* currency)
{
	switch (action)
	{
	case Miracles::ClericEnlightment:
		if (currency->prayers >= 30) {
			currency->prayers += 3;
			currency->sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::BordersPressure:
		if (currency->prayers >= 30) {
			currency->prayers += 2;
			currency->sacrifices -= 2;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::BeastTame:
		if (currency->prayers >= 60) {
			currency->prayers += 1;
			currency->sacrifices -= 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::MassConversion:
		if (currency->prayers >= 60) {
			currency->prayers += 3;
			currency->sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::ForcedTraition:
		if (currency->prayers >= 60) {
			currency->prayers += 3;
			currency->sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::WeakenedBorders:
		if (currency->prayers >= 60) {
			currency->prayers += 1;
			currency->sacrifices -= 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::CallToArms:
		if (currency->prayers >= 60) {
			currency->prayers += 3;
			currency->sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::GoldenExpansion:
		if (currency->prayers >= 60) {
			currency->prayers += 3;
			currency->sacrifices -= 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Miracles::Victory:
		if (currency->prayers >= 60) {
			//FUNCTION OF THE EFFECT
		}
		break;
	default:
		break;
	}
}

void CurrencySystem::Disaster(Disasters action, CurrencySystem* currency)
{
	switch (action)
	{
	case Disasters::MilitaryPressure:
		if (currency->sacrifices >= 30) {
			currency->prayers -= 3;
			currency->sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::TroopsInspiration:
		if (currency->sacrifices >= 30) {
			currency->prayers -= 2;
			currency->sacrifices += 2;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::HolyMeteor:
		if (currency->sacrifices >= 60) {
			currency->prayers -= 3;
			currency->sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::StrikingFear:
		if (currency->sacrifices >= 60) {
			currency->prayers -= 1;
			currency->sacrifices += 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::BloodFlow:
		if (currency->sacrifices >= 60) {
			currency->prayers -= 3;
			currency->sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::TheresRoom:
		if (currency->sacrifices >= 60) {
			currency->prayers -= 1;
			currency->sacrifices += 1;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::ColdAssassination:
		if (currency->sacrifices >= 60) {
			currency->prayers -= 3;
			currency->sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::AnimalShelter:
		if (currency->sacrifices >= 60) {
			currency->prayers -= 3;
			currency->sacrifices += 3;
			//FUNCTION OF THE EFFECT
		}
		break;
	case Disasters::Victory:
		if (currency->sacrifices >= 60) {
			//FUNCTION OF THE EFFECT
		}
		break;
	default:
		break;
	}
}
