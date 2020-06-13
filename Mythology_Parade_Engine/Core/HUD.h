#ifndef __HUD_H__
#define __HUD_H__

#include "j1Timer.h"
#include "SDL/include/SDL.h"
#include "PugiXml\src\pugixml.hpp"
#include"p2Point.h"

enum class CloseSceneMenus {
	None,
	Pause,
	Options,
	Confirmation,
	Confirmation_and_Pause,
	Research,
	Unknown
};

class ImageUI;
class WindowUI;
class ButtonUI;
class TextUI;
class Entity;
class Building;
enum class UnitType;
enum BuildingType;
enum CivilizationType;
class ResearchMenu;

class HUD
{


public:

	//Constructor
	HUD(ResearchMenu* r);

	//Destructor
	~HUD();

	// Called before the first frame
	void StartHUD(ResearchMenu* r);

	// Called when clicking esc
	void ActivatePauseMenu();

	// Called when clicking close button in pause menu
	void DeactivatePauseMenu();

	// Called when clicking options button in pause menu
	void ActivateOptionsMenu();

	// Called when clicking close button in options menu
	void DeactivateOptionsMenu();

	// Called when clicking a button in the menu with confirmation message
	void ActivateConfirmationMenu(std::string str);

	// Called when clicking no in the confirmation message
	void DeactivateConfirmationMenu();

	// Called when selecting troops or buildings
	void HUDUpdateSelection(std::list<Entity*>, Building*, bool update_list = true);

	// Called to order the list of selected
	std::list<Entity*> OrderSelectedList(std::list<Entity*>, int);

	//Called when scrolling down
	bool HUDScrollDown();

	//Called when scrolling up
	bool HUDScrollUp();

	// Called when deleting the list of troops in the HUD
	void HUDDeleteListTroops(bool delete_everything = true);

	// Called when deleting the selected troop in the HUD
	void HUDDeleteSelectedTroop();

	// Called when deleting the selected troop's action buttons in the HUD
	void HUDDeleteActionButtons();

	// Called to update every frame the information of the selected thing
	void UpdateSelectedThing();

	// Called when clicking a thing in the production queue of a building
	void CancelProduction(iPoint);

	// Called when clicked on one of the buttons of the right after selecting troops
	void ClickOnSelectionButton(SDL_Rect);

	// Called when changing the type of unit appearing in the central part of the HUD
	void ChangeTypeOfUnitFirstSelected(int, int);

	// Called when deleting a type of troop in the selected list
	void DeleteTroopsInSelectedList(int, int);

	//Called when creating or updating the action buttons
	void ManageActionButtons(bool create_buttons = false, bool viking = true);

	// Called to get the rect of the sprite of the portrait
	SDL_Rect GetSpritePortrait(int type_of_portrait, UnitType unit_type);

	// Called to get the unit type from a sprite portrait
	UnitType GetUnitTypeFromSpritePortrait(SDL_Rect);

	// Called to get the rect of the sprite of the portrait of the building
	SDL_Rect GetSpritePortraitBuilding(int type_of_portrait, BuildingType building_type, CivilizationType civilization);

	// Called to get the rect of the sprite of the portrait of the thing produced
	SDL_Rect GetSpritePortraitProduction(int type_of_portrait, const std::string &produced_type, CivilizationType civilization);

	//Called when clicking the research button
	void ActivateResearchMenu();

	//Called when clicking close button in the research menu
	void DeactivateResearchMenu();

	//Called when pausing the game
	void PauseGame();

	//Called when resuming the game
	void ResumeGame();

	//Called when updating the scene to change the width of the sliders
	void UpdateSlider(int index);

	//Called when updating the IA bar every frame
	void UpdateIABar(float percentage);

private:
	enum class Type_Selected {
		None,
		Assassin,
		Pikeman,
		Monk,
		Cleric,
		Minotaur,
		Cyclop,
		Jotnar,
		Draugar,
		Fortress,
		Temple,
		Encampment,
		Monastery,
		Unknown
	};

	WindowUI* ui_pause_window;
	ButtonUI* ui_button[7];
	TextUI* ui_text[8];
	WindowUI* ui_options_window;
	TextUI* ui_text_options[3];
	WindowUI* ui_winlose_window;
	ButtonUI* ui_button_winlose[2];
	TextUI* ui_text_winlose[4];
	WindowUI* ui_confirmation_window;
	ButtonUI* ui_button_confirmation[2];
	TextUI* ui_text_confirmation[4];
	ImageUI* hud_bar_producing[2];
	ButtonUI* hud_list_troops[12];
	TextUI* hud_number_troops[12];
	int number_of_troops[12];
	UnitType type_of_troops[12];
	ImageUI* hud_selected_troop;
	TextUI* hud_stats_selected_troop[13];
	Type_Selected type_thing_selected;
	ButtonUI* hud_button_actions[9];
	ImageUI* hud_button_actions_unclickable[9];
	ImageUI* ui_pause_black_screen[2];
	TextUI* ui_text_volume_sliders[2];
	ResearchMenu* research_menu;
	int damage;
	int range;
	int speed;
	int level;
	int experience;
	int health;
	int influence;
	int max_cap;


public:
	ImageUI* ui_ingame;
	TextUI* ui_text_ingame[3];
	CloseSceneMenus close_menus;
	std::string confirmation_option;
	ButtonUI* ui_button_options[2];
	Entity* thing_selected;
	j1Timer timer_win_lose;
	bool start_timer;
	ImageUI* ui_volume_sliders[6];
	ImageUI* ia_bar_back;
	ImageUI* ia_bar_front;
};

#endif // !__HUD__H_
