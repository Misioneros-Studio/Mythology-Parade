#include "TooltipData.h"
#include "p2Log.h"
#include "p2Defs.h"



TooltipData::TooltipData()
{
	GetTooltipData();
}

TooltipData::~TooltipData()
{
}

void TooltipData::GetTooltipData()
{
	for (int i = 0; i < TOTAL_TOOLTIPS; i++) {
		tooltips[i].line1 = "";
		tooltips[i].line2 = "";
		tooltips[i].line3 = "";
		tooltips[i].line4 = "";
		tooltips[i].line5 = "";
		tooltips[i].line6 = "";
		tooltips[i].line7 = "";
		tooltips[i].line8 = "";
		tooltips[i].line9 = "";
		tooltips[i].line10 = "";
		tooltips[i].line11 = "";
		tooltips[i].line12 = "";
		tooltips[i].lines = 0;
		tooltips[i].has_title = false;
	}

	pugi::xml_document	tooltip_file;
	pugi::xml_node		tooltip;
	LOG("FIRST PHASE CORRECT");

	pugi::xml_parse_result result = tooltip_file.load_file("dialog.xml");

	LOG("SECOND PHASE CORRECT");

	if (result == NULL)
		LOG("Could not load xml file dialog.xml. pugi error: %s", result.description());
	else
		LOG("Dialog xml loaded correctly");

	tooltip = tooltip_file.child("dialog");

	if (tooltip.empty() == false)
	{
		int i = 0;
		for (pugi::xml_node_iterator it = tooltip.begin(); it != tooltip.end(); it++) {
			if (i < TOTAL_TOOLTIPS) {
				pugi::xml_node node = it->first_child();
				tooltips[i].title = it->child("title").child_value();
				if (tooltips[i].title != "") {
					tooltips[i].has_title = true;
					tooltips[i].lines++;
				}
				tooltips[i].line1 = it->child("line1").child_value();
				if (tooltips[i].line1 != "")
					tooltips[i].lines++;
				tooltips[i].line2 = it->child("line2").child_value();
				if (tooltips[i].line2 != "")
					tooltips[i].lines++;
				tooltips[i].line3 = it->child("line3").child_value();
				if (tooltips[i].line3 != "")
					tooltips[i].lines++;
				tooltips[i].line4 = it->child("line4").child_value();
				if (tooltips[i].line4 != "")
					tooltips[i].lines++;
				tooltips[i].line5 = it->child("line5").child_value();
				if (tooltips[i].line5 != "")
					tooltips[i].lines++;
				tooltips[i].line6 = it->child("line6").child_value();
				if (tooltips[i].line6 != "")
					tooltips[i].lines++;
				tooltips[i].line7 = it->child("line7").child_value();
				if (tooltips[i].line7 != "")
					tooltips[i].lines++;
				tooltips[i].line8 = it->child("line8").child_value();
				if (tooltips[i].line8 != "")
					tooltips[i].lines++;
				tooltips[i].line9 = it->child("line9").child_value();
				if (tooltips[i].line9 != "")
					tooltips[i].lines++;
				tooltips[i].line10 = it->child("line10").child_value();
				if (tooltips[i].line10 != "")
					tooltips[i].lines++;
				tooltips[i].line11 = it->child("line11").child_value();
				if (tooltips[i].line11 != "")
					tooltips[i].lines++;
				tooltips[i].line12 = it->child("line11").child_value();
				if (tooltips[i].line12 != "")
					tooltips[i].lines++;
				i++;
			}
		}
	}
}

Tooltip TooltipData::GetTooltip(int i)
{
	if (i >= 0 && i < TOTAL_TOOLTIPS)
		return tooltips[i];
	else
		return tooltips[0];
}

std::string TooltipData::GetLineTooltip(int i, Tooltip tooltip)
{
	switch (i) {
	case 1:
		return tooltip.line1;
		break;
	case 2:
		return tooltip.line2;
		break;
	case 3:
		return tooltip.line3;
		break;
	case 4:
		return tooltip.line4;
		break;
	case 5:
		return tooltip.line5;
		break;
	case 6:
		return tooltip.line6;
		break;
	case 7:
		return tooltip.line7;
		break;
	case 8:
		return tooltip.line8;
		break;
	case 9:
		return tooltip.line9;
		break;
	case 10:
		return tooltip.line10;
		break;
	case 11:
		return tooltip.line11;
		break;
	case 12:
		return tooltip.line2;
		break;
	}
	return tooltip.title;
}
