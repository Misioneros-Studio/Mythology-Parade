#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1TitleScene.h"
#include "j1Audio.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "j1ParticleManager.h"

j1TitleScene::j1TitleScene() : j1Module()
{
	name.append("title_scene");
	active = true;

}

// Destructor
j1TitleScene::~j1TitleScene()
{}

// Called before render is available
bool j1TitleScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Title Scene");
	bool ret = true;

	active = false;

	return ret;
}


// Called before the first frame
bool j1TitleScene::Start()
{
	App->gui->active = true;
	App->gui->Start();

	for (int i = 0; i < 6; i++)
	{
		ui_button[i] = nullptr;
		ui_text[i] = nullptr;
	}
	destroy = false;
	wantToLoad = false;
	App->audio->CleanFxs();

	App->gui->sfx_UI[(int)UI_Audio::MAIN_MENU] = App->audio->LoadFx("audio/ui/Click_Main_Menu.wav");
	App->gui->sfx_UI[(int)UI_Audio::CONFIRMATION] = App->audio->LoadFx("audio/ui/Click_Standard2.wav");
	App->gui->sfx_UI[(int)UI_Audio::CLOSE] = App->audio->LoadFx("audio/ui/Close_Menu.wav");
	App->gui->sfx_UI[(int)UI_Audio::LOAD] = App->audio->LoadFx("audio/ui/load.wav");


	ui_button[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, nullptr, { 1535,145,237,38 }, { 787,240,237,38 }, "NEW", { 787,342,237,38 }, { 787,291,237,38 }, false,
		{ 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
	ui_text[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 1622,158,237,38 }, { 0,0,100,100 }, "New Game", { 0,0,0,255 }));
	ui_button[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, nullptr, { 1535,220,237,38 }, { 787,240,237,38 }, "LOAD", { 787,342,237,38 }, { 787,291,237,38 }, false, 
		{ 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
	ui_text[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 1619,232,237,38 }, { 0,0,100,100 }, "Load Game", { 0,0,0,255 }));
	ui_button[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, nullptr, { 1535,295,237,38 }, { 787,240,237,38 }, "TUTORIAL", { 787,342,237,38 }, { 787,291,237,38 }, false,
		{ 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
	ui_text[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 1628,307,237,38 }, { 0,0,100,100 }, "Tutorial", { 0,0,0,255 }));
	ui_button[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, nullptr, { 1535,370,237,38 }, { 787,240,237,38 }, "OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false, 
		{ 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
	ui_text[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 1629,382,237,38 }, { 0,0,100,100 }, "Options", { 0,0,0,255 }));
	ui_button[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, nullptr, { 1535,445,237,38 }, { 787,240,237,38 }, "CREDITS", { 787,342,237,38 }, { 787,291,237,38 }, false,
		{ 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
	ui_text[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 1627,457,237,38 }, { 0,0,100,100 }, "Credits"));
	ui_button[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, nullptr, { 1535,520,237,38 }, { 787,240,237,38 }, "EXIT", { 787,342,237,38 }, { 787,291,237,38 }, false, 
		{ 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
	ui_text[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 1640,532,237,38 }, { 0,0,100,100 }, "Exit", { 0,0,0,255 }));
	

	close_menus = CloseTitleSceneMenus::None;


	for (int i = 0; i < 18; i++) {
		ui_text_credits[i] = nullptr;
		if (i < 9) {
			ui_button_credits[i] = nullptr;
			if (i < 6) {
				ui_volume_sliders[i] = nullptr;
				if (i < 4) {
					if (i < 3) {
						if (i < 2) {
							ui_button_options[i] = nullptr;
							ui_text_tutorial[i] = nullptr;
							ui_button_confirmation[i] = nullptr;
							ui_text_volume_sliders[i] = nullptr;
						}
						ui_button_civilization[i] = nullptr;
						ui_text_options[i] = nullptr;
					}
					ui_text_civilization[i] = nullptr;
					ui_text_confirmation[i] = nullptr;
				}
			}
		}
	}
	ui_tutorial_options = nullptr;
	ui_pause_black_screen = nullptr;


	ui_civilization_window = nullptr;
	ui_confirmation_window = nullptr;
	ui_tutorial_window = nullptr;
	ui_options_window = nullptr;
	ui_credits_window = nullptr;


	confirmation_option = "";
	title_assets_tex = App->tex->Load("gui/TitleAssets.png");
	App->audio->PlayMusic("audio/music/MainTitle-Use2.ogg", 2.0F);

	return true;
}

// Called each loop iteration
bool j1TitleScene::PreUpdate()
{
	if (ui_button[0]->GetScreenRect().x!=35&&App->fade_to_black->current_step==App->fade_to_black->none)
		MoveMenu();
	return true;
}

// Called each loop iteration
bool j1TitleScene::Update(float dt)
{
	switch (close_menus)
	{
	case CloseTitleSceneMenus::Credits:
		DeactivateCredits();
		close_menus = CloseTitleSceneMenus::None;
		break;
	case CloseTitleSceneMenus::Tutorial:
		DeactivateTutorial();
		close_menus = CloseTitleSceneMenus::None;
		break;
	case CloseTitleSceneMenus::Options:
		DeactivateOptionsMenu();
		close_menus = CloseTitleSceneMenus::None;
		break;
	case CloseTitleSceneMenus::Confirmation:
		DeactivateConfirmationMenu();
		close_menus = CloseTitleSceneMenus::None;
		break;
	case CloseTitleSceneMenus::Civilization:
		DeactivateCivilizationMenu();
		close_menus = CloseTitleSceneMenus::None;
		break;
	}
	if (ui_volume_sliders[0] != nullptr) {
		ui_volume_sliders[0]->quad.w= (int)(181 * ((float)App->audio->GetVolumeMusic() / (float)128));
	}
	if (ui_volume_sliders[3] != nullptr) {
		ui_volume_sliders[3]->quad.w= (int)(181 * ((float)App->audio->GetVolumeFx() / (float)128));
		if (ui_volume_sliders[5]->unclicked == true) {
			App->audio->PlayFx(-1, App->gui->sfx_UI[(int)UI_Audio::LOAD]);
		}
	}
	SDL_Rect sec2 = { 0, 0, App->render->camera.w, App->render->camera.h };
	App->render->Blit(title_assets_tex, 0, 0, &sec2, 0.f);
	return true;
}

// Called each loop iteration
bool j1TitleScene::PostUpdate()
{
	bool ret = true;

	

	return ret;
}

// Called before quitting
bool j1TitleScene::CleanUp()
{
	LOG("Freeing title scene");
	DeactivateCivilizationMenu();
	DeactivateCredits();
	DeactivateOptionsMenu();
	DeactivateTutorial();
	DeactivateConfirmationMenu();
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

	App->tex->UnLoad(title_assets_tex);

	return true;
}


void j1TitleScene::MoveMenu() {
	for (int i = 0; i < 6; i++)
	{
		bool stop = false;
		SDL_Rect r = ui_button[i]->GetScreenRect();
		float n = 35 + r.x;
		r.x -= 0.1*n;
		if (r.x < 35) {
			n = 35 - r.x;
			r.x = 35;
			stop = true;
		}
		ui_button[i]->SetRect(r);
		r = ui_text[i]->GetScreenRect();
		if (stop == true)
			r.x -= n;
		else
			r.x -= 0.1*n;
		ui_text[i]->SetRect(r);
	}
}

// Called when clicking options button in pause menu
void j1TitleScene::ActivateOptionsMenu() {
	if (ui_options_window == nullptr) {
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150));
		ui_options_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,321 }, { 1278,4,459,321 }));
		ui_button_options[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_options_window, { 520,453,237,38 }, { 787,240,237,38 }, "CLOSE OPTIONS", { 787,342,237,38 },
			{ 787,291,237,38 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CLOSE));
		ui_button_options[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_options_window, { 570,260,36,36 }, { 16,21,36,36 }, "FULLSCREEN", { 98,21,36,36 },
			{ 57,21,36,36 }, false, { 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
		if (App->win->isFullscreen() == true) {
			ui_button_options[1]->sprite1.y = ui_button_options[1]->sprite2.y = ui_button_options[1]->sprite3.y = 61;
		}
		ui_text_options[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 619,465,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 }));
		ui_text_options[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 583,212,237,38 }, { 0,0,100,100 }, "OPTIONS", { 255,255,255,255 }, { 1,0,0,0 }));
		ui_text_options[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 620,270,237,38 }, { 0,0,100,100 }, "FULLSCREEN", { 255,255,255,255 }));
		ui_text_volume_sliders[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 590,314,237,38 }, { 0,0,100,100 }, "MUSIC'S VOLUME", { 255,255,255,255 }));
		ui_volume_sliders[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,340,(int)(181* ((float)App->audio->GetVolumeMusic() / (float)128)),17 },
			{ 1072,250,181,17 }));
		ui_volume_sliders[1]=static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 531,334,215,30 }, { 1053,350,215,30 }));
		ui_volume_sliders[2]=static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,337,36,24 }, { 1072,180,36,24 }, "VOLUME_CONTROL", { 0,0,0,0 }, { 0,0,0,0 },
			true, { 548,337,181,0 }, App->audio, 0, false, ((float)App->audio->GetVolumeMusic() / (float)128)));
		ui_text_volume_sliders[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 598,379,237,38 }, { 0,0,100,100 }, "FX'S VOLUME", { 255,255,255,255 }));
		ui_volume_sliders[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,405,(int)(181 * ((float)App->audio->GetVolumeFx() / (float)128)),17 },
			{ 1072,250,181,17 }));
		ui_volume_sliders[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 531,399,215,30 }, { 1053,350,215,30 }));
		ui_volume_sliders[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,402,36,24 }, { 1072,180,36,24 }, "FX_CONTROL", { 0,0,0,0 }, { 0,0,0,0 },
			true, { 548,402,181,0 }, App->audio, 0, false, ((float)App->audio->GetVolumeFx() / (float)128)));

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
		if (ui_pause_black_screen != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen);
			ui_pause_black_screen = nullptr;
		}
		for (int i = 5; i >= 0; i--) {
			if (ui_volume_sliders[i] != nullptr) {
				App->gui->DeleteUIElement(ui_volume_sliders[i]);
				ui_volume_sliders[i] = nullptr;
			}
			if (i < 3) {
				if (i < 2) {
					if (ui_button_options[i] != nullptr) {
						App->gui->DeleteUIElement(ui_button_options[i]);
						ui_button_options[i] = nullptr;
					}
					if (ui_text_volume_sliders[i] != nullptr) {
						App->gui->DeleteUIElement(ui_text_volume_sliders[i]);
						ui_text_volume_sliders[i] = nullptr;
					}
				}
				if (ui_text_options[i] != nullptr) {
					App->gui->DeleteUIElement(ui_text_options[i]);
					ui_text_options[i] = nullptr;
				}
			}
		}
	}
	for (int i = 0; i < 6; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}
	
}


// Called when clicking new game button in menu
void j1TitleScene::ActivatCivilizationMenu() {
	if (ui_civilization_window == nullptr) {
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150));
		ui_civilization_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 }));
		ui_button_civilization[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_civilization_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE CIVILIZATION", 
			{ 787,342,237,38 }, { 787,291,237,38 },	false, { 0,0,0,0 }, this, (int)UI_Audio::CLOSE));
		ui_button_civilization[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_civilization_window, { 450,250,117,38 }, { 834,125,117,24 }, "GREEK", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
		ui_button_civilization[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_civilization_window, { 710,250,117,38 }, { 834,125,117,24 }, "VIKING", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::MAIN_MENU));
		ui_text_civilization[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,312,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 }));
		ui_text_civilization[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 463,212,237,38 }, { 0,0,100,100 }, "CHOOSE YOUR CIVILIZATION", { 255,255,255,255 },
			{ 1,0,0,0 }));
		ui_text_civilization[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 470,256,237,38 }, { 0,0,100,100 }, "Greek", { 0,0,0,255 }, { 1,0,0,0 }));
		ui_text_civilization[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 725,256,237,38 }, { 0,0,100,100 }, "Viking", { 0,0,0,255 }, { 1,0,0,0 }));
	}
	for (int i = 0; i < 6; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking close button in civilization menu or after picking a civilization
void j1TitleScene::DeactivateCivilizationMenu() {
	if (ui_civilization_window != nullptr) {
		App->gui->DeleteUIElement(ui_civilization_window);
		ui_civilization_window = nullptr;
		if (ui_pause_black_screen != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen);
			ui_pause_black_screen = nullptr;
		}
		for (int i = 3; i >= 0; i--) {
			if (i < 3) {
				if (ui_button_civilization[i] != nullptr) {
					App->gui->DeleteUIElement(ui_button_civilization[i]);
					ui_button_civilization[i] = nullptr;
				}
			}
			for (int i = 3; i >= 0; i--) {
				if (ui_text_civilization[i] != nullptr) {
					App->gui->DeleteUIElement(ui_text_civilization[i]);
					ui_text_civilization[i] = nullptr;
				}
			}
		}
	}
	for (int i = 0; i < 6; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}
}

// Called when clicking options button in pause menu
void j1TitleScene::ActivateTutorial() {
	if (ui_tutorial_window == nullptr) {
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150));
		ui_tutorial_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 }));
		ui_tutorial_options = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_tutorial_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE TUTORIAL", { 787,342,237,38 },
			{ 787,291,237,38 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CLOSE));
		ui_text_tutorial[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,312,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 }));
		ui_text_tutorial[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,212,237,38 }, { 0,0,100,100 }, "TUTORIAL", { 255,255,255,255 }, { 1,0,0,0 }));
	}
	for (int i = 0; i < 6; i++) {
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
		if (ui_pause_black_screen != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen);
			ui_pause_black_screen = nullptr;
		}
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
	for (int i = 0; i < 6; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}

}


// Called when clicking options button in pause menu
void j1TitleScene::ActivateCredits() {
	if (ui_credits_window == nullptr) {
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150));
		ui_credits_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,321 }, { 1278,4,459,321 }));
		ui_button_credits[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 520,453,237,38 }, { 787,240,237,38 }, "CLOSE CREDITS", { 787,342,237,38 },
			{ 787,291,237,38 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CLOSE));
		ui_text_credits[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,465,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 }));
		ui_text_credits[17] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 587,212,237,38 }, { 0,0,100,100 }, "CREDITS", { 255,255,255,255 }, { 1,0,0,0 }));
		ui_text_credits[9] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 435,256,237,38 }, { 0,0,100,100 }, "Leader", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 420,288,117,24 }, { 834,125,117,24 }, "BERNAT'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 457,294,237,38 }, { 0,0,100,100 }, "Bernat", { 0,0,0,255 }));
		ui_text_credits[10] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 602,256,237,38 }, { 0,0,100,100 }, "Mngt.", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 581,288,117,24 }, { 834,125,117,24 }, "JORDI'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 622,294,237,38 }, { 0,0,100,100 }, "Jordi", { 0,0,0,255 }));
		ui_text_credits[11] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 757,256,237,38 }, { 0,0,100,100 }, "Design", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 742,288,117,24 }, { 834,125,117,24 }, "EUDALD'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 779,294,237,38 }, { 0,0,100,100 }, "Eudald", { 0,0,0,255 }));
		ui_text_credits[12] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 445,323,237,38 }, { 0,0,100,100 }, "Code", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 420,355,117,24 }, { 834,125,117,24 }, "MIQUEL'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 457,361,237,38 }, { 0,0,100,100 }, "Miquel", { 0,0,0,255 }));
		ui_text_credits[13] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 618,323,237,38 }, { 0,0,100,100 }, "Art", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 581,355,117,24 }, { 834,125,117,24 }, "JOSEP'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 622,361,237,38 }, { 0,0,100,100 }, "Josep", { 0,0,0,255 }));
		ui_text_credits[14] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 787,323,237,38 }, { 0,0,100,100 }, "UI", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[6] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 742,355,117,24 }, { 834,125,117,24 }, "ARNAU'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[6] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 781,361,237,38 }, { 0,0,100,100 }, "Arnau", { 0,0,0,255 }));
		ui_text_credits[15] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 441,390,237,38 }, { 0,0,100,100 }, "Audio", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[7] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 420,422,117,24 }, { 834,125,117,24 }, "MARC'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[7] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 461,428,237,38 }, { 0,0,100,100 }, "Marc", { 0,0,0,255 }));
		ui_text_credits[16] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 783,390,237,38 }, { 0,0,100,100 }, "QA", { 255,255,255,255 }, { 1,0,0,0, }));
		ui_button_credits[8] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_credits_window, { 742,422,117,24 }, { 834,125,117,24 }, "ROGER'S GITHUB", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_credits[8] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 781,428,237,38 }, { 0,0,100,100 }, "Roger", { 0,0,0,255 }));
	}
	for (int i = 0; i < 6; i++) {
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
		if (ui_pause_black_screen != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen);
			ui_pause_black_screen = nullptr;
		}
		for (int i = 17; i >= 0; i--) {
			if (i < 9) {
				if (ui_button_credits[i] != nullptr) {
					App->gui->DeleteUIElement(ui_button_credits[i]);
					ui_button_credits[i] = nullptr;
				}
			}
			if (ui_text_credits[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_credits[i]);
				ui_text_credits[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < 6; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}

}

// Called when clicking a button in the menu with confirmation message
void j1TitleScene::ActivateConfirmationMenu(std::string str) {
	if (ui_confirmation_window == nullptr) {
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150));
		ui_confirmation_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 }));
		ui_button_confirmation[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 470,300,117,38 }, { 834,125,117,24 }, "YES", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_confirmation[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 509,306,237,38 }, { 0,0,100,100 }, "Yes", { 0,0,0,255 }, { 1,0,0,0 }));
		ui_button_confirmation[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 690,300,117,38 }, { 834,125,117,24 }, "NO", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, this, (int)UI_Audio::CONFIRMATION));
		ui_text_confirmation[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 731,306,237,38 }, { 0,0,100,100 }, "No", { 0,0,0,255 }, { 1,0,0,0 }));
		std::string text = str + " ?";
		ui_text_confirmation[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 463,212,237,38 }, { 0,0,100,100 }, "ARE YOU SURE YOU WANT TO", { 255,255,255,255 }, 
			{ 1,0,0,0 }));
		int size = text.length();
		ui_text_confirmation[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 640 - (6 * size),247,237,38 }, { 0,0,100,100 }, text, { 255,255,255,255 }, { 1,0,0,0 }));

	}
	for (int i = 0; i < 6; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking no in the confirmation message
void j1TitleScene::DeactivateConfirmationMenu() {
	confirmation_option.clear();
	if (ui_confirmation_window != nullptr) {
		App->gui->DeleteUIElement(ui_confirmation_window);
		ui_confirmation_window = nullptr;
		if (ui_pause_black_screen != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen);
			ui_pause_black_screen = nullptr;
		}
		for (int i = 3; i >= 0; i--) {
			if (i < 2)
			{
				if (ui_button_confirmation[i] != nullptr) {
					App->gui->DeleteUIElement(ui_button_confirmation[i]);
					ui_button_confirmation[i] = nullptr;
				}
			}
			if (ui_text_confirmation[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_confirmation[i]);
				ui_text_confirmation[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < 6; i++) {
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
			ActivatCivilizationMenu();
		}
		else if (element->name == "CLOSE CIVILIZATION")
		{
			close_menus = CloseTitleSceneMenus::Civilization;
		}
		else if (element->name == "LOAD")
		{
			confirmation_option = "LOAD";
			ActivateConfirmationMenu("LOAD THE GAME");
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
			confirmation_option = "EXIT";
			ActivateConfirmationMenu("EXIT");
		}
		else if (element->name == "NO")
		{
			close_menus = CloseTitleSceneMenus::Confirmation;
		}
		else if (element->name == "YES")
		{
			if (confirmation_option.compare("LOAD") == 0)
			{
				App->fade_to_black->FadeToBlack(which_fade::title_to_scene, 2, "viking");
				destroy = true;
				wantToLoad = true;
			}
			else if (confirmation_option.compare("EXIT") == 0)
			{
				exitGame = true;
			}
			close_menus = CloseTitleSceneMenus::Confirmation;
		}
		else if (element->name == "GREEK")
		{
			App->fade_to_black->FadeToBlack(which_fade::title_to_scene, 2, "greek");

			destroy = true;
			wantToLoad = false;
		}
		else if (element->name == "VIKING")
		{
			App->fade_to_black->FadeToBlack(which_fade::title_to_scene, 2, "viking");

			destroy = true;
			wantToLoad = false;
		}
		else if (element->name == "FULLSCREEN") {
			App->win->ToggleFullscreen();
			if (ui_button_options[1]->sprite1.y == 21) {
				ui_button_options[1]->sprite1.y = ui_button_options[1]->sprite2.y = ui_button_options[1]->sprite3.y = 61;
			}
			else if (ui_button_options[1]->sprite1.y == 61) {
				ui_button_options[1]->sprite1.y = ui_button_options[1]->sprite2.y = ui_button_options[1]->sprite3.y = 21;
			}
		}
		else if (element->name == "BERNAT'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/BernatCasanas", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element->name == "JORDI'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/Jordi-Pardo", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element->name == "EUDALD'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/Hevne", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element->name == "MIQUEL'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/MayKoder", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element->name == "JOSEP'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/daskza19", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element->name == "ARNAU'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/Arnau77", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element->name == "MARC'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/Ramsubito", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element->name == "ROGER'S GITHUB") {
			ShellExecuteA(NULL, "open", "https://github.com/Rugiacreed", NULL, NULL, SW_SHOWNORMAL);
		}
		break;


	default:
		break;
	}
}