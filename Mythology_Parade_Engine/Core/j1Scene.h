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

	void OnClick(UI* element, float argument = 0);

	void FinishResearching(std::string);

	void DoWinOrLoseWindow(int type, bool win);
	fPoint DoTransitionWinLose(int pos_x, int pos_y, SDL_Texture* tex, j1Timer time);
	float LerpValue(float percent, float start, float end);

private:
	
	fPoint global_pos;

	SDL_Texture* winlose_tex;


public:
	SDL_Rect mapLimitsRect;
	SDL_Texture* debugBlue_tex;
	SDL_Texture* debugRed_tex;
	bool paused_game;
	bool godMode;
	bool clickToPath;
	bool first_time_timer_win = false;
	j1Timer animation_win_lose_timer;

	HUD* hud;
	ResearchMenu* research_menu;
	uint WinViking_sound;
	uint WinGreek_sound;
	uint Lose_sound;

	bool isInTutorial;

public:
	bool nextUnit_selected;
	bool nextBuilding_selected;
};

#endif // __j1SCENE_H__