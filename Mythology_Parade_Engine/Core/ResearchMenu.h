#ifndef __RESEARCHMENU_H__
#define __RESEARCHMENU_H__

#include "SDL/include/SDL.h"
#include "PugiXml\src\pugixml.hpp"
#include"p2Point.h"

class ImageUI;
class WindowUI;
class ButtonUI;
class TextUI;
class Entity;
class Building;
enum class UnitType;
enum BuildingType;
enum CivilizationType;
class Player;

class ResearchMenu
{


public:

	//Constructor
	ResearchMenu(Player* player);

	//Destructor
	~ResearchMenu();

	// Called before the first frame
	void StartResearchMenu(Player* play);

	//Called when clicking the research button
	void ActivateResearchMenu();

	//Called when clicking close button in the research menu
	void DeactivateResearchMenu();

	//Return if research_window is active or not (nullptr)
	bool CheckReasearchWindow();

	//Called to create the button or image to research the temple;
	void CreateTempleButton();

	//Called to create the button or image to research the encampment;
	void CreateEncampmentButton();

	//Called to create the button or image to research the cleric;
	void CreateClericButton();

	//Called to create the button or image to research the assassin;
	void CreateAssassinButton();

	//Called to create the button or image to research the lawful beast;
	void CreateLawfulBeastButton();

	//Called to create the button or image to research the chaotic beast;
	void CreateChaoticBeastButton();

	//Called to create the button or image to research the lawful miracle;
	void CreateLawfulMiracleButton();

	//Called to create the button or image to research the chaotic miracle;
	void CreateChaoticMiracleButton();

	//Called to create the button or image to research the lawful victory;
	void CreateLawfulVictoryButton();

	//Called to create the button or image to research the chaotic victory;
	void CreateChaoticVictoryButton();


private:
	WindowUI* ui_research_window;
	ButtonUI* ui_button_research[11];
	ImageUI* ui_image_research[11];
	TextUI* ui_text_research[2];
	Player* player;
};

#endif // !__RESEARCHMENU_H__
