#ifndef _MIRACLESDISASTERS_
#define _MIRACLESDISASTERS_

#include "CurrencySystem.h"

class MiraclesDisasters 
{
public:
	enum class Miracles {
		ClericEnlightment,
		BordersPressure,
		BeastTame,
		MassConversion,
		ForcedTraition,
		WeakenedBorders,
		CallToArms,
		GoldenExpansion,
		Victory
	};
	enum class Disasters {
		MilitaryPressure,
		TroopsInspiration,
		HolyMeteor,
		StrikingFear,
		BloodFlow,
		TheresRoom,
		ColdAssassination,
		AnimalShelter,
		Victory
	};

public:
	MiraclesDisasters();
	~MiraclesDisasters();

	void Miracle(Miracles action, CurrencySystem* currency);
	void Disaster(Disasters action, CurrencySystem* currency);
};


#endif // !_MIRACLESDISASTERS_
