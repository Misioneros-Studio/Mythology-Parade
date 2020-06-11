#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "EntityManager.h"
#include "j1Window.h"
#include "CombatUnit.h"
#include "HUD.h"
#include "ResearchMenu.h"
#include "TooltipData.h"
#include "j1TutorialScene.h"


//Constructor
HUD::HUD(ResearchMenu* r) 
{
	StartHUD(r);
}

//Destructor
HUD::~HUD() {
	research_menu = nullptr;
}

// Called before the first frame
void HUD::StartHUD(ResearchMenu* r) {
	research_menu = r;

	ui_winlose_window = nullptr;
	ui_pause_window = nullptr;
	ui_options_window = nullptr;
	ui_confirmation_window = nullptr;

	type_thing_selected = Type_Selected::Unknown;



	start_timer = false;
	ui_ingame = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,590,1280,130 }, { 0,590,1280,130 }));
	range = damage = influence = health = level = experience = max_cap = speed = 0;

	for (int i = 0; i < 12; i++)
	{
		hud_list_troops[i] = nullptr;
		hud_number_troops[i] = nullptr;
		hud_stats_selected_troop[i] = nullptr;
		number_of_troops[i] = 0;
		type_of_troops[i] = UnitType::NONE;
		if (i < 9) {
			hud_button_actions[i] = nullptr;
			hud_button_actions_unclickable[i] = nullptr;
			if (i < 8)
			{
				if (i < 7)
				{
					ui_button[i] = nullptr;
					if (i < 5) {
						ui_volume_sliders[i] = nullptr;
						if (i < 4) {
							ui_text_winlose[i] = nullptr;
							ui_text_confirmation[i] = nullptr;
							if (i < 3) {
								ui_button_confirmation[i] = nullptr;
								ui_text_ingame[i] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 104,610 + (i * 33),57,38 }, { 0,0,100,100 }, "9999", Panel_Fade::panel_fade_in, { 255,255,255,255 }
								, { 1,0,0,0 }, false, { 0,0,0,0 },nullptr,0,false,-1.0f,0, (int)TooltipsAvailable::currencymoduletip));
								ui_text_options[i] = nullptr;
								if (i < 2) {
									ui_button_winlose[i] = nullptr;
									ui_button_options[i] = nullptr;
									ui_pause_black_screen[i] = nullptr;
									ui_text_volume_sliders[i] = nullptr;
									hud_bar_producing[i] = nullptr;
								}
							}
						}
					}
				}
				ui_text[i] = nullptr;
			}
		}
	}


	hud_selected_troop = nullptr;
	thing_selected = nullptr;

	confirmation_option = "";

	close_menus = CloseSceneMenus::None;

}

// Called when clicking esc
void HUD::ActivatePauseMenu() {

	if (ui_pause_window == nullptr) {
		App->audio->PlayFx(3, App->scene->OpenPauseMenu_sfx);
		PauseGame();
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150, Panel_Fade::panel_fade_in));
		if (App->scene->isInTutorial == false) {
			ui_pause_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,50,459,531 }, { 216,21,459,531 }, "", Panel_Fade::panel_fade_in));
			ui_button[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,110,237,38 }, { 787,240,237,38 }, "SAVE", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::SAVE));
			ui_text[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,122,237,38 }, { 0,0,100,100 }, "Save Game", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
			ui_button[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,170,237,38 }, { 787,240,237,38 }, "LOAD", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::LOAD));
			ui_text[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,182,237,38 }, { 0,0,100,100 }, "Load Game", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
			ui_button[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,230,237,38 }, { 787,240,237,38 }, "OPTIONS", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::OPTIONS));
			ui_text[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 613,242,237,38 }, { 0,0,100,100 }, "Options", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
			ui_button[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,290,237,38 }, { 787,240,237,38 }, "RESTART", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::RESTART));
			ui_text[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 614,302,237,38 }, { 0,0,100,100 }, "Restart", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
			ui_button[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,350,237,38 }, { 787,240,237,38 }, "SURRENDER", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::SURRENDER));
			ui_text[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,362,237,38 }, { 0,0,100,100 }, "Surrender", Panel_Fade::panel_fade_in));
			ui_button[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,410,237,38 }, { 787,240,237,38 }, "EXIT", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::EXIT));
			ui_text[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 625,422,237,38 }, { 0,0,100,100 }, "Exit", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
			ui_button[6] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,510,237,38 }, { 787,240,237,38 }, "CLOSE", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE));
			ui_text[6] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 620,522,237,38 }, { 0,0,100,100 }, "Close", Panel_Fade::panel_fade_in));
			ui_text[7] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,62,237,38 }, { 0,0,100,100 }, "PAUSE", Panel_Fade::panel_fade_in, { 255,255,255,255 }, { 1,0,0,0 }));
		}
		else {
			ui_pause_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,150,459,303 }, { 216,21,459,531 }, "", Panel_Fade::panel_fade_in));
			ui_button[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,210,237,38 }, { 787,240,237,38 }, "OPTIONS", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::OPTIONS));
			ui_text[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 613,222,237,38 }, { 0,0,100,100 }, "Options", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
			ui_button[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,270,237,38 }, { 787,240,237,38 }, "EXIT", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::EXIT));
			ui_text[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 625,282,237,38 }, { 0,0,100,100 }, "Exit", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
			ui_button[6] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,390,237,38 }, { 787,240,237,38 }, "CLOSE", Panel_Fade::panel_fade_in, { 787,342,237,38 },
				{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE));
			ui_text[6] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 620,402,237,38 }, { 0,0,100,100 }, "Close", Panel_Fade::panel_fade_in));
			ui_text[7] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,162,237,38 }, { 0,0,100,100 }, "PAUSE", Panel_Fade::panel_fade_in, { 255,255,255,255 }, { 1,0,0,0 }));
		}
	}
}

// Called when clicking close button in pause menu
void HUD::DeactivatePauseMenu() {
	if (ui_pause_window != nullptr) {
		App->gui->DeleteUIElement(ui_pause_window);
		ui_pause_window = nullptr;
		if (ui_pause_black_screen[0] != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen[0]);
			ui_pause_black_screen[0] = nullptr;
		}
		for (int i = 7; i >= 0; i--) {
			if (i != 7) {
				if (ui_button[i] != nullptr) {
					App->gui->DeleteUIElement(ui_button[i]);
					ui_button[i] = nullptr;
				}
			}
			if (ui_text[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text[i]);
				ui_text[i] = nullptr;
			}
		}
	}
	ResumeGame();

}

// Called when clicking options button in pause menu
void HUD::ActivateOptionsMenu() {
	if (ui_options_window == nullptr) {
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150, Panel_Fade::panel_fade_in));
		ui_options_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,321 }, { 1278,4,459,321 },"",Panel_Fade::panel_fade_in));
		ui_button_options[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_options_window, { 520,453,237,38 }, { 787,240,237,38 }, "CLOSE OPTIONS", Panel_Fade::panel_fade_in, { 787,342,237,38 },
			{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE));
		ui_button_options[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_options_window, { 570,260,36,36 }, { 16,21,36,36 }, "FULLSCREEN", Panel_Fade::panel_fade_in, { 98,21,36,36 },
			{ 57,21,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU));
		if (App->win->isFullscreen() == true) {
			ui_button_options[1]->sprite1.y = ui_button_options[1]->sprite2.y = ui_button_options[1]->sprite3.y = 61;
		}
		ui_text_options[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 619,465,237,38 }, { 0,0,100,100 }, "Close", Panel_Fade::panel_fade_in,{ 0,0,0,255 }));
		ui_text_options[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 583,212,237,38 }, { 0,0,100,100 }, "OPTIONS", Panel_Fade::panel_fade_in, { 255,255,255,255 }, { 1,0,0,0 }));
		ui_text_options[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 620,270,237,38 }, { 0,0,100,100 }, "FULLSCREEN", Panel_Fade::panel_fade_in, { 255,255,255,255 }));
		ui_text_volume_sliders[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 590,314,237,38 }, { 0,0,100,100 }, "MUSIC'S VOLUME", Panel_Fade::panel_fade_in,{ 255,255,255,255 }));
		ui_volume_sliders[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,340,(int)(181 * ((float)App->audio->GetVolumeMusic() / (float)128)),17 },
			{ 1072,250,181,17 }, "",Panel_Fade::panel_fade_in));
		ui_volume_sliders[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 531,334,215,30 }, { 1053,350,215,30 },"", Panel_Fade::panel_fade_in));
		ui_volume_sliders[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,337,36,24 }, { 1072,180,36,24 }, "VOLUME_CONTROL", Panel_Fade::panel_fade_in, { 0,0,0,0 },
			{ 0,0,0,0 }, true, { 548,337,181,0 }, App->audio, 0, false, ((float)App->audio->GetVolumeMusic() / (float)128)));
		ui_text_volume_sliders[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, ui_options_window, { 598,379,237,38 }, { 0,0,100,100 }, "FX'S VOLUME", Panel_Fade::panel_fade_in, { 255,255,255,255 }));
		ui_volume_sliders[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,405,(int)(181 * ((float)App->audio->GetVolumeFx() / (float)128)),17 },
			{ 1072,250,181,17 }, "", Panel_Fade::panel_fade_in));
		ui_volume_sliders[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 531,399,215,30 }, { 1053,350,215,30 },"", Panel_Fade::panel_fade_in));
		ui_volume_sliders[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_options_window, { 548,402,36,24 }, { 1072,180,36,24 }, "FX_CONTROL", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			true, { 548,402,181,0 }, App->audio, 0, false, ((float)App->audio->GetVolumeFx() / (float)128)));
	}
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking close button in options menu
void HUD::DeactivateOptionsMenu() {
	if (ui_options_window != nullptr) {
		App->gui->DeleteUIElement(ui_options_window);
		ui_options_window = nullptr;
		if (ui_pause_black_screen[1] != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen[1]);
			ui_pause_black_screen[1] = nullptr;
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
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}
}

// Called when clicking a button in the menu with confirmation message
void HUD::ActivateConfirmationMenu(std::string str) {
	if (ui_confirmation_window == nullptr) {
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150, Panel_Fade::panel_fade_in));
		ui_confirmation_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 },"", Panel_Fade::panel_fade_in));
		ui_button_confirmation[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 470,300,117,38 }, { 834,125,117,24 }, "YES", Panel_Fade::panel_fade_in, { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CONFIRMATION));
		ui_text_confirmation[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 509,306,237,38 }, { 0,0,100,100 }, "Yes", Panel_Fade::panel_fade_in, { 0,0,0,255 }, { 1,0,0,0 }));
		ui_button_confirmation[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 690,300,117,38 }, { 834,125,117,24 }, "NO", Panel_Fade::panel_fade_in, { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CONFIRMATION));
		ui_text_confirmation[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 731,306,237,38 }, { 0,0,100,100 }, "No", Panel_Fade::panel_fade_in, { 0,0,0,255 }, { 1,0,0,0 }));
		std::string text = str + " ?";
		ui_text_confirmation[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 463,212,237,38 }, { 0,0,100,100 }, "ARE YOU SURE YOU WANT TO", Panel_Fade::panel_fade_in, { 255,255,255,255 },
			{ 1,0,0,0 }));
		int size = text.length();
		ui_text_confirmation[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 640 - (6 * size),247,237,38 }, { 0,0,100,100 }, text, Panel_Fade::panel_fade_in, { 255,255,255,255 }, { 1,0,0,0 }));

	}
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = false;
		}
	}
}

// Called when clicking no in the confirmation message
void HUD::DeactivateConfirmationMenu() {
	confirmation_option.clear();
	if (ui_confirmation_window != nullptr) {
		App->gui->DeleteUIElement(ui_confirmation_window);
		ui_confirmation_window = nullptr;
		if (ui_pause_black_screen[1] != nullptr) {
			App->gui->DeleteUIElement(ui_pause_black_screen[1]);
			ui_pause_black_screen[1] = nullptr;
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
	for (int i = 0; i < 7; i++) {
		if (ui_button[i] != nullptr) {
			ui_button[i]->front = true;
		}
	}
}

// Called when selecting troops or buildings
void HUD::HUDUpdateSelection(std::list<Entity*> listEntities, Building* building_selected, bool update_list) {
	int i = 0;
	bool viking = true;
	if (App->scene->isInTutorial == true)
		App->tutorialscene->fortress_selected = false;
	HUDDeleteListTroops(update_list);
	HUDDeleteSelectedTroop();
	HUDDeleteActionButtons();
	for (std::list<Entity*>::iterator it = listEntities.begin(); it != listEntities.end(); ++it) {
		if (it._Ptr->_Myval->type == EntityType::UNIT) {
			Unit* unit = static_cast<Unit*>(it._Ptr->_Myval);
			if (i == 0) {
				number_of_troops[i]++;
				type_of_troops[i] = unit->unitType;
				i++;
			}
			else {
				bool new_troop = true;
				for (int j = 0; j < i; j++) {
					if (type_of_troops[j] == unit->unitType) {
						number_of_troops[j]++;
						new_troop = false;
					}
				}
				if (new_troop == true) {
					number_of_troops[i]++;
					type_of_troops[i] = unit->unitType;
					i++;
				}
			}
		}
	}
	listEntities=OrderSelectedList(listEntities, i);
	SDL_Rect r = { 825,618,30,41 };
	SDL_Rect r2 = { 828,645,17,9 };
	if (update_list == true) {
		for (int j = 0; j < i; j++) {

			if (j == 6) {
				r.y += 50;
				r2.y += 50;
				r.x = 825;
				r2.x = 828;
			}
			hud_list_troops[j] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, static_cast<UI*>(ui_ingame), r, GetSpritePortrait(1, type_of_troops[j]), "Troop",
				Panel_Fade::no_one_fade, GetSpritePortrait(3, type_of_troops[j]), GetSpritePortrait(2, type_of_troops[j]), false, { 0,0,0,0 }, App->scene, 0, false, -1.0F, 1));
			if (number_of_troops[j] > 1) {
				if (number_of_troops[j] < 10) {
					r2.x += 5;
				}
				hud_number_troops[j] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), r2, { 0,0,100,100 }, std::to_string(number_of_troops[j]), Panel_Fade::no_one_fade,
					{ 255,255,255,255 }));
				if (number_of_troops[j] < 10) {
					r2.x -= 5;
				}
			}
			r.x += 34;
			r2.x += 34;
		}
	}
	if (building_selected != nullptr && listEntities.empty()) {
		std::queue<std::string> production_queue = building_selected->GetProduction();
		int size_queue = production_queue.size();
		CivilizationType civ = building_selected->civilization;
		for (int j = 0; j < size_queue + 1; j++) {
			std::string thing_produced;
			if (j == 0)
				thing_produced = building_selected->GetElementProducing();
			else {
				thing_produced = production_queue.front();
				production_queue.pop();
			}
			if (j == 6) {
				r.y += 50;
				r.x = 825;
			}
			hud_list_troops[j] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, static_cast<UI*>(ui_ingame), r, GetSpritePortraitProduction(0, thing_produced, civ),
				"Thing_Produced", Panel_Fade::no_one_fade, GetSpritePortraitProduction(2, thing_produced, civ), GetSpritePortraitProduction(1, thing_produced, civ), false, { 0,0,0,0 },
				App->scene, 0, false, -1.0F, 1));
			r.x += 34;
		}
		if (building_selected->GetElementProducing() != "") {
			float w = building_selected->GetPercentage();
			hud_bar_producing[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 826,652,28,5 }, { 1072,319,181,17 }, "", Panel_Fade::no_one_fade));
			if (building_selected->researching == true && building_selected->index_researching == 0)
				hud_bar_producing[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 826,652,(int)(28 * w),5 }, { 1072,284,181,17 }, "", Panel_Fade::no_one_fade));
			else
				hud_bar_producing[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 826,652,(int)(28 * w),5 }, { 1072,250,181,17 }, "", Panel_Fade::no_one_fade));
		}
	}
	SDL_Rect position_name = { 725,603,30,30 };
	if (!listEntities.empty()) {
		if (listEntities.begin()._Ptr->_Myval->type == EntityType::UNIT) {
			thing_selected = listEntities.begin()._Ptr->_Myval;
			Unit* unit = static_cast<Unit*>(listEntities.begin()._Ptr->_Myval);
			hud_selected_troop = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 640,613,76,105 }, GetSpritePortrait(0, unit->unitType), "Troop", Panel_Fade::no_one_fade,
				{ 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1));
			hud_stats_selected_troop[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), position_name, { 0,0,100,100 }, "Unit", Panel_Fade::no_one_fade, { 0,0,0,255 }));
			bool combat_unit = false;
			switch (unit->unitType) {
			case UnitType::MONK:
				hud_stats_selected_troop[0]->SetString("Monk");
				type_thing_selected = Type_Selected::Monk;
				position_name.x += 18;
				hud_stats_selected_troop[0]->SetRect(position_name);
				break;
			case UnitType::CLERIC:
				hud_stats_selected_troop[0]->SetString("Cleric");
				type_thing_selected = Type_Selected::Cleric;
				position_name.x += 14;
				hud_stats_selected_troop[0]->SetRect(position_name);
				break;
			case UnitType::ASSASSIN:
				hud_stats_selected_troop[0]->SetString("Assassin");
				type_thing_selected = Type_Selected::Assassin;
				position_name.x += 6;
				hud_stats_selected_troop[0]->SetRect(position_name);
				combat_unit = true;
				break;
			case UnitType::PIKEMAN:
				hud_stats_selected_troop[0]->SetString("Pikeman");
				type_thing_selected = Type_Selected::Pikeman;
				combat_unit = true;
				break;
			case UnitType::MINOTAUR:
				hud_stats_selected_troop[0]->SetString("Minotaur");
				type_thing_selected = Type_Selected::Minotaur;
				position_name.x += 6;
				hud_stats_selected_troop[0]->SetRect(position_name);
				break;
			case UnitType::CYCLOP:
				hud_stats_selected_troop[0]->SetString("Cyclop");
				type_thing_selected = Type_Selected::Cyclop;
				position_name.x += 14;
				hud_stats_selected_troop[0]->SetRect(position_name);
				break;
			case UnitType::DRAUGAR:
				hud_stats_selected_troop[0]->SetString("Draugar");
				type_thing_selected = Type_Selected::Draugar;
				position_name.x += 12;
				hud_stats_selected_troop[0]->SetRect(position_name);
				break;
			case UnitType::JOTNAR:
				hud_stats_selected_troop[0]->SetString("Jotnar");
				type_thing_selected = Type_Selected::Jotnar;
				position_name.x += 14;
				hud_stats_selected_troop[0]->SetRect(position_name);
				break;
			}
			if (combat_unit == true) {
				CombatUnit* cunit = static_cast<CombatUnit*>(unit);
				hud_stats_selected_troop[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,622,30,30 }, { 0,0,100,100 }, "Damage:", Panel_Fade::no_one_fade,
					{ 0,0,0,255 }));
				hud_stats_selected_troop[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 780,622,30,30 }, { 0,0,100,100 },
					std::to_string(cunit->GetDamageValue()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				damage = cunit->GetDamageValue();
				hud_stats_selected_troop[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,636,30,30 }, { 0,0,100,100 }, "Range:", Panel_Fade::no_one_fade,
					{ 0,0,0,255 }));
				hud_stats_selected_troop[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 768,636,30,30 }, { 0,0,100,100 },
					std::to_string(cunit->GetRangeValue()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				range = cunit->GetRangeValue();
				hud_stats_selected_troop[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,650,30,30 }, { 0,0,100,100 }, "Speed:", Panel_Fade::no_one_fade,
					{ 0,0,0,255 }));
				hud_stats_selected_troop[6] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 760,650,30,30 }, { 0,0,100,100 },
					std::to_string(cunit->GetSpeedValue()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				speed = cunit->GetSpeedValue();
				hud_stats_selected_troop[7] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,664,30,30 }, { 0,0,100,100 }, "Level:", Panel_Fade::no_one_fade,
					{ 0,0,0,255 }));
				hud_stats_selected_troop[8] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 760,664,30,30 }, { 0,0,100,100 },
					std::to_string(cunit->GetLevel()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				level = cunit->GetLevel();
				hud_stats_selected_troop[9] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,678,30,30 }, { 0,0,100,100 }, "Exp:", Panel_Fade::no_one_fade,
					{ 0,0,0,255 }));
				hud_stats_selected_troop[10] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 747,678,30,30 }, { 0,0,100,100 },
					std::to_string(cunit->GetExperience()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				experience = cunit->GetExperience();
				hud_stats_selected_troop[11] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,703,30,10 }, { 0,0,100,100 }, "Health:", Panel_Fade::no_one_fade,
					{ 0,0,0,255 }));
				hud_stats_selected_troop[12] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 773,703,30,10 }, { 0,0,100,100 },
					std::to_string(unit->GetHealth()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				health = cunit->GetHealth();
			}
			else {
				hud_stats_selected_troop[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,703,30,10 }, { 0,0,100,100 }, "Health:", Panel_Fade::no_one_fade,
					{ 0,0,0,255 }));
				hud_stats_selected_troop[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 773,703,30,10 }, { 0,0,100,100 },
					std::to_string(unit->GetHealth()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				health = unit->GetHealth();
				if (unit->unitType == UnitType::CLERIC) {
					hud_stats_selected_troop[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,622,30,30 }, { 0,0,100,100 }, "Speed:", Panel_Fade::no_one_fade,
						{ 0,0,0,255 }));
					hud_stats_selected_troop[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 760,622,30,30 }, { 0,0,100,100 },
						std::to_string(100), Panel_Fade::no_one_fade, { 0,0,0,255 }));
					speed = 100;
				}
				else if(unit->unitType == UnitType::MINOTAUR||unit->unitType==UnitType::DRAUGAR) {
					hud_stats_selected_troop[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,622,30,10 }, { 0,0,100,100 }, "Health", Panel_Fade::no_one_fade,
						{ 0,0,0,255 }));
					hud_stats_selected_troop[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,636,30,10 }, { 0,0,100,100 }, "decreased:", Panel_Fade::no_one_fade,
						{ 0,0,0,255 }));
					hud_stats_selected_troop[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 791,636,30,10 }, { 0,0,100,100 },
						std::to_string(20), Panel_Fade::no_one_fade, { 0,0,0,255 }));
					hud_stats_selected_troop[6] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,664,30,30 }, { 0,0,100,100 }, "Range:", Panel_Fade::no_one_fade,
						{ 0,0,0,255 }));
					hud_stats_selected_troop[7] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 768,664,30,30 }, { 0,0,100,100 },
						std::to_string(3), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				}
				else if (unit->unitType == UnitType::JOTNAR || unit->unitType == UnitType::CYCLOP) {
					hud_stats_selected_troop[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,622,30,10 }, { 0,0,100,100 }, "Health", Panel_Fade::no_one_fade,
						{ 0,0,0,255 }));
					hud_stats_selected_troop[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,636,30,10 }, { 0,0,100,100 }, "raised:", Panel_Fade::no_one_fade,
						{ 0,0,0,255 }));
					hud_stats_selected_troop[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 767,636,30,10 }, { 0,0,100,100 },
						std::to_string(50), Panel_Fade::no_one_fade, { 0,0,0,255 }));
					hud_stats_selected_troop[6] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,664,30,30 }, { 0,0,100,100 }, "Range:", Panel_Fade::no_one_fade,
						{ 0,0,0,255 }));
					hud_stats_selected_troop[7] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 768,664,30,30 }, { 0,0,100,100 },
						std::to_string(1), Panel_Fade::no_one_fade, { 0,0,0,255 }));
				}
			}
		}
	}
	else if (building_selected != nullptr) {
		thing_selected = building_selected;
		hud_selected_troop = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 640,613,76,105 }, GetSpritePortraitBuilding(0,
			building_selected->GetBuildingType(), building_selected->civilization), "Building", Panel_Fade::no_one_fade, { 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1));
		hud_stats_selected_troop[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), position_name, { 0,0,100,100 }, "Building", Panel_Fade::no_one_fade, { 0,0,0,255 }));
		switch (building_selected->GetBuildingType()) {
		case BuildingType::FORTRESS:
			if (App->scene->isInTutorial == true)
				App->tutorialscene->fortress_selected = true;
			hud_stats_selected_troop[0]->SetString("Fortress");
			//position_name.x += 18;
			hud_stats_selected_troop[0]->SetRect(position_name);
			type_thing_selected = Type_Selected::Fortress;
			break;
		case BuildingType::MONASTERY:
			hud_stats_selected_troop[0]->SetString("Monastery");
			type_thing_selected = Type_Selected::Monastery;
			break;
		case BuildingType::TEMPLE:
			hud_stats_selected_troop[0]->SetString("Temple");
			type_thing_selected = Type_Selected::Temple;
			//position_name.x += 6;
			hud_stats_selected_troop[0]->SetRect(position_name);
			break;
		case BuildingType::ENCAMPMENT:
			hud_stats_selected_troop[0]->SetString("Encampment");
			type_thing_selected = Type_Selected::Encampment;
			break;
		}
		hud_stats_selected_troop[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,622,30,30 }, { 0,0,100,100 }, "Influence:", Panel_Fade::no_one_fade, { 0,0,0,255 }));
		hud_stats_selected_troop[2] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 790,622,30,30 }, { 0,0,100,100 },
			std::to_string(building_selected->GetInfluence()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
		influence = building_selected->GetInfluence();
		hud_stats_selected_troop[3] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,636,30,30 }, { 0,0,100,100 }, "Damage:", Panel_Fade::no_one_fade, { 0,0,0,255 }));
		hud_stats_selected_troop[4] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 778,636,30,30 }, { 0,0,100,100 },
			std::to_string(building_selected->GetDamage()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
		damage = building_selected->GetDamage();
		hud_stats_selected_troop[5] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,650,30,30 }, { 0,0,100,100 }, "Max Cap:", Panel_Fade::no_one_fade, { 0,0,0,255 }));
		hud_stats_selected_troop[6] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 780,650,30,30 }, { 0,0,100,100 },
			std::to_string(building_selected->GetMaxCap()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
		max_cap = building_selected->GetMaxCap();
		hud_stats_selected_troop[7] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 720,703,30,10 }, { 0,0,100,100 }, "Health:", Panel_Fade::no_one_fade, { 0,0,0,255 }));
		hud_stats_selected_troop[8] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, static_cast<UI*>(ui_ingame), { 773,703,30,10 }, { 0,0,100,100 },
			std::to_string(building_selected->GetHealth()), Panel_Fade::no_one_fade, { 0,0,0,255 }));
		health = building_selected->GetHealth();
		if (building_selected->civilization == CivilizationType::GREEK)
			viking = false;
	}
	ManageActionButtons(true, viking);
}


std::list<Entity*> HUD::OrderSelectedList(std::list<Entity*> list_selected, int different_types_of_units)
{
	if (different_types_of_units < 2)
		return list_selected;
	std::list<Entity*> list_to_return;
	for (int i = 0; i < different_types_of_units; i++) {
		int number_of_units_of_one_type = 0;
		int j = 0;
		for (std::list<Entity*>::iterator it = list_selected.begin(); it!=list_selected.end() && number_of_units_of_one_type < number_of_troops[i]; ++it) {
			if (j < i)
				j++;
			else if (it._Ptr->_Myval->type == EntityType::UNIT) {
				Unit* unit = static_cast<Unit*>(it._Ptr->_Myval);
				if (unit->unitType == type_of_troops[i])
					list_to_return.push_back(it._Ptr->_Myval);
			}
		}
	}
	App->entityManager->getPlayer()->SetEntitiesSelected(list_to_return);
	return list_to_return;
}

//Called when scrolling down
bool HUD::HUDScrollDown()
{
	std::list <Entity*> list_entities = App->entityManager->getPlayer()->GetEntitiesSelected();
	int number_of_units = number_of_troops[0];
	if (number_of_units > 1) {
		Entity* entity = list_entities.begin()._Ptr->_Myval;
		if (entity->type == EntityType::UNIT) {
			std::list<Entity*>::iterator it = list_entities.begin();
			std::list<Entity*>::iterator it2 = list_entities.begin();
			for (int i = 0; i < number_of_units; i++) {
				++it2;
				it._Ptr->_Myval = it2._Ptr->_Myval;
				if (i + 1 < number_of_units)
					++it;
			}
			it._Ptr->_Myval = entity;
			thing_selected = list_entities.begin()._Ptr->_Myval;
			App->entityManager->getPlayer()->SetEntitiesSelected(list_entities);
		}
	}
	if (number_of_units == 0)
		return false;
	else
		return true;
}

//Called when scrolling up
bool HUD::HUDScrollUp()
{
	std::list <Entity*> list_entities = App->entityManager->getPlayer()->GetEntitiesSelected();
	int number_of_units = number_of_troops[0];
	if (number_of_units > 1) {
		Entity* entity = list_entities.begin()._Ptr->_Myval;
		Entity* entity2;
		if (entity->type == EntityType::UNIT) {
			std::list<Entity*>::iterator it = list_entities.begin();
			for (int i = 1; i < number_of_units; i++) {
				++it;
				entity2 = it._Ptr->_Myval;
				it._Ptr->_Myval = entity;
				entity = entity2;
			}
			list_entities.begin()._Ptr->_Myval = entity;
			thing_selected = list_entities.begin()._Ptr->_Myval;
			App->entityManager->getPlayer()->SetEntitiesSelected(list_entities);
		}
	}
	if (number_of_units == 0)
		return false;
	else
		return true;
}

// Called when deleting the list of troops in the HUD
void HUD::HUDDeleteListTroops(bool delete_everything) {
	for (int i = 12; i >= 0; i--) {
		number_of_troops[i] = 0;
		if (i < 2) {
			if (hud_bar_producing[i] != nullptr) {
				App->gui->DeleteUIElement(hud_bar_producing[i]);
				hud_bar_producing[i] = nullptr;
			}
		}
		if (delete_everything) {
			if (hud_list_troops[i] != nullptr) {
				App->gui->DeleteUIElement(hud_list_troops[i]);
				hud_list_troops[i] = nullptr;
			}
			if (hud_number_troops[i] != nullptr) {
				App->gui->DeleteUIElement(hud_number_troops[i]);
				hud_number_troops[i] = nullptr;
			}
		}
	}
}

// Called when deleting the selected troop in the HUD
void HUD::HUDDeleteSelectedTroop() {
	thing_selected = nullptr;
	type_thing_selected = Type_Selected::None;
	for (int i = 12; i >= 0; i--) {
		if (hud_stats_selected_troop[i] != nullptr) {
			App->gui->DeleteUIElement(hud_stats_selected_troop[i]);
			hud_stats_selected_troop[i] = nullptr;
		}
	}
	if (hud_selected_troop != nullptr) {
		App->gui->DeleteUIElement(hud_selected_troop);
		hud_selected_troop = nullptr;
	}
}

// Called when deleting the selected troop's action buttons in the HUD
void HUD::HUDDeleteActionButtons() {
	for (int i = 8; i >= 0; i--) {
		if (hud_button_actions[i] != nullptr) {
			App->gui->DeleteUIElement(hud_button_actions[i]);
			hud_button_actions[i] = nullptr;
		}
		if (hud_button_actions_unclickable[i] != nullptr) {
			App->gui->DeleteUIElement(hud_button_actions_unclickable[i]);
			hud_button_actions_unclickable[i] = nullptr;
		}
	}
}

// Called to update every frame the information of the selected thing
void HUD::UpdateSelectedThing() {
	if (thing_selected->type == EntityType::UNIT) {
		if (hud_stats_selected_troop[9] != nullptr) {
			CombatUnit* cunit = static_cast<CombatUnit*>(thing_selected);
			if (damage != cunit->GetDamageValue()) {
				hud_stats_selected_troop[2]->SetString(std::to_string(cunit->GetDamageValue()));
				damage = cunit->GetDamageValue();
			}
			if (range != cunit->GetRangeValue()) {
				hud_stats_selected_troop[4]->SetString(std::to_string(cunit->GetRangeValue()));
				range = cunit->GetRangeValue();
			}
			if (speed != cunit->GetSpeedValue()) {
				hud_stats_selected_troop[6]->SetString(std::to_string(cunit->GetSpeedValue()));
				speed = cunit->GetSpeedValue();
			}
			if (level != cunit->GetLevel()) {
				hud_stats_selected_troop[8]->SetString(std::to_string(cunit->GetLevel()));
				level = cunit->GetLevel();
			}
			if (experience != cunit->GetExperience()) {
				hud_stats_selected_troop[10]->SetString(std::to_string(cunit->GetExperience()));
				experience = cunit->GetExperience();
			}
			if (health != cunit->GetHealth()) {
				hud_stats_selected_troop[12]->SetString(std::to_string(cunit->GetHealth()));
				health = cunit->GetHealth();
			}
		}
		else {
			Unit* unit = static_cast<Unit*>(thing_selected);
			if (health != unit->GetHealth()) {
				hud_stats_selected_troop[2]->SetString(std::to_string(unit->GetHealth()));
				health = unit->GetHealth();
			}
		}
	}
	else if (thing_selected->type == EntityType::BUILDING) {
		Building* building = static_cast<Building*>(thing_selected);
		if (influence != building->GetInfluence()) {
			hud_stats_selected_troop[2]->SetString(std::to_string(building->GetInfluence()));
			influence = building->GetInfluence();
		}
		if (damage != building->GetDamage()) {
			hud_stats_selected_troop[4]->SetString(std::to_string(building->GetDamage()));
			damage = building->GetDamage();
		}
		if (max_cap != building->GetMaxCap()) {
			hud_stats_selected_troop[6]->SetString(std::to_string(building->GetMaxCap()));
			max_cap = building->GetMaxCap();
		}
		if (health != building->GetHealth()) {
			hud_stats_selected_troop[8]->SetString(std::to_string(building->GetHealth()));
			health = building->GetHealth();
		}
		if (App->scene->update_production_list) {
			HUDDeleteListTroops();
			std::queue<std::string> production_queue = building->GetProduction();
			if (!production_queue.empty() || building->GetElementProducing() != "") {
				SDL_Rect r = { 825,618,30,41 };
				int size_queue = production_queue.size();
				CivilizationType civ = building->civilization;
				for (int j = 0; j < size_queue+1; j++) {
					std::string thing_produced;
					if (j == 0)
						thing_produced = building->GetElementProducing();
					else {
						thing_produced = production_queue.front();
						production_queue.pop();
					}
					if (j == 6) {
						r.y += 50;
						r.x = 825;
					}
					hud_list_troops[j] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, static_cast<UI*>(ui_ingame), r, GetSpritePortraitProduction(0, thing_produced, civ),
						"Thing_Produced", Panel_Fade::no_one_fade, GetSpritePortraitProduction(2, thing_produced, civ), GetSpritePortraitProduction(1, thing_produced, civ), false, { 0,0,0,0 },
						App->scene, 0, false, -1.0F, 1));
					r.x += 34;
				}
				if (building->GetElementProducing() != "") {
					hud_bar_producing[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 826,652,28,5 }, { 1072,319,181,17 }, "", Panel_Fade::no_one_fade));
					if (building->researching == true && building->index_researching == 0)
						hud_bar_producing[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 826,652,28,5 }, { 1072,284,181,17 }, "", Panel_Fade::no_one_fade));
					else
						hud_bar_producing[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, static_cast<UI*>(ui_ingame), { 826,652,28,5 }, { 1072,250,181,17 }, "", Panel_Fade::no_one_fade));
				}
			}
			App->scene->update_production_list = false;
		}
		if (hud_bar_producing[1] != nullptr) {
			float w = building->GetPercentage();
			hud_bar_producing[1]->quad.w = 28 * w;
		}
	}
}

// Called when clicking a thing in the production queue of a building
void HUD::CancelProduction(iPoint position)
{
	int index = 0;
	switch (position.x) {
	case 825:
		index = 0;
		break;
	case 859:
		index = 1;
		break;
	case 893:
		index = 2;
		break;
	case 927:
		index = 3;
		break;
	case 961:
		index = 4;
		break;
	case  995:
		index = 5;
		break;
	}
	if (position.y == 668)
		index += 6;
	if (thing_selected->type == EntityType::BUILDING) {
		Building* building = static_cast<Building*>(thing_selected);
		building->CancelProduction(index);
	}
}

// Called when clicked on one of the buttons of the right after selecting troops
void HUD::ClickOnSelectionButton(SDL_Rect sprite)
{
	UnitType unit_type = GetUnitTypeFromSpritePortrait(sprite);
	if (thing_selected->type == EntityType::UNIT) {
		Unit* unit = static_cast<Unit*>(thing_selected);
		int index = 0;
		int total = 0;
		for (int i = 0; i < 12 && type_of_troops[i] != unit_type; i++) {
			total += number_of_troops[i];
			index++;
		}
		if (index < 12) {
			if (unit_type == unit->unitType)
				DeleteTroopsInSelectedList(index, total);
			else
				ChangeTypeOfUnitFirstSelected(index, total);
		}
	}
}

// Called when changing the type of unit appearing in the central part of the HUD
void HUD::ChangeTypeOfUnitFirstSelected(int index, int number_of_troops_before)
{
	std::list<Entity*> entity_list = App->entityManager->getPlayer()->GetEntitiesSelected();
	for (int i = 0; i < number_of_troops[index]; i++) {
		std::list<Entity*>::iterator it = entity_list.begin();
		for (int j = 1; j < number_of_troops_before+ number_of_troops[index]; j++) {
			++it;
		}
		Entity* entity = it._Ptr->_Myval;
		entity_list.erase(it);
		entity_list.push_front(entity);
	}
	App->entityManager->getPlayer()->SetEntitiesSelected(entity_list);
	App->scene->update_selection = true;
	App->scene->dont_update_types_of_troops = false;
}

// Called when deleting a type of troop in the selected list
void HUD::DeleteTroopsInSelectedList(int index, int number_of_troops_before)
{
	std::list<Entity*> entity_list = App->entityManager->getPlayer()->GetEntitiesSelected();
	for (int i = 0; i < number_of_troops[index]; i++) {
		std::list<Entity*>::iterator it = entity_list.begin();
		for (int i = 0; i < number_of_troops_before; i++) {
			++it;
		}
		it._Ptr->_Myval->SetSelected(false);
		entity_list.erase(it);
	}
	App->entityManager->getPlayer()->SetEntitiesSelected(entity_list);
	App->scene->update_selection = true;
}

//Called when creating or updating the action buttons
void HUD::ManageActionButtons(bool create_buttons, bool viking) {
	Player* player = App->entityManager->getPlayer();
	if (create_buttons == true) {
		switch (type_thing_selected) {
		case Type_Selected::Assassin:
		{
			CombatUnit* cunit = static_cast<CombatUnit*>(thing_selected);
			int exp_needed = cunit->GetExperienceToLevel(cunit->GetLevel());
			hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,36,36 }, { 16,185,36,36 }, "Move", Panel_Fade::no_one_fade, { 98,185,36,36 }, { 57,185,36,36 },
				false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::movebutton));
			hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 241,613,36,36 }, { 16,143,36,36 }, "Attack", Panel_Fade::no_one_fade, { 98,143,36,36 }, { 57,143,36,36 },
				false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::attackbutton));
			if (exp_needed > cunit->GetExperience()) {
				hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 282,613,36,36 }, { 16,102,36,36 }, "Upgrade", Panel_Fade::no_one_fade, { 98,102,36,36 },
					{ 57,102,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::levelupbutton_assassin));
			}
			else {
				hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 282,613,36,36 }, { 98,442,36,36 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 }, { 0,0,0,0 },
					false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 0, (int)TooltipsAvailable::levelupbutton_assassin));
			}
			break;
		}
		case Type_Selected::Monk:
			hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 241,613,36,36 }, { 16,529,36,36 }, "Heal", Panel_Fade::no_one_fade, { 98,529,36,36 }, { 57,529,36,36 },
				false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::healbutton));
		case Type_Selected::Cleric:
		case Type_Selected::Jotnar:
		case Type_Selected::Minotaur:
		case Type_Selected::Cyclop:
		case Type_Selected::Draugar:
			hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,36,36 }, { 16,185,36,36 }, "Move", Panel_Fade::no_one_fade, { 98,185,36,36 }, { 57,185,36,36 },
				false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::movebutton));
			break;
		case Type_Selected::Fortress:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (viking == true) {
					if (player->GetFaith() >= 200 && player->num_temple < 8 && player->research_temple == true) {
						hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,604,99,60 }, { 2,2,99,60 }, "Produce_Temple", Panel_Fade::no_one_fade, { 2,124,99,60 },
							{ 2,63,99,60 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::temple));
					}
					else {
						if (player->research_temple == false) {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,604,99,60 }, { 329,313,99,60 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,604,99,60 }, { 329,313,99,60 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::temple));
						}
					}
					if (player->GetFaith() >= 200 && player->num_encampment < 7 && player->research_encampment == true) {
						hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 104,11,99,51 }, "Produce_Encampment", Panel_Fade::no_one_fade,
							{ 104,133,99,51 }, { 104,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
					}
					else {
						if (player->research_encampment == false) {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,322,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,322,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
						}
					}
					if (player->GetFaith() >= 200 && player->num_monastery < 5) {
						hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 206,10,99,52 }, "Produce_Monastery", Panel_Fade::no_one_fade,
							{ 206,132,99,52 }, { 206,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					else {
						hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,321,99,52 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					if (player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_lawful_beast == true) {
						hud_button_actions[7] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,672,67,41 }, { 434,540,67,41 }, "Produce_Lawful_Beast", Panel_Fade::no_one_fade,
							{ 434,634,67,41 }, { 434,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::jotnar_unlocked));
					}
					else {
						if (player->research_lawful_beast == false) {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 434,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 434,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::jotnar_unlocked));
						}
					}
					if (player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_chaotic_beast == true) {
						hud_button_actions[8] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,672,67,41 }, { 504,540,67,41 }, "Produce_Chaotic_Beast", Panel_Fade::no_one_fade,
							{ 504,634,67,41 }, { 504,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::draugar_unlocked));
					}
					else {
						if (player->research_chaotic_beast == false) {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 504,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 504,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::draugar_unlocked));
						}
					}
				}
				else {
					if (player->GetFaith() >= 200 && player->num_temple < 8 && player->research_temple == true) {
						hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,602,99,62 }, { 308,0,99,62 }, "Produce_Temple", Panel_Fade::no_one_fade,
							{ 308,122,99,62 }, { 308,61,99,62 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::temple));
					}
					else {
						if (player->research_temple == false) {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,602,99,62 }, { 329,246,99,62 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,602,99,62 }, { 329,246,99,62 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::temple));
						}
					}
					if (player->GetFaith() >= 200 && player->num_encampment < 7 && player->research_encampment == true) {
						hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 410,11,99,51 }, "Produce_Encampment", Panel_Fade::no_one_fade,
							{ 410,133,99,51 }, { 410,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
					}
					else {
						if (player->research_encampment == false) {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,257,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,257,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
						}
					}
					if (player->GetFaith() >= 200 && player->num_monastery < 5) {
						hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 512,10,99,52 }, "Produce_Monastery", Panel_Fade::no_one_fade,
							{ 512,132,99,52 }, { 512,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					else {
						hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,256,99,52 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					if (player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_lawful_beast == true) {
						hud_button_actions[7] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,672,67,41 }, { 363,540,67,41 }, "Produce_Lawful_Beast", Panel_Fade::no_one_fade,
							{ 363,634,67,41 }, { 363,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::cyclop_unlocked));
					}
					else {
						if (player->research_lawful_beast == false) {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 363,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 363,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::cyclop_unlocked));
						}
					}
					if (player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_chaotic_beast == true) {
						hud_button_actions[8] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,672,67,41 }, { 293,540,67,41 }, "Produce_Chaotic_Beast", Panel_Fade::no_one_fade,
							{ 293,634,67,41 }, { 293,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::minotaur_unlocked));
					}
					else {
						if (player->research_chaotic_beast == false) {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 293,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 293,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::minotaur_unlocked));
						}
					}
				}
				if (player->GetFaith() >= 600 && building->GetProduction().size() < 11 && (player->research_chaotic_victory == true || player->research_lawful_victory == true)) {
					if (player->research_chaotic_victory == true) {
						hud_button_actions[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 512,621,67,41 }, { 75,540,67,41 }, "Produce_Victory", Panel_Fade::no_one_fade,
							{ 75,632,67,41 }, { 75,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::victory2_unlocked));
					}
					else {
						hud_button_actions[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 512,621,67,41 }, { 75,540,67,41 }, "Produce_Victory", Panel_Fade::no_one_fade,
							{ 75,632,67,41 }, { 75,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::victory1_unlocked));
					}
				}
				else {
					if (player->research_chaotic_victory == false && player->research_lawful_victory == false) {
						hud_button_actions_unclickable[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::victorylocked));
					}
					else if (player->research_chaotic_victory == true) {
						hud_button_actions_unclickable[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::victory2_unlocked));
					}
					else {
						hud_button_actions_unclickable[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::victory1_unlocked));
					}

				}
				if (player->GetFaith() >= 300 && building->GetProduction().size() < 11 && player->research_lawful_miracle == true) {
					hud_button_actions[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 344,672,67,41 }, { 219,540,67,41 }, "Produce_Lawful_Miracle", Panel_Fade::no_one_fade,
						{ 219,632,67,41 }, { 219,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::miracle_unlocked));
				}
				else {
					if (player->research_lawful_miracle == false) {
						hud_button_actions_unclickable[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,672,67,41 }, { 219,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::miraclelocked));
					}
					else {
						hud_button_actions_unclickable[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,672,67,41 }, { 219,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::miracle_unlocked));
					}
				}
				if (player->GetFaith() >= 300 && building->GetProduction().size() < 11 && player->research_chaotic_miracle == true) {
					hud_button_actions[6] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 416,672,67,41 }, { 412,726,67,41 }, "Produce_Chaotic_Miracle", Panel_Fade::no_one_fade,
						{ 412,820,67,41 }, { 412,773,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::disaster_unlocked));
				}
				else {
					if (player->research_chaotic_miracle == false) {
						hud_button_actions_unclickable[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 416,672,67,41 }, { 412,867,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::disasterlocked));
					}
					else {
						hud_button_actions_unclickable[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 416,672,67,41 }, { 412,867,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::disaster_unlocked));
					}
				}
				if (building->researching == false && building->GetProduction().size() < 11) {
					hud_button_actions[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 584,626,36,36 }, { 16,227,36,36 }, "Research", Panel_Fade::no_one_fade, { 98,227,36,36 },
						{ 57,227,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::researchbutton));
				}
				else {
					hud_button_actions_unclickable[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 584,626,36,36 }, { 98,484,36,36 }, "", Panel_Fade::no_one_fade,{ 0,0,0,0 },
						{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 0, (int)TooltipsAvailable::researchbutton));
				}
			}
			break;
		}
		case Type_Selected::Temple:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (player->GetFaith() >= 150 && building->GetProduction().size() < 11 && player->research_cleric == true) {
					hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 574,540,67,41 }, "Produce_Cleric", Panel_Fade::no_one_fade, { 574,634,67,41 },
						{ 574,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::cleric_unlocked));
				}
				else {
					if (player->research_cleric == false) {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 574,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
					}
					else {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 574,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::cleric_unlocked));
					}
				}
			}
			break;
		}
		case Type_Selected::Encampment:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (player->GetFaith() >= 100 && player->research_assassin == true && building->GetProduction().size() < 11) {
					hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 215,185,67,41 }, "Produce_Assassin", Panel_Fade::no_one_fade,
						{ 215,275,67,41 }, { 215,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::assassin_unlocked));
				}
				else {
					if (player->research_assassin == false) {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 549,382,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
					}
					else {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 549,382,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::assassin_unlocked));
					}
				}
			}
			break;
		}
		case Type_Selected::Monastery:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (player->GetFaith() >= 50 && building->GetProduction().size() < 11) {
					hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 73,185,67,41 }, "Produce_Monk", Panel_Fade::no_one_fade, { 73,275,67,41 },
						{ 73,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::monk));
				}
				else {
					hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 407,382,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
						{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::monk));
				}
				if (player->GetFaith() >= 40 && building->GetProduction().size() < 11) {
					hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,613,67,41 }, { 3,540,67,41 }, "Produce_Sacrifices", Panel_Fade::no_one_fade, { 3,632,67,41 },
						{ 3,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
				else {
					hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,613,67,41 }, { 3,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 }, { 0,0,0,0 },
						false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
				if (player->GetFaith() >= 40 && building->GetProduction().size() < 11) {
					hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 344,613,67,41 }, { 146,540,67,41 }, "Produce_Prayers", Panel_Fade::no_one_fade, { 146,632,67,41 },
						{ 146,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
				else {
					hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,613,67,41 }, { 146,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
						{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
			}
			break;
		}
		}
	}
	else {
		switch (type_thing_selected) {
		case Type_Selected::Assassin:
		{
			CombatUnit* cunit = static_cast<CombatUnit*>(thing_selected);
			int exp_needed = cunit->GetExperienceToLevel(cunit->GetLevel());
			if (hud_button_actions[2] == nullptr && exp_needed <= cunit->GetExperience()) {
				if (hud_button_actions_unclickable[2] != nullptr) {
					App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
					hud_button_actions_unclickable[2] = nullptr;
				}
				hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 282,613,36,36 }, { 16,102,36,36 }, "Upgrade", Panel_Fade::no_one_fade, { 98,102,36,36 },
					{ 57,102,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::levelupbutton_assassin));
			}
			else if (hud_button_actions_unclickable[2] == nullptr && exp_needed > cunit->GetExperience()) {
				if (hud_button_actions[2] != nullptr) {
					App->gui->DeleteUIElement(hud_button_actions[2]);
					hud_button_actions[2] = nullptr;
				}
				hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 282,613,36,36 }, { 98,442,36,36 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 }, { 0,0,0,0 },
					false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 0, (int)TooltipsAvailable::levelupbutton_assassin));
			}
			break;
		}
		case Type_Selected::Fortress:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (viking == true) {
					if (hud_button_actions[0] == nullptr && player->GetFaith() >= 200 && player->num_temple < 8 && player->research_temple == true) {
						if (hud_button_actions_unclickable[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
							hud_button_actions_unclickable[0] = nullptr;
						}
						hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,604,99,60 }, { 2,2,99,60 }, "Produce_Temple", Panel_Fade::no_one_fade, { 2,124,99,60 },
							{ 2,63,99,60 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::temple));
					}
					else if (hud_button_actions_unclickable[0] == nullptr && (player->GetFaith() < 200 || player->num_temple >= 8 || player->research_temple == false)) {
						if (hud_button_actions[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[0]);
							hud_button_actions[0] = nullptr;
						}
						if (player->research_temple == false) {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,604,99,60 }, { 329,313,99,60 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,604,99,60 }, { 329,313,99,60 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::temple));
						}
					}
					if (hud_button_actions[1] == nullptr && player->GetFaith() >= 200 && player->num_encampment < 7 && player->research_encampment == true) {
						if (hud_button_actions_unclickable[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[1]);
							hud_button_actions_unclickable[1] = nullptr;
						}
						hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 104,11,99,51 }, "Produce_Encampment", Panel_Fade::no_one_fade,
							{ 104,133,99,51 }, { 104,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
					}
					else if (hud_button_actions_unclickable[1] == nullptr && (player->GetFaith() < 200 || player->num_encampment >= 7 || player->research_encampment == false)) {
						if (hud_button_actions[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[1]);
							hud_button_actions[1] = nullptr;
						}
						if (player->research_encampment == false) {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,322,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,322,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
						}
					}
					if (hud_button_actions[2] == nullptr && player->GetFaith() >= 200 && player->num_monastery < 5) {
						if (hud_button_actions_unclickable[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
							hud_button_actions_unclickable[2] = nullptr;
						}
						hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 206,10,99,52 }, "Produce_Monastery", Panel_Fade::no_one_fade,
							{ 206,132,99,52 }, { 206,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					else if (hud_button_actions_unclickable[2] == nullptr && (player->GetFaith() < 200 || player->num_monastery >= 5)) {
						if (hud_button_actions[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[2]);
							hud_button_actions[2] = nullptr;
						}
						hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,321,99,52 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					if (hud_button_actions[7] == nullptr && player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_lawful_beast == true) {
						if (hud_button_actions_unclickable[7] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[7]);
							hud_button_actions_unclickable[7] = nullptr;
						}
						hud_button_actions[7] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,672,67,41 }, { 434,540,67,41 }, "Produce_Lawful_Beast", Panel_Fade::no_one_fade,
							{ 434,634,67,41 }, { 434,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::jotnar_unlocked));
					}
					else if (hud_button_actions_unclickable[7] == nullptr && (player->GetFaith() < 200 || building->GetProduction().size() >= 11 ||
						player->research_lawful_beast != true)) {
						if (hud_button_actions[7] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[7]);
							hud_button_actions[7] = nullptr;
						}
						if (player->research_lawful_beast) {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 434,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 434,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::jotnar_unlocked));
						}
					}
					if (hud_button_actions[8] == nullptr && player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_chaotic_beast == true) {
						if (hud_button_actions_unclickable[7] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[7]);
							hud_button_actions_unclickable[7] = nullptr;
						}
						hud_button_actions[8] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,672,67,41 }, { 504,540,67,41 }, "Produce_Chaotic_Beast", Panel_Fade::no_one_fade,
							{ 504,634,67,41 }, { 504,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::draugar_unlocked));
					}
					else if (hud_button_actions_unclickable[8] == nullptr && (player->GetFaith() < 200 || building->GetProduction().size() >= 11 ||
						player->research_chaotic_beast != true)) {
						if (hud_button_actions[8] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[8]);
							hud_button_actions[8] = nullptr;
						}
						if (player->research_chaotic_beast == false) {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 504,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 504,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::draugar_unlocked));
						}
					}
				}
				else {
					if (hud_button_actions[0] == nullptr && player->GetFaith() >= 200 && player->num_temple < 8 && player->research_temple == true) {
						if (hud_button_actions_unclickable[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
							hud_button_actions_unclickable[0] = nullptr;
						}
						hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,602,99,62 }, { 308,0,99,62 }, "Produce_Temple", Panel_Fade::no_one_fade,
							{ 308,122,99,62 }, { 308,61,99,62 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::temple));
					}
					else if (hud_button_actions_unclickable[0] == nullptr && (player->GetFaith() < 200 || player->num_temple >= 8 || player->research_temple == false)) {
						if (hud_button_actions[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[0]);
							hud_button_actions[0] = nullptr;
						}
						if (player->research_temple == false) {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,602,99,62 }, { 329,246,99,62 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,602,99,62 }, { 329,246,99,62 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::temple));
						}
					}
					if (hud_button_actions[1] == nullptr && player->GetFaith() >= 200 && player->num_encampment < 7 && player->research_encampment == true) {
						if (hud_button_actions_unclickable[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[1]);
							hud_button_actions_unclickable[1] = nullptr;
						}
						hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 410,11,99,51 }, "Produce_Encampment", Panel_Fade::no_one_fade,
							{ 410,133,99,51 }, { 410,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
					}
					else if (hud_button_actions_unclickable[1] == nullptr && (player->GetFaith() < 200 || player->num_encampment >= 7 || player->research_encampment == false)) {
						if (hud_button_actions[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[1]);
							hud_button_actions[1] = nullptr;
						}
						if (player->research_encampment == false) {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,257,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::buildingconstructionlocked));
						}
						else {
							hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,257,99,51 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::encampment));
						}
					}
					if (hud_button_actions[2] == nullptr && player->GetFaith() >= 200 && player->num_monastery < 5) {
						if (hud_button_actions_unclickable[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
							hud_button_actions_unclickable[2] = nullptr;
						}
						hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 512,10,99,52 }, "Produce_Monastery", Panel_Fade::no_one_fade,
							{ 512,132,99,52 }, { 512,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					else if (hud_button_actions_unclickable[2] == nullptr && (player->GetFaith() < 200 || player->num_monastery >= 5)) {
						if (hud_button_actions[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[2]);
							hud_button_actions[2] = nullptr;
						}
						hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,256,99,52 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::monastery));
					}
					if (hud_button_actions[7] == nullptr && player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_lawful_beast == true) {
						if (hud_button_actions_unclickable[7] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[7]);
							hud_button_actions_unclickable[7] = nullptr;
						}
						hud_button_actions[7] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,672,67,41 }, { 363,540,67,41 }, "Produce_Lawful_Beast", Panel_Fade::no_one_fade,
							{ 363,634,67,41 }, { 363,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::cyclop_unlocked));
					}
					else if (hud_button_actions_unclickable[7] == nullptr && (player->GetFaith() < 200 || building->GetProduction().size() >= 11 ||
						player->research_lawful_beast != true)) {
						if (hud_button_actions[7] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[7]);
							hud_button_actions[7] = nullptr;
						}
						if (player->research_lawful_beast == false) {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 363,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,672,67,41 }, { 363,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::cyclop_unlocked));
						}
					}
					if (hud_button_actions[8] == nullptr && player->GetFaith() >= 200 && building->GetProduction().size() < 11 && player->research_chaotic_beast == true) {
						if (hud_button_actions_unclickable[7] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[7]);
							hud_button_actions_unclickable[7] = nullptr;
						}
						hud_button_actions[8] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,672,67,41 }, { 293,540,67,41 }, "Produce_Chaotic_Beast", Panel_Fade::no_one_fade,
							{ 293,634,67,41 }, { 293,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::minotaur_unlocked));
					}
					else if (hud_button_actions_unclickable[8] == nullptr && (player->GetFaith() < 200 || building->GetProduction().size() >= 11 ||
						player->research_chaotic_beast != true)) {
						if (hud_button_actions[8] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[8]);
							hud_button_actions[8] = nullptr;
						}
						if (player->research_chaotic_beast == false) {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 293,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
						}
						else {
							hud_button_actions_unclickable[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,672,67,41 }, { 293,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
								{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::minotaur_unlocked));
						}
					}
				}
				if (hud_button_actions[3] == nullptr && player->GetFaith() >= 600 && building->GetProduction().size() < 11 && (player->research_chaotic_victory == true ||
					player->research_lawful_victory == true)) {
					if (hud_button_actions_unclickable[3] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[3]);
						hud_button_actions_unclickable[3] = nullptr;
					}
					if (player->research_chaotic_victory == true) {
						hud_button_actions[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 512,621,67,41 }, { 75,540,67,41 }, "Produce_Victory", Panel_Fade::no_one_fade,
							{ 75,632,67,41 }, { 75,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::victory2_unlocked));
					}
					else if (player->research_lawful_victory == true) {
						hud_button_actions[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 512,621,67,41 }, { 75,540,67,41 }, "Produce_Victory", Panel_Fade::no_one_fade,
							{ 75,632,67,41 }, { 75,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::victory1_unlocked));
					}
				}
				else if (hud_button_actions_unclickable[3] == nullptr && (player->GetFaith() < 600 || building->GetProduction().size() >= 11)) {
					if (hud_button_actions[3] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[3]);
						hud_button_actions[3] = nullptr;
					}
					if (player->research_chaotic_victory == false && player->research_lawful_victory == false) {
						hud_button_actions_unclickable[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::victorylocked));
					}
					else if (player->research_chaotic_victory == true) {
						hud_button_actions_unclickable[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::victory2_unlocked));
					}
					else {
						hud_button_actions_unclickable[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::victory1_unlocked));
					}
				}
				if (hud_button_actions[4] == nullptr && building->researching == false && building->GetProduction().size() < 11) {
					if (hud_button_actions_unclickable[4] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[4]);
						hud_button_actions_unclickable[4] = nullptr;
					}
					hud_button_actions[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 584,626,36,36 }, { 16,227,36,36 }, "Research", Panel_Fade::no_one_fade, { 98,227,36,36 },
						{ 57,227,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0f, 0, (int)TooltipsAvailable::researchbutton));
				}
				else if (hud_button_actions_unclickable[4] == nullptr && (building->researching == true || building->GetProduction().size() >= 11)) {
					if (hud_button_actions[4] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[4]);
						hud_button_actions[4] = nullptr;
					}
					hud_button_actions_unclickable[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 584,626,36,36 }, { 98,484,36,36 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
						{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 0, (int)TooltipsAvailable::researchbutton));
				}
				if (hud_button_actions[5] == nullptr && player->GetFaith() >= 300 && building->GetProduction().size() < 11 && player->research_lawful_miracle == true) {
					if (hud_button_actions_unclickable[5] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[5]);
						hud_button_actions_unclickable[5] = nullptr;
					}
					hud_button_actions[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 344,672,67,41 }, { 219,540,67,41 }, "Produce_Lawful_Miracle", Panel_Fade::no_one_fade,
						{ 219,632,67,41 }, { 219,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::miracle_unlocked));
				}
				else if (hud_button_actions_unclickable[5] == nullptr && (player->GetFaith() < 300 || building->GetProduction().size() >= 11 ||
					player->research_lawful_miracle != true)) {
					if (hud_button_actions[5] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[5]);
						hud_button_actions[5] = nullptr;
					}
					if (player->research_lawful_miracle == false) {
						hud_button_actions_unclickable[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,672,67,41 }, { 219,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::miraclelocked));
					}
					else {
						hud_button_actions_unclickable[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,672,67,41 }, { 219,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::miracle_unlocked));
					}
				}
				if (hud_button_actions[6] == nullptr && player->GetFaith() >= 300 && building->GetProduction().size() < 11 && player->research_chaotic_miracle == true) {
					if (hud_button_actions_unclickable[6] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[6]);
						hud_button_actions_unclickable[6] = nullptr;
					}
					hud_button_actions[6] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 416,672,67,41 }, { 412,726,67,41 }, "Produce_Chaotic_Miracle", Panel_Fade::no_one_fade,
						{ 412,820,67,41 }, { 412,773,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::disaster_unlocked));
				}
				else if (hud_button_actions_unclickable[6] == nullptr && (player->GetFaith() < 300 || building->GetProduction().size() >= 11 ||
					player->research_chaotic_miracle != true)) {
					if (hud_button_actions[6] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[6]);
						hud_button_actions[6] = nullptr;
					}
					if (player->research_chaotic_miracle == false) {
						hud_button_actions_unclickable[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 416,672,67,41 }, { 412,867,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::disasterlocked));
					}
					else {
						hud_button_actions_unclickable[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 416,672,67,41 }, { 412,867,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::disaster_unlocked));
					}
				}
			}
			break;
		}
		case Type_Selected::Encampment:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (hud_button_actions[0] == nullptr && player->GetFaith() >= 100 && player->research_assassin == true &&
					building->GetProduction().size() < 11) {
					if (hud_button_actions_unclickable[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
						hud_button_actions_unclickable[0] = nullptr;
					}
					hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 215,185,67,41 }, "Produce_Assassin", Panel_Fade::no_one_fade,
						{ 215,275,67,41 }, { 215,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::assassin_unlocked));
				}
				else if (hud_button_actions_unclickable[0] == nullptr && (player->GetFaith() < 100 || player->research_assassin == false ||
					building->GetProduction().size() >= 11)) {
					if (hud_button_actions[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[0]);
						hud_button_actions[0] = nullptr;
					}
					if (player->research_assassin == false) {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 549,382,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
					}
					else {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 549,382,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::assassin_unlocked));
					}
				}
			}
			break;
		}
		case Type_Selected::Monastery:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (hud_button_actions[0] == nullptr && player->GetFaith() >= 50 && building->GetProduction().size() < 11) {
					if (hud_button_actions_unclickable[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
						hud_button_actions_unclickable[0] = nullptr;
					}
					hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 73,185,67,41 }, "Produce_Monk", Panel_Fade::no_one_fade, { 73,275,67,41 },
						{ 73,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::monk));
				}
				else if (hud_button_actions_unclickable[0] == nullptr && (player->GetFaith() < 50 || building->GetProduction().size() >= 11)) {
					if (hud_button_actions[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[0]);
						hud_button_actions[0] = nullptr;
					}
					hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 407,382,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
						{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::monk));
				}
				if (hud_button_actions[1] == nullptr && player->GetFaith() >= 40 && building->GetProduction().size() < 11) {
					if (hud_button_actions_unclickable[1] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[1]);
						hud_button_actions_unclickable[1] = nullptr;
					}
					hud_button_actions[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,613,67,41 }, { 3,540,67,41 }, "Produce_Sacrifices", Panel_Fade::no_one_fade, { 3,632,67,41 },
						{ 3,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
				else if (hud_button_actions_unclickable[1] == nullptr && (player->GetFaith() < 40 || building->GetProduction().size() >= 11)) {
					if (hud_button_actions[1] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[1]);
						hud_button_actions[1] = nullptr;
					}
					hud_button_actions_unclickable[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,613,67,41 }, { 3,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 }, { 0,0,0,0 },
						false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
				if (hud_button_actions[2] == nullptr && player->GetFaith() >= 40 && building->GetProduction().size() < 11) {
					if (hud_button_actions_unclickable[2] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
						hud_button_actions_unclickable[2] = nullptr;
					}
					hud_button_actions[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 344,613,67,41 }, { 146,540,67,41 }, "Produce_Prayers", Panel_Fade::no_one_fade, { 146,632,67,41 },
						{ 146,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
				else if (hud_button_actions_unclickable[2] == nullptr && (player->GetFaith() < 40 || building->GetProduction().size() >= 11)) {
					if (hud_button_actions[2] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[2]);
						hud_button_actions[2] = nullptr;
					}
					hud_button_actions_unclickable[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,613,67,41 }, { 146,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
						{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::producecurrencybutton));
				}
			}
			break;
		}
		case Type_Selected::Temple:
		{
			Building* building = static_cast<Building*>(thing_selected);
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (hud_button_actions[0] == nullptr && player->GetFaith() >= 150 && building->GetProduction().size() < 11 && player->research_cleric == true) {
					if (hud_button_actions_unclickable[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
						hud_button_actions_unclickable[0] = nullptr;
					}
					hud_button_actions[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 574,540,67,41 }, "Produce_Cleric", Panel_Fade::no_one_fade, { 574,634,67,41 },
						{ 574,587,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1, (int)TooltipsAvailable::cleric_unlocked));
				}
				else if (hud_button_actions_unclickable[0] == nullptr && (player->GetFaith() < 150 || building->GetProduction().size() >= 11 ||
					player->research_cleric != true)) {
					if (hud_button_actions[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[0]);
						hud_button_actions[0] = nullptr;
					}
					if (player->research_cleric == false) {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 574,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::unitproductionlocked));
					}
					else {
						hud_button_actions_unclickable[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 574,678,67,41 }, "", Panel_Fade::no_one_fade, { 0,0,0,0 },
							{ 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1, (int)TooltipsAvailable::cleric_unlocked));
					}
				}
			}
			break;
		}
		}

	}
}

// Called to get the rect of the sprite of the portrait
SDL_Rect HUD::GetSpritePortrait(int type_of_portrait, UnitType unit_type) {
	SDL_Rect sprite = { 0,0,0,0 };
	if (type_of_portrait == 0) {
		switch (unit_type) {
		case UnitType::ASSASSIN:
			sprite = { 2,321,76,105 };
			break;
		case UnitType::PIKEMAN:
			sprite = { 82,321,76,105 };
			break;
		case UnitType::MONK:
			sprite = { 162,321,76,105 };
			break;
		case UnitType::PRIEST:
			sprite = { 242,321,76,105 };
			break;
		case UnitType::MINOTAUR:
			sprite = { 324,726,76,105 };
			break;
		case UnitType::CYCLOP:
			sprite = { 242,726,76,105 };
			break;
		case UnitType::JOTNAR:
			sprite = { 162,726,76,105 };
			break;
		case UnitType::DRAUGAR:
			sprite = { 82,726,76,105 };
			break;
		case UnitType::CLERIC:
			sprite = { 2,726,76,105 };
			break;
		}
	}
	else if (type_of_portrait == 1) {
		switch (unit_type) {
		case UnitType::ASSASSIN:
			sprite = { 299,194,30,41 };
			break;
		case UnitType::PIKEMAN:
			sprite = { 331,194,30,41 };
			break;
		case UnitType::MONK:
			sprite = { 362,194,30,41 };
			break;
		case UnitType::PRIEST:
			sprite = { 393,194,30,41 };
			break;
		case UnitType::MINOTAUR:
			sprite = { 424,194,30,41 };
			break;
		case UnitType::CYCLOP:
			sprite = { 455,194,30,41 };
			break;
		case UnitType::JOTNAR:
			sprite = { 487,194,30,41 };
			break;
		case UnitType::DRAUGAR:
			sprite = { 518,194,30,41 };
			break;
		case UnitType::CLERIC:
			sprite = { 549,194,30,41 };
			break;
		}
	}
	else if (type_of_portrait == 2) {
		switch (unit_type) {
		case UnitType::ASSASSIN:
			sprite = { 3,836,30,41 };
			break;
		case UnitType::PIKEMAN:
			sprite = { 34,836,30,41 };
			break;
		case UnitType::MONK:
			sprite = { 66,836,30,41 };
			break;
		case UnitType::PRIEST:
			sprite = { 97,836,30,41 };
			break;
		case UnitType::MINOTAUR:
			sprite = { 128,836,30,41 };
			break;
		case UnitType::CYCLOP:
			sprite = { 159,836,30,41 };
			break;
		case UnitType::JOTNAR:
			sprite = { 191,836,30,41 };
			break;
		case UnitType::DRAUGAR:
			sprite = { 222,836,30,41 };
			break;
		case UnitType::CLERIC:
			sprite = { 253,836,30,41 };
			break;
		}
	}
	else if (type_of_portrait == 3) {
		switch (unit_type) {
		case UnitType::ASSASSIN:
			sprite = { 3,881,30,41 };
			break;
		case UnitType::PIKEMAN:
			sprite = { 34,881,30,41 };
			break;
		case UnitType::MONK:
			sprite = { 66,881,30,41 };
			break;
		case UnitType::PRIEST:
			sprite = { 97,881,30,41 };
			break;
		case UnitType::MINOTAUR:
			sprite = { 128,881,30,41 };
			break;
		case UnitType::CYCLOP:
			sprite = { 159,881,30,41 };
			break;
		case UnitType::JOTNAR:
			sprite = { 191,881,30,41 };
			break;
		case UnitType::DRAUGAR:
			sprite = { 222,881,30,41 };
			break;
		case UnitType::CLERIC:
			sprite = { 253,881,30,41 };
			break;
		}
	}
	return sprite;

}

// Called to get the unit type from a sprite portrait
UnitType HUD::GetUnitTypeFromSpritePortrait(SDL_Rect sprite)
{
	if (sprite.w == 30 && sprite.h == 41) {
		if (sprite.y == 194) {
			if (sprite.x == 299)
				return UnitType::ASSASSIN;
			else if (sprite.x == 331)
				return UnitType::PIKEMAN;
			else if (sprite.x == 362)
				return UnitType::MONK;
			else if (sprite.x == 393)
				return UnitType::PRIEST;
			else if (sprite.x == 424)
				return UnitType::MINOTAUR;
			else if (sprite.x == 455)
				return UnitType::CYCLOP;
			else if (sprite.x == 487)
				return UnitType::JOTNAR;
			else if (sprite.x == 518)
				return UnitType::DRAUGAR;
			else if (sprite.x == 549)
				return UnitType::CLERIC;
		}
	}
	return UnitType::NONE;
}

// Called to get the rect of the sprite of the portrait of the building
SDL_Rect HUD::GetSpritePortraitBuilding(int type_of_portrait, BuildingType building_type, CivilizationType civilization) {
	SDL_Rect sprite = { 0,0,0,0 };
	if (type_of_portrait == 0) {
		switch (building_type) {
		case BuildingType::FORTRESS:
			if (civilization == CivilizationType::VIKING)
				sprite = { 2,431,76,105 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 323,431,76,105 };
			break;
		case BuildingType::MONASTERY:
			if (civilization == CivilizationType::VIKING)
				sprite = { 82,431,76,105 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 403,431,76,105 };
			break;
		case BuildingType::ENCAMPMENT:
			if (civilization == CivilizationType::VIKING)
				sprite = { 162,431,76,105 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 483,431,76,105 };
			break;
		case BuildingType::TEMPLE:
			if (civilization == CivilizationType::VIKING)
				sprite = { 242,431,76,105 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 563,431,76,105 };
			break;
		}
	}
	return sprite;
}

// Called to get the rect of the sprite of the portrait of the thing produced
SDL_Rect HUD::GetSpritePortraitProduction(int type_of_portrait, const std::string &produced_type, CivilizationType civilization)
{
	SDL_Rect sprite = { 0,0,0,0 };
	if (type_of_portrait == 0) {
		if (produced_type == "Assassin")
			sprite = { 3,931,30,41 };
		else if (produced_type == "Pikeman")
			sprite = { 34,931,30,41 };
		else if (produced_type == "Monk")
			sprite = { 66,931,30,41 };
		else if (produced_type == "Priest")
			sprite = { 97,931,30,41 };
		else if (produced_type == "Lawful_Beast" || produced_type == "Lawful Beast") {
			if (civilization == CivilizationType::VIKING)
				sprite = { 191,931,30,41 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 159,931,30,41 };
		}
		else if (produced_type == "Chaotic_Beast" || produced_type == "Chaotic Beast") {
			if (civilization == CivilizationType::GREEK)
				sprite = { 128,931,30,41 };
			else if (civilization == CivilizationType::VIKING)
				sprite = { 222,931,30,41 };
		}
		else if (produced_type == "Cleric")
			sprite = { 253,931,30,41 };
		else if (produced_type == "Miracle" || produced_type == "Lawful Miracle")
			sprite = { 284,931,30,41 };
		else if (produced_type == "Prayers")
			sprite = { 316,931,30,41 };
		else if (produced_type == "Victory" || produced_type == "Lawful Victory" || produced_type == "Chaotic Victory")
			sprite = { 347,931,30,41 };
		else if (produced_type == "Sacrifices")
			sprite = { 378,931,30,41 };
		else if (produced_type == "Disaster" || produced_type == "Chaotic Miracle")
			sprite = { 409,931,30,41 };
		else if (produced_type == "Temple") {
			if (civilization == CivilizationType::VIKING)
				sprite = { 470,931,30,41 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 533,931,30,41 };
		}
		else if (produced_type == "Encampment") {
			if (civilization == CivilizationType::GREEK)
				sprite = { 502,931,30,41 };
			else if (civilization == CivilizationType::VIKING)
				sprite = { 440,931,30,41 };
		}
	}
	else if (type_of_portrait == 1) {
		if (produced_type == "Assassin")
			sprite = { 3,976,30,41 };
		else if (produced_type == "Pikeman")
			sprite = { 34,976,30,41 };
		else if (produced_type == "Monk")
			sprite = { 66,976,30,41 };
		else if (produced_type == "Priest")
			sprite = { 97,976,30,41 };
		else if (produced_type == "Lawful_Beast" || produced_type == "Lawful Beast") {
			if (civilization == CivilizationType::VIKING)
				sprite = { 191,976,30,41 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 159,976,30,41 };
		}
		else if (produced_type == "Chaotic_Beast" || produced_type == "Chaotic Beast") {
			if (civilization == CivilizationType::GREEK)
				sprite = { 128,976,30,41 };
			else if (civilization == CivilizationType::VIKING)
				sprite = { 222,976,30,41 };
		}
		else if (produced_type == "Cleric")
			sprite = { 253,976,30,41 };
		else if (produced_type == "Miracle" || produced_type == "Lawful Miracle")
			sprite = { 284,976,30,41 };
		else if (produced_type == "Prayers")
			sprite = { 316,976,30,41 };
		else if (produced_type == "Victory" || produced_type == "Lawful Victory" || produced_type == "Chaotic Victory")
			sprite = { 347,976,30,41 };
		else if (produced_type == "Sacrifices")
			sprite = { 378,976,30,41 };
		else if (produced_type == "Disaster" || produced_type == "Chaotic Miracle")
			sprite = { 409,976,30,41 };
		else if (produced_type == "Temple") {
			if (civilization == CivilizationType::VIKING)
				sprite = { 470,976,30,41 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 533,976,30,41 };
		}
		else if (produced_type == "Encampment") {
			if (civilization == CivilizationType::GREEK)
				sprite = { 502,976,30,41 };
			else if (civilization == CivilizationType::VIKING)
				sprite = { 440,976,30,41 };
		}
	}
	else if (type_of_portrait == 2) {
		if (produced_type == "Assassin")
			sprite = { 3,1021,30,41 };
		else if (produced_type == "Pikeman")
			sprite = { 34,1021,30,41 };
		else if (produced_type == "Monk")
			sprite = { 66,1021,30,41 };
		else if (produced_type == "Priest")
			sprite = { 97,1021,30,41 };
		else if (produced_type == "Lawful_Beast" || produced_type == "Lawful Beast") {
			if (civilization == CivilizationType::VIKING)
				sprite = { 191,1021,30,41 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 159,1021,30,41 };
		}
		else if (produced_type == "Chaotic_Beast" || produced_type == "Chaotic Beast") {
			if (civilization == CivilizationType::GREEK)
				sprite = { 128,1021,30,41 };
			else if (civilization == CivilizationType::VIKING)
				sprite = { 222,1021,30,41 };
		}
		else if (produced_type == "Cleric")
			sprite = { 253,1021,30,41 };
		else if (produced_type == "Miracle" || produced_type == "Lawful Miracle")
			sprite = { 284,1021,30,41 };
		else if (produced_type == "Prayers")
			sprite = { 316,1021,30,41 };
		else if (produced_type == "Victory" || produced_type == "Lawful Victory" || produced_type == "Chaotic Victory")
			sprite = { 347,1021,30,41 };
		else if (produced_type == "Sacrifices")
			sprite = { 378,1021,30,41 };
		else if (produced_type == "Disaster" || produced_type == "Chaotic Miracle")
			sprite = { 409,1021,30,41 };
		else if (produced_type == "Temple") {
			if (civilization == CivilizationType::VIKING)
				sprite = { 470,1021,30,41 };
			else if (civilization == CivilizationType::GREEK)
				sprite = { 533,1021,30,41 };
		}
		else if (produced_type == "Encampment") {
			if (civilization == CivilizationType::GREEK)
				sprite = { 502,1021,30,41 };
			else if (civilization == CivilizationType::VIKING)
				sprite = { 440,1021,30,41 };
		}
	}
	return sprite;
}

//Called when clicking the research button
void HUD::ActivateResearchMenu() {
	if (research_menu->CheckReasearchWindow() == false) {
		PauseGame();
		uint w, h;
		App->win->GetWindowSize(w, h);
		ui_pause_black_screen[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,0,(int)w,(int)h }, "", 0, 0, 0, 150,Panel_Fade::panel_fade_in));
		research_menu->ActivateResearchMenu();
		App->audio->PlayFx(4, App->scene->Research_sound);
	}
}

//Called when clicking close button in the research menu
void HUD::DeactivateResearchMenu()
{
	if (research_menu->CheckReasearchWindow() == true)
	{
		research_menu->DeactivateResearchMenu();
		if (ui_pause_black_screen[1] != nullptr)
		{
			App->gui->DeleteUIElement(ui_pause_black_screen[1]);
			ui_pause_black_screen[1] = nullptr;
		}
	}
	App->audio->PlayFx(4, App->scene->ResearchFinished);
	ResumeGame();
}

//Called when pausing the game
void HUD::PauseGame() {
	App->scene->paused_game = true;
	App->gui->DeactivateButtons();
}

//Called when resuming the game
void HUD::ResumeGame() {
	App->scene->paused_game = false;
	App->gui->ActivateButtons();
}

//Called when updating the scene to change the width of the sliders
void HUD::UpdateSlider(int index) {
	if (index == 0) {
		ui_volume_sliders[0]->quad.w = (int)(181 * ((float)App->audio->GetVolumeMusic() / (float)128));
	}
	else if (index == 3) {
		ui_volume_sliders[3]->quad.w = (int)(181 * ((float)App->audio->GetVolumeFx() / (float)128));
		if (ui_volume_sliders[5]->unclicked == true) {
			App->audio->PlayFx(-1, App->gui->sfx_UI[(int)UI_Audio::LOAD]);
		}
	}
}
