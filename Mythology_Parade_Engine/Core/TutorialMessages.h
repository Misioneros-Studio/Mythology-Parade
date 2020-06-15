#ifndef __TUTORIALMESSAGES_H__
#define __TUTORIALMESSAGES_H__

#include "SDL/include/SDL.h"
#include "PugiXml\src\pugixml.hpp"
#include"p2Point.h"

#define TOTAL_TUTORIAL_MESSAGES 16

enum class TutorialMessagesAvailable {
	presentationdialog,
	cameramovements,
	fogofwar,
	fortressandproduction,
	selectionsystem,
	buildabuildingandproduceaunit,
	researchtreeintroduction,
	researchanewbuildingandanewunit,
	sacrificesandprayers,
	createanewbuildingandanewunit,
	moveunits,
	attackorconvertanenemyunit,
	levelupaunit,
	tooltips,
	winquest,
	winconditions
};

struct TutorialMessage {
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
};

class TutorialMessageData
{

public:

	//Constructor
	TutorialMessageData();

	//Destructor
	~TutorialMessageData();

	// Called before the first frame
	void GetTutorialMessageData();

	// Called to get a tooltip
	TutorialMessage GetTutorialMessage(int);

	//Called to get a line of a tooltip
	std::string GetLineTutorialMessage(int, TutorialMessage);

private:
	TutorialMessage tutorial_messages[TOTAL_TUTORIAL_MESSAGES];
};

#endif // !__TUTORIALMESSAGES_H__
