#ifndef __TUTORIALMESSAGES_H__
#define __TUTORIALMESSAGES_H__

#include "SDL/include/SDL.h"
#include "PugiXml\src\pugixml.hpp"
#include"p2Point.h"

#define TOTAL_TUTORIAL_MESSAGES 13

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
	levelupaunit
};

struct Tutorial_Message {
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
	Tutorial_Message GetTutorialMessage(int);

	//Called to get a line of a tooltip
	std::string GetLineTutorialMessage(int, Tutorial_Message);

private:
	Tutorial_Message tutorial_messages[TOTAL_TUTORIAL_MESSAGES];
};

#endif // !__TUTORIALMESSAGES_H__
