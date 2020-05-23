#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

#define CURSOR_WIDTH 2

class TextUI;
class WindowUI;
class j1Timer;
enum Panel_Fade {
	no_one_fade,
	panel_fade_in,
	panel_fade_out
};

enum class Type
{
	NONE,
	BUTTON,
	IMAGE,
	WINDOW,
	TEXT,
	LISTTEXTS,
	INPUT,
	UNKNOWN
};

enum class UI_Audio
{
	NONE=-1,
	MAIN_MENU,
	CONFIRMATION,
	SAVE,
	LOAD,
	OPTIONS,
	RESTART,
	SURRENDER,
	EXIT,
	CLOSE,
	UNKNOWN
};

struct SDL_Texture;

class UI :public j1Module
{
public:
	UI(Type s_type, SDL_Rect r, UI* p, bool d, bool f, SDL_Rect d_area, bool consol = false, int num_toooltip = -1);

	// Destructor
	virtual ~UI() {}

	// Called when before render is available
	virtual bool Awake(pugi::xml_node&) { return true; }

	// Call before first frame
	virtual bool Start() { return true; }

	// Called before all Updates
	virtual bool PreUpdate();

	// Update
	virtual bool Update(float dt);

	// Called after all Updates
	virtual bool PostUpdate();

	// Called before quitting
	virtual bool CleanUp();

	// Called to destroy tooltip
	void DestroyTooltip();

	bool Move();

	void SetPriority(int prior);

	int GetPriority() { return priority; };
	
	// Called to show tooltip
	void ShowTooltip(int, int, uint, uint);

	SDL_Rect GetScreenRect();
	SDL_Rect GetParentScreenRect();
	SDL_Rect GetLocalRect();
	iPoint GetScreenPos();
	iPoint GetScreenToWorldPos();
	iPoint GetLocalPos();
	void SetLocalPos(iPoint pos);
	void SetRect(SDL_Rect rect);
	void SetScreenRect(SDL_Rect rect);
	void UpdateLocalRect();
	bool CheckMouse();
	bool CheckFocusable() { return focusable; }
	SDL_Rect GetDragArea() { return drag_area; }
	SDL_Rect Check_Printable_Rect(SDL_Rect sprite, iPoint& dif_sprite);
	SDL_Rect Check_Printable_Rect(SDL_Rect sprite, iPoint& dif_sprite, SDL_Rect quad);
	bool GetConsole() { return console; }

public:
	SDL_Rect sprite1;
	SDL_Rect quad;
	bool debug;
	bool focus;
	j1Module* listener;
	Type type;
	int num_atlas;
	j1Timer timer_tooltip;

private:
	SDL_Rect screen_rect;
	SDL_Rect local_rect;
	SDL_Rect mask_rect;
	UI* parent;
	bool drageable;
	bool focusable;
	SDL_Rect drag_area;
	bool console;
	int priority;
	bool has_tooltip;
	int tooltip_num;
	WindowUI* tooltip_window;
	TextUI* tooltip_texts[13];
	bool has_timer_tooltip_started;

protected:
	Panel_Fade fade_panel;
	int alpha;
	j1Timer fade_panel_timer;
	int fade_panel_time;
	int max_alpha;
};
class ImageUI :public UI
{
public:
	ImageUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, bool d, bool f, SDL_Rect d_area, float drag_position_scroll_bar, Panel_Fade p_fade, int num_tooltip);
	ImageUI(Type type, UI* p, SDL_Rect r, int re, int g, int b, int a, bool d, bool f, SDL_Rect d_area, Panel_Fade p_fade);

	// Destructor
	virtual ~ImageUI() {}

	// Called before all Updates
	bool Update(float dt);
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	fPoint GetDragPositionNormalized();

public:
	iPoint drag_position_0;
	iPoint drag_position_1;
	bool square;
	int red;
	int green;
	int blue;
	bool unclicked;
};
class WindowUI :public UI
{
public:
	WindowUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, bool d, bool f, SDL_Rect d_area, Panel_Fade p_fade);

	// Destructor
	virtual ~WindowUI() {}

	// Called after all Updates
	bool Update(float dt);
	bool PostUpdate();
};
class TextUI :public UI
{

public:
	TextUI(Type type, UI* p, SDL_Rect r, std::string str, bool d, bool f, SDL_Rect d_area, bool console, SDL_Color coulor, bool title, Panel_Fade p_fade, int num_tooltip);

	// Destructor
	virtual ~TextUI() {}

	// Called after all Updates
	bool Update(float dt);
	bool PostUpdate();

	void SetString(std::string);

public:
	std::string stri;
	SDL_Color color;

private:
	bool title_default;
};

class ListTextsUI :public UI
{
public:
	ListTextsUI(Type type, UI* p, SDL_Rect r, std::string str, bool d, bool f, SDL_Rect d_area, bool console, Panel_Fade p_fade);

	// Destructor
	virtual ~ListTextsUI() {}

	// Called after all Updates
	bool Update(float dt);
	bool PostUpdate();

	void SetListOfStrings(std::string string, int position);

	int GetNumberOfStrings() { return number_of_stri; }

private:

	std::list<std::string> stri;
	int number_of_stri;
};

class ButtonUI :public UI
{
public:

	ButtonUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, SDL_Rect spriten2, SDL_Rect spriten3, bool d, bool f, SDL_Rect d_area, int audio, Panel_Fade p_fade, int num_tooltip);

	// Destructor
	virtual ~ButtonUI() {}

	// Called when before render is available
	bool Awake(pugi::xml_node&) { return true; };

	// Call before first frame
	bool Start() { return true; };

	// Called before all Updates
	bool PreUpdate();
	bool Update(float dt);
	// Called after all Updates
	bool PostUpdate();

public:
	SDL_Rect sprite2;
	SDL_Rect sprite3;
	bool pushed;
	bool over;
	bool isLocked;
	bool front;
	int click_sfx;
};

class TextInputUI :public UI
{
public:
	TextInputUI(Type type, UI* p, SDL_Rect r, int re, int g, int b, int a, std::string str, bool d, bool f, SDL_Rect d_area, Panel_Fade p_fade);

	// Destructor
	virtual ~TextInputUI() {}

	// Called after all Updates
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void ChangeLabel(std::string text);

	void SetLabel(std::string text);

	void SetPositionToZero();

	std::string GetLabel() { return label; }

private:
	std::string label;
	bool text_input;
	int position;
	bool square;
	int red;
	int green;
	int blue;
	int alpha;
};
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called before all Updates
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	UI* CreateUIElement(Type type, UI* p, SDL_Rect r, SDL_Rect sprite = { 0,0,0,0 }, std::string str = "", Panel_Fade p_fade = Panel_Fade::no_one_fade, SDL_Rect sprite2 = { 0,0,0,0 }, SDL_Rect sprite3 = { 0,0,0,0 }, bool drageable = false,
		SDL_Rect drag_area = { 0,0,0,0 }, j1Module* s_listener = nullptr, int audio = 0, bool console = false, float drag_position_scroll_bar = -1, int number_atlas = 0,
		int num_tooltip = -1);
	UI* CreateUIElement(Type type, UI* p, SDL_Rect r, std::string str, int re, int g, int b, int a, bool drageable = false, SDL_Rect drag_area = { 0,0,0,0 }, j1Module* s_listener = nullptr, Panel_Fade p_fade = Panel_Fade::no_one_fade);

	bool DeleteUIElement(UI*);

	void ChangeDebug();

	void ChangeFocus();

	void DeleteFocus();

	const SDL_Texture* GetAtlas(int num_atlas) const;

	void ClearUI();

	void ReturnConsole();

	void WorkWithTextInput(std::string text);

	void DeactivateButtons();

	void ActivateButtons();

	SDL_Texture* GetTexture();

private:

	std::list <UI*> UIs;
	SDL_Texture* atlas_num_0;
	SDL_Texture* atlas_num_1;
	SDL_Texture* atlas_num_2;
	std::string atlas_file_name_num_0;
	std::string atlas_file_name_num_1;
	std::string atlas_file_name_num_2;
	SDL_Texture* cursor_tex;
	j1Timer minimap_feedback_timer;

public:

	//Audio
	int sfx_UI[9];
	bool lockClick;
	bool cursor_attack;
	bool cursor_move;
	iPoint cursor_size;

};

#endif // __j1GUI_H__