#ifndef __j1TITLESCENE__
#define __j1TITLESCENE__

#include "j1Module.h"

enum class CloseTitleSceneMenus{
	None,
	Credits,
	Options,
	Confirmation,
	Civilization,
	Unknown
};
struct SDL_Texture;
class ImageUI;
class WindowUI;
class ButtonUI;
class TextUI;
class QuadTree;

class j1TitleScene : public j1Module
{
public:

	j1TitleScene();

	// Destructor
	virtual ~j1TitleScene();

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

	// Called when creating the menu
	void MoveMenu();

	// Called when clicking options button in menu
	void ActivateOptionsMenu();

	// Called when clicking close button in options menu
	void DeactivateOptionsMenu();

	// Called when clicking credits button in menu
	void ActivateCredits();

	// Called when clicking close button in credits
	void DeactivateCredits();

	// Called when clicking new game button in menu
	void ActivatCivilizationMenu();

	// Called when clicking close button in civilization menu or after picking a civilization
	void DeactivateCivilizationMenu();

	// Called when clicking a button in the menu with confirmation message
	void ActivateConfirmationMenu(std::string str);

	// Called when clicking no in the confirmation message
	void DeactivateConfirmationMenu();


	void OnClick(UI* element, float argument = 0);

private:
	SDL_Texture* title_assets_tex;
	ButtonUI* ui_button[6];
	TextUI* ui_text[6];
	WindowUI* ui_options_window;
	ButtonUI* ui_button_options[2];
	TextUI* ui_text_options[3];
	WindowUI* ui_credits_window;
	ButtonUI* ui_button_credits[9];
	TextUI* ui_text_credits[18];
	WindowUI* ui_confirmation_window;
	ButtonUI* ui_button_confirmation[2];
	TextUI* ui_text_confirmation[4];
	std::string confirmation_option;
	WindowUI* ui_civilization_window;
	ButtonUI* ui_button_civilization[3];
	TextUI* ui_text_civilization[4];
	ImageUI* ui_volume_sliders[6];
	TextUI* ui_text_volume_sliders[2];
	ImageUI* ui_pause_black_screen;
public:
	CloseTitleSceneMenus close_menus;
	bool wantToLoad;
	bool destroy;
};

#endif // __j1TITLESCENE_H__