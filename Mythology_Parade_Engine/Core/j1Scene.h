#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "FoWManager.h"

struct SDL_Rect;



struct SDL_Texture;
class ImageUI;
class WindowUI;
class ButtonUI;
class TextUI;
class QuadTree;
class Entity;
class Building;
enum class UnitType;
enum BuildingType;
enum CivilizationType;
class HUD;
class ResearchMenu;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	void ClickToPath();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called when restarting the game
	void RestartGame();

	// Called when returning to main menu (either winning/losing or by menu options like exit)
	void BackToTitleMenu();

	// Called to return faith after canceling a production
	void ReturnFaith(std::string);

	void OnClick(UI* element, float argument = 0);

	void FinishResearching(std::string);


public:
	SDL_Rect mapLimitsRect;
	SDL_Texture* debugBlue_tex;
	SDL_Texture* debugRed_tex;
	bool paused_game;
	bool godMode;
	bool clickToPath;


	HUD* hud;
	ResearchMenu* research_menu;
	int WinViking_sound;
	int WinGreek_sound;
	int Lose_sound;

	//Ramos
	int OpenPauseMenu_sfx;
	int Research_sound;
	int ResearchFinished;
	int Select_sfx;

	//Arnau
	bool update_selection;
	bool dont_update_types_of_troops;
	bool update_production_list;

	bool isInTutorial;

public:
	bool nextUnit_selected;
	bool nextBuilding_selected;
	bool building_meteor;

};

#endif // __j1SCENE_H__
