#include "MiraclesDisasters.h"
#include "CurrencySystem.h"

MiraclesDisasters::MiraclesDisasters()
{
}

MiraclesDisasters::~MiraclesDisasters()
{
}

void MiraclesDisasters::Miracle(Miracles action, CurrencySystem* currency)
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

void MiraclesDisasters::Disaster(Disasters action, CurrencySystem* currency)
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
