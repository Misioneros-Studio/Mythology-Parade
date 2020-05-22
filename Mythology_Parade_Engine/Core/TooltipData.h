#ifndef __TOOLTIPDATA_H__
#define __TOOLTIPDATA_H__

#include "SDL/include/SDL.h"
#include "PugiXml\src\pugixml.hpp"
#include"p2Point.h"

#define TOTAL_TOOLTIPS 49

enum class TooltipsAvailable {
	monastery,
	monk,
	temple_unresearched,
	temple,
	encampment_unresearched,
	encampment,
	cleric_locked,
	cleric_unresearched,
	cleric_unlocked,
	assassin_locked,
	assassin_unresearched,
	assassin_unlocked,
	cyclop_locked,
	cyclop_unresearched,
	cyclop_unlocked,
	jotnar_locked,
	jotnar_unresearched,
	jotnar_unlocked,
	minotaur_locked,
	minotaur_unresearched,
	minotaur_unlocked,
	draugar_locked,
	draugar_unresearched,
	draugar_unlocked,
	disaster_locked,
	disaster_unresearched,
	disaster_unlocked,
	miracle_locked,
	miracle_unresearched,
	miracle_unlocked,
	victory1_locked,
	victory1_unresearched,
	victory1_unlocked,
	victory2_locked,
	victory2_unresearched,
	victory2_unlocked,
	currencymoduletip,
	researchbutton,
	movebutton,
	attackbutton,
	healbutton,
	levelupbutton_assassin,
	levelupbutton_cleric,
	producecurrencybutton,
	buildingconstructionlocked,
	unitproductionlocked,
	miraclelocked,
	disasterlocked,
	victorylocked
};

struct Tooltip {
	int lines;
	bool has_title;
	std::string title;
	std::string line1;
	std::string line2;
	std::string line3;
	std::string line4;
	std::string line5;
	std::string line6;
	std::string line7;
	std::string line8;
	std::string line9;
	std::string line10;
	std::string line11;
	std::string line12;
};

class TooltipData
{

public:

	//Constructor
	TooltipData();

	//Destructor
	~TooltipData();

	// Called before the first frame
	void GetTooltipData();

	// Called to get a tooltip
	Tooltip GetTooltip(int);

	//Called to get a line of a tooltip
	std::string GetLineTooltip(int, Tooltip);

private:
	Tooltip tooltips[TOTAL_TOOLTIPS];
};

#endif // !__TOOLTIPDATA_H__
