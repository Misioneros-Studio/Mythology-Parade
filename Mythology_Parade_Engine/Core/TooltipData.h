#ifndef __TOOLTIPDATA_H__
#define __TOOLTIPDATA_H__

#include "SDL/include/SDL.h"
#include "PugiXml\src\pugixml.hpp"
#include"p2Point.h"

#define TOTAL_TOOLTIPS 32

enum class TooltipsAvailable {
	temple,
	encampment,
	cleric_locked,
	cleric_unlocked,
	assassin_locked,
	assassin_unlocked,
	cyclop_locked,
	cyclop_unlocked,
	jotnar_locked,
	jotnar_unlocked,
	minotaur,
	draugar,
	miracle_locked,
	miracle_unlocked,
	disaster_locked,
	disaster_unlocked,
	victory1_locked,
	victory1_unlocked,
	victory2_locked,
	victory2_unlocked,
	currencymoduletip,
	researchbutton,
	movebutton,
	actionbutton,
	levelupbutton_assassin,
	levelupbutton_cleric,
	unitcreationbutton,
	producecurrencybutton,
	buildingconstructionlocked,
	unitproductionlocked,
	miraclelocked,
	disasterlocked
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
