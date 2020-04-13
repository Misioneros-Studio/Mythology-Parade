#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"

enum class CloseSceneMenus {
	None,
	Pause,
	Options,
	Confirmation,
	Confirmation_and_Pause,
	Unknown
};

struct SDL_Texture;
class ImageUI;
class WindowUI;
class ButtonUI;
class TextUI;
class QuadTree;


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

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

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

	// Called when returning to main menu (either winning/losing or by menu options like exit)
	void BackToTitleMenu();

	// Called when restarting the game
	void RestartGame();


	void OnClick(UI* element, float argument = 0);

	void DoWinOrLoseWindow(int type, bool win);

private:


  
	SDL_Texture* winlose_tex;
	ImageUI* ui_ingame;
	WindowUI* ui_pause_window;
	ButtonUI* ui_button[7];
	TextUI* ui_text[8];
	WindowUI* ui_options_window;
	ButtonUI* ui_button_options;
	TextUI* ui_text_options[2];
	WindowUI* ui_winlose_window;
	ButtonUI* ui_button_winlose[2];
	TextUI* ui_text_winlose[4];
	j1Timer timer_win_lose;
	bool start_timer;
	WindowUI* ui_confirmation_window;
	ButtonUI* ui_button_confirmation[2];
	TextUI* ui_text_confirmation[4];
	std::string confirmation_option;
  
public:
	SDL_Texture* debugBlue_tex;
	SDL_Texture* debugRed_tex;
	bool paused_game;

	TextUI* ui_text_ingame[3];
	CloseSceneMenus close_menus;
};

#endif // __j1SCENE_H__