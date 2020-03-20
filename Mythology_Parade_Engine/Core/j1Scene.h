#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

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
	bool Awake();

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

	void OnClick(UI* element, float argument = 0);

private:
	SDL_Texture* debug_tex;
	SDL_Texture* cursor_tex;
	ImageUI* ui_ingame;
	WindowUI* ui_pause_window;
	ButtonUI* ui_button[7];
	TextUI* ui_text[8];
	WindowUI* ui_options_window;
	ButtonUI* ui_button_options;
	TextUI* ui_text_options[2];
public:
	QuadTree* quadTree;
};

#endif // __j1SCENE_H__