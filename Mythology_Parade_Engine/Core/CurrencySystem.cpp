#include "CurrencySystem.h"
#include "j1App.h"
#include "j1Scene.h"

CurrencySystem::CurrencySystem()
{
	faith = 0;
	sacrifices = 0;
	prayers = 0;

	time_prayers = time_sacrifices = 10;

	basefaithRatio = 2;
	faithRatio = basefaithRatio;

}

CurrencySystem::~CurrencySystem()
{
}

void CurrencySystem::IncreaseFaith()
{
	if(!App->scene->paused_game)
	{
		if(faithRatio>0)
		{
			faith += faithRatio;
		}
	}
}

void CurrencySystem::DecreaseFaith(int number)
{
	if (!App->scene->paused_game)
	{
		if (number > 0)
		{
			faith -= number;
		}
	}
}

void CurrencySystem::IncreaseSacrifice(ActionSacrifice action)
{
	if (App->scene->paused_game)
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
}

void CurrencySystem::IncreasePrayers(ActionPrayers action)
{
	if (App->scene->paused_game)
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
}
void CurrencySystem::IncreaseAll(int number)
{
	if (number > 0)
	{
		faith += number;
		sacrifices += number;
		prayers += number;
	}
}
void CurrencySystem::IncreaseFaithRatio(int number)
{
	faithRatio = basefaithRatio + number;
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
