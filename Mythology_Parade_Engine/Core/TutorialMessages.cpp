#include "TutorialMessages.h"
#include "p2Log.h"
#include "p2Defs.h"



TutorialMessageData::TutorialMessageData()
{
	GetTutorialMessageData();
}

TutorialMessageData::~TutorialMessageData()
{
}

void TutorialMessageData::GetTutorialMessageData()
{
	for (int i = 0; i < TOTAL_TUTORIAL_MESSAGES; i++) {
		tutorial_messages[i].line1 = "";
		tutorial_messages[i].line2 = "";
		tutorial_messages[i].line3 = "";
		tutorial_messages[i].line4 = "";
		tutorial_messages[i].line5 = "";
		tutorial_messages[i].line6 = "";
		tutorial_messages[i].line7 = "";
		tutorial_messages[i].line8 = "";
		tutorial_messages[i].lines = 0;
		tutorial_messages[i].has_title = false;
	}

	pugi::xml_document	tutorial_message_file;
	pugi::xml_node		tutorial_message;

	pugi::xml_parse_result result = tutorial_message_file.load_file("TutorialDialog.xml");


	if (result == NULL)
		LOG("Could not load xml file dialog.xml. pugi error: %s", result.description());
	else
		LOG("Dialog xml loaded correctly");

	tutorial_message = tutorial_message_file.child("tutorialdialog");

	if (tutorial_message.empty() == false)
	{
		int i = 0;
		for (pugi::xml_node_iterator it = tutorial_message.begin(); it != tutorial_message.end(); it++) {
			if (i < TOTAL_TUTORIAL_MESSAGES) {
				pugi::xml_node node = it->first_child();
				tutorial_messages[i].title = it->child("title").child_value();
				if (tutorial_messages[i].title != "") {
					tutorial_messages[i].has_title = true;
					tutorial_messages[i].lines++;
				}
				tutorial_messages[i].line1 = it->child("line1").child_value();
				if (tutorial_messages[i].line1 != "")
					tutorial_messages[i].lines++;
				tutorial_messages[i].line2 = it->child("line2").child_value();
				if (tutorial_messages[i].line2 != "")
					tutorial_messages[i].lines++;
				tutorial_messages[i].line3 = it->child("line3").child_value();
				if (tutorial_messages[i].line3 != "")
					tutorial_messages[i].lines++;
				tutorial_messages[i].line4 = it->child("line4").child_value();
				if (tutorial_messages[i].line4 != "")
					tutorial_messages[i].lines++;
				tutorial_messages[i].line5 = it->child("line5").child_value();
				if (tutorial_messages[i].line5 != "")
					tutorial_messages[i].lines++;
				tutorial_messages[i].line6 = it->child("line6").child_value();
				if (tutorial_messages[i].line6 != "")
					tutorial_messages[i].lines++;
				tutorial_messages[i].line7 = it->child("line7").child_value();
				if (tutorial_messages[i].line7 != "")
					tutorial_messages[i].lines++;
				tutorial_messages[i].line8 = it->child("line8").child_value();
				if (tutorial_messages[i].line8 != "")
					tutorial_messages[i].lines++;
				i++;
			}
		}
	}
}

TutorialMessage TutorialMessageData::GetTutorialMessage(int i)
{
	if (i >= 0 && i < TOTAL_TUTORIAL_MESSAGES)
		return tutorial_messages[i];
	else
		return tutorial_messages[0];
}

std::string TutorialMessageData::GetLineTutorialMessage(int i, TutorialMessage tutorial_message)
{
	switch (i) {
	case 1:
		return tutorial_message.line1;
		break;
	case 2:
		return tutorial_message.line2;
		break;
	case 3:
		return tutorial_message.line3;
		break;
	case 4:
		return tutorial_message.line4;
		break;
	case 5:
		return tutorial_message.line5;
		break;
	case 6:
		return tutorial_message.line6;
		break;
	case 7:
		return tutorial_message.line7;
		break;
	case 8:
		return tutorial_message.line8;
		break;
	}
	return tutorial_message.title;
}
