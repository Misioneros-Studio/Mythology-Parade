#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1TitleScene.h"
#include"j1Audio.h"


j1TitleScene::j1TitleScene() : j1Module()
{
	name.append("title_scene");
	active = true;

}

// Destructor
j1TitleScene::~j1TitleScene()
{}

// Called before render is available
bool j1TitleScene::Awake()
{
	LOG("Loading Title Scene");
	bool ret = true;

	return ret;
}


// Called before the first frame
bool j1TitleScene::Start()
{

	for (int i = 0; i < 6; i++)
	{
		ui_button[i] = nullptr;
		ui_text[i] = nullptr;
	}

	ui_button[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, nullptr, { 35,145,237,38 }, { 787,240,237,38 }, "NEW", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
	ui_text[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 122,158,237,38 }, { 0,0,100,100 }, "New Game", { 0,0,0,255 });
	ui_button[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, nullptr, { 35,220,237,38 }, { 787,240,237,38 }, "LOAD", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
	ui_text[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 119,232,237,38 }, { 0,0,100,100 }, "Load Game", { 0,0,0,255 });
	ui_button[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, nullptr, { 35,295,237,38 }, { 787,240,237,38 }, "TUTORIAL", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
	ui_text[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 128,307,237,38 }, { 0,0,100,100 }, "Tutorial", { 0,0,0,255 });
	ui_button[3] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, nullptr, { 35,370,237,38 }, { 787,240,237,38 }, "OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
	ui_text[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 129,382,237,38 }, { 0,0,100,100 }, "Options", { 0,0,0,255 });
	ui_button[4] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, nullptr, {35,445,237,38 }, { 787,240,237,38 }, "CREDITS", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
	ui_text[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 127,457,237,38 }, { 0,0,100,100 }, "Credits");
	ui_button[5] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, nullptr, { 35,520,237,38 }, { 787,240,237,38 }, "EXIT", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
	ui_text[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 140,532,237,38 }, { 0,0,100,100 }, "Exit", { 0,0,0,255 });

	close_menus = CloseTitleSceneMenus::None;

	cursor_tex = App->tex->Load("gui/cursors.png");
	title_assets_tex = App->tex->Load("gui/TitleAssets.png");
	App->audio->PlayMusic("audio/music/MainTitle_Use.ogg");
	return true;
}

// Called each loop iteration
bool j1TitleScene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1TitleScene::Update(float dt)
{
	switch (close_menus)
	{
	case::CloseTitleSceneMenus::Credits:
		DeactivateCredits();
		close_menus = CloseTitleSceneMenus::None;
		break;
	case::CloseTitleSceneMenus::Tutorial:
		DeactivateTutorial();
		close_menus = CloseTitleSceneMenus::None;
		break;
	case::CloseTitleSceneMenus::Options:
		DeactivateOptionsMenu();
		close_menus = CloseTitleSceneMenus::None;
		break;
	}
	SDL_Rect sec2 = { 0, 0, 1280, 720 };
	App->render->Blit(title_assets_tex, 0, 0, &sec2);
	return true;
}

// Called each loop iteration
bool j1TitleScene::PostUpdate()
{
	bool ret = true;

	//Show cursor ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	SDL_Rect sec = { 0, 0, 54, 45 };

	p = App->render->ScreenToWorld(x, y);

	App->render->Blit(cursor_tex, p.x, p.y, &sec);

	return ret;
}

// Called before quitting
bool j1TitleScene::CleanUp()
{
	LOG("Freeing title scene");
	for (int i = 5; i >= 0; i--) {
		if (ui_button[i] != nullptr) {
			App->gui->DeleteUIElement(ui_button[i]);
			ui_button[i] = nullptr;
		}
		if (ui_text[i] != nullptr) {
			App->gui->DeleteUIElement(ui_text[i]);
			ui_text[i] = nullptr;
		}
	}

	App->tex->UnLoad(cursor_tex);

	return true;
}




// Called when clicking options button in pause menu
void j1TitleScene::ActivateOptionsMenu() {
	if (ui_options_window == nullptr) {
		ui_options_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 });
		ui_button_options = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_options_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text_options[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,312,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text_options[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,212,237,38 }, { 0,0,100,100 }, "OPTIONS", { 255,255,255,255 }, { 1,0,0,0 });
	}
	for (int i = 0; i < 5; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking close button in options menu
void j1TitleScene::DeactivateOptionsMenu() {
	if (ui_options_window != nullptr) {
		App->gui->DeleteUIElement(ui_options_window);
		ui_options_window = nullptr;
		if (ui_button_options != nullptr) {
			App->gui->DeleteUIElement(ui_button_options);
			ui_button_options = nullptr;
		}
		for (int i = 1; i >= 0; i--) {
			if (ui_text_options[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_options[i]);
				ui_text_options[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}
	
}


// Called when clicking options button in pause menu
void j1TitleScene::ActivateTutorial() {
	if (ui_tutorial_window == nullptr) {
		ui_tutorial_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 });
		ui_tutorial_options = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_tutorial_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE TUTORIAL", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text_tutorial[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,312,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text_tutorial[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,212,237,38 }, { 0,0,100,100 }, "TUTORIAL", { 255,255,255,255 }, { 1,0,0,0 });
	}
	for (int i = 0; i < 5; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking close button in options menu
void j1TitleScene::DeactivateTutorial() {
	if (ui_tutorial_window != nullptr) {
		App->gui->DeleteUIElement(ui_tutorial_window);
		ui_tutorial_window = nullptr;
		if (ui_tutorial_options != nullptr) {
			App->gui->DeleteUIElement(ui_tutorial_options);
			ui_tutorial_options = nullptr;
		}
		for (int i = 1; i >= 0; i--) {
			if (ui_text_tutorial[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_tutorial[i]);
				ui_text_tutorial[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}

}


// Called when clicking options button in pause menu
void j1TitleScene::ActivateCredits() {
	if (ui_credits_window == nullptr) {
		ui_credits_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 });
		ui_button_credits = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE CREDITS", { 787,342,237,38 }, { 787,291,237,38 }, false, { 0,0,0,0 }, this);
		ui_text_credits[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,312,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text_credits[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,212,237,38 }, { 0,0,100,100 }, "CREDITS", { 255,255,255,255 }, { 1,0,0,0 });
	}
	for (int i = 0; i < 5; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking close button in options menu
void j1TitleScene::DeactivateCredits() {
	if (ui_credits_window != nullptr) {
		App->gui->DeleteUIElement(ui_credits_window);
		ui_credits_window = nullptr;
		if (ui_button_credits != nullptr) {
			App->gui->DeleteUIElement(ui_button_credits);
			ui_button_credits = nullptr;
		}
		for (int i = 1; i >= 0; i--) {
			if (ui_text_credits[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_credits[i]);
				ui_text_credits[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}

}


void j1TitleScene::OnClick(UI* element, float argument)
{
	switch (element->type)
	{

	case Type::BUTTON:

		if (element->name == "NEW")
		{
			App->change_scene = true;
			destroy = true;
		}
		else if (element->name == "LOAD")
		{
			App->LoadGame("save_game.xml");
		}
		else if (element->name == "TUTORIAL")
		{
			ActivateTutorial();
		}
		else if (element->name == "CLOSE TUTORIAL")
		{
			close_menus = CloseTitleSceneMenus::Tutorial;
		}
		else if (element->name == "OPTIONS")
		{
			ActivateOptionsMenu();
		}
		else if (element->name == "CLOSE OPTIONS")
		{
			close_menus = CloseTitleSceneMenus::Options;
		}
		else if (element->name == "CREDITS")
		{
			ActivateCredits();
		}
		else if (element->name == "CLOSE CREDITS")
		{
			close_menus = CloseTitleSceneMenus::Credits;
		}
		else if (element->name == "EXIT")
		{
			exitGame = true;
		}
		break;


	default:
		break;
	}
}















/*ui_pause_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,100,459,531 }, { 216,21,459,531 });
ui_button[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,160,237,38 }, { 787,240,237,38 }, "SAVE", { 787,342,237,38 }, { 787,291,237,38 }, false,
	{ 0,0,0,0 }, this, (int)UI_Audio::SAVE);
ui_text[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,172,237,38 }, { 0,0,100,100 }, "Save Game", { 0,0,0,255 });
ui_button[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,220,237,38 }, { 787,240,237,38 }, "LOAD", { 787,342,237,38 }, { 787,291,237,38 }, false,
	{ 0,0,0,0 }, this, (int)UI_Audio::LOAD);
ui_text[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,232,237,38 }, { 0,0,100,100 }, "Load Game", { 0,0,0,255 });
ui_button[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,280,237,38 }, { 787,240,237,38 }, "OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false,
	{ 0,0,0,0 }, this, (int)UI_Audio::OPTIONS);
ui_text[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 613,292,237,38 }, { 0,0,100,100 }, "Options", { 0,0,0,255 });
ui_button[3] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,340,237,38 }, { 787,240,237,38 }, "RESTART", { 787,342,237,38 }, { 787,291,237,38 }, false,
	{ 0,0,0,0 }, this, (int)UI_Audio::RESTART);
ui_text[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 614,352,237,38 }, { 0,0,100,100 }, "Restart", { 0,0,0,255 });
ui_button[4] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,400,237,38 }, { 787,240,237,38 }, "SURRENDER", { 787,342,237,38 }, { 787,291,237,38 }, false,
	{ 0,0,0,0 }, this, (int)UI_Audio::SURRENDER);
ui_text[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,412,237,38 }, { 0,0,100,100 }, "Surrender");
ui_button[5] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,460,237,38 }, { 787,240,237,38 }, "EXIT", { 787,342,237,38 }, { 787,291,237,38 }, false,
	{ 0,0,0,0 }, this, (int)UI_Audio::EXIT);
ui_text[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 625,472,237,38 }, { 0,0,100,100 }, "Exit", { 0,0,0,255 });
ui_button[6] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,560,237,38 }, { 787,240,237,38 }, "CLOSE", { 787,342,237,38 }, { 787,291,237,38 }, false,
	{ 0,0,0,0 }, this, (int)UI_Audio::CLOSE);
ui_text[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 620,572,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
ui_text[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,112,237,38 }, { 0,0,100,100 }, "PAUSE", { 255,255,255,255 }, { 1,0,0,0 });*/