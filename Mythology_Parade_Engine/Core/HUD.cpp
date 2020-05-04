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


//Constructor
HUD::HUD() {
	this->StartHUD();
}

//Destructor
HUD::~HUD() {

}

// Called before the first frame
void HUD::StartHUD() {
	ui_research_window = nullptr;
	ui_winlose_window = nullptr;
	for (uint i = 0; i < 4; i++)
	{
		ui_text_winlose[i] = nullptr;
	}

	start_timer = false;
	ui_ingame = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, nullptr, { 0,590,1280,130 }, { 0,590,1280,130 });


	for (int i = 0; i < 3; i++)
	{
		ui_text_ingame[i] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 104,610 + (i * 33),237,38 }, { 0,0,100,100 }, "9999", { 255,255,255,255 }, { 1,0,0,0 });
	}
	for (int i = 0; i < 8; i++)
	{
		if (i != 7)
		{
			ui_button[i] = nullptr;
		}
		ui_text[i] = nullptr;
	}
	for (int i = 0; i < 13; i++)
	{
		hud_list_troops[i] = nullptr;
		hud_number_troops[i] = nullptr;
		hud_stats_selected_troop[i] = nullptr;
	}

	for (int i = 0; i < 5; i++) {
		hud_button_actions[i] = nullptr;
		hud_button_actions_unclickable[i] = nullptr;
	}

	hud_selected_troop = nullptr;
	thing_selected = nullptr;

	for (int i = 0; i < 4; i++)
	{
		if (i < 3)
		{
			ui_button_confirmation[i] = nullptr;
		}
		ui_text_confirmation[i] = nullptr;
	}
	confirmation_option = "";

	close_menus = CloseSceneMenus::None;

	research_encampment = research_monastery = research_temple = false;
}

// Called when clicking esc
void HUD::ActivatePauseMenu() {
	if (ui_pause_window == nullptr) {
		ui_pause_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,100,459,531 }, { 216,21,459,531 });
		ui_button[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,160,237,38 }, { 787,240,237,38 }, "SAVE", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, App->scene, (int)UI_Audio::SAVE);
		ui_text[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,172,237,38 }, { 0,0,100,100 }, "Save Game", { 0,0,0,255 });
		ui_button[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,220,237,38 }, { 787,240,237,38 }, "LOAD", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, App->scene, (int)UI_Audio::LOAD);
		ui_text[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,232,237,38 }, { 0,0,100,100 }, "Load Game", { 0,0,0,255 });
		ui_button[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,280,237,38 }, { 787,240,237,38 }, "OPTIONS", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, App->scene, (int)UI_Audio::OPTIONS);
		ui_text[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 613,292,237,38 }, { 0,0,100,100 }, "Options", { 0,0,0,255 });
		ui_button[3] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,340,237,38 }, { 787,240,237,38 }, "RESTART", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, App->scene, (int)UI_Audio::RESTART);
		ui_text[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 614,352,237,38 }, { 0,0,100,100 }, "Restart", { 0,0,0,255 });
		ui_button[4] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,400,237,38 }, { 787,240,237,38 }, "SURRENDER", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, App->scene, (int)UI_Audio::SURRENDER);
		ui_text[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 607,412,237,38 }, { 0,0,100,100 }, "Surrender");
		ui_button[5] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,460,237,38 }, { 787,240,237,38 }, "EXIT", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, App->scene, (int)UI_Audio::EXIT);
		ui_text[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 625,472,237,38 }, { 0,0,100,100 }, "Exit", { 0,0,0,255 });
		ui_button[6] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_pause_window, { 520,560,237,38 }, { 787,240,237,38 }, "CLOSE", { 787,342,237,38 }, { 787,291,237,38 }, false,
			{ 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
		ui_text[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 620,572,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 604,112,237,38 }, { 0,0,100,100 }, "PAUSE", { 255,255,255,255 }, { 1,0,0,0 });
	}
	App->scene->paused_game = true;
}

// Called when clicking close button in pause menu
void HUD::DeactivatePauseMenu() {
	if (ui_pause_window != nullptr) {
		App->gui->DeleteUIElement(ui_pause_window);
		ui_pause_window = nullptr;
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
	App->scene->paused_game = false;
}


// Called when clicking options button in pause menu
void HUD::ActivateOptionsMenu() {
	if (ui_options_window == nullptr) {
		ui_options_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 });

		ui_button_options[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_options_window, { 520,300,237,38 }, { 787,240,237,38 }, "CLOSE OPTIONS", { 787,342,237,38 },
			{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
		ui_button_options[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_options_window, { 570,250,36,36 }, { 16,21,36,36 }, "FULLSCREEN", { 98,21,36,36 },
			{ 57,21,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU);
		if (App->win->isFullscreen() == true) {
			ui_button_options[1]->sprite1.y = ui_button_options[1]->sprite2.y = ui_button_options[1]->sprite3.y = 61;
		}
		ui_text_options[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,312,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text_options[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,212,237,38 }, { 0,0,100,100 }, "OPTIONS", { 255,255,255,255 }, { 1,0,0,0 });
		ui_text_options[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 620,260,237,38 }, { 0,0,100,100 }, "FULLSCREEN", { 255,255,255,255 });
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
		for (int i = 1; i >= 0; i--) {
			if (ui_button_options[i] != nullptr) {
				App->gui->DeleteUIElement(ui_button_options[i]);
				ui_button_options[i] = nullptr;
			}
		}
		for (int i = 2; i >= 0; i--) {
			if (ui_text_options[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_options[i]);
				ui_text_options[i] = nullptr;
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
		ui_confirmation_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,168 }, { 790,408,459,168 });
		ui_button_confirmation[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 470,300,117,38 }, { 834,125,117,24 }, "YES", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CONFIRMATION);
		ui_text_confirmation[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 509,306,237,38 }, { 0,0,100,100 }, "Yes", { 0,0,0,255 }, { 1,0,0,0 });
		ui_button_confirmation[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_confirmation_window, { 690,300,117,38 }, { 834,125,117,24 }, "NO", { 834,149,117,24 },
			{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CONFIRMATION);
		ui_text_confirmation[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 731,306,237,38 }, { 0,0,100,100 }, "No", { 0,0,0,255 }, { 1,0,0,0 });
		std::string text = str + " ?";
		ui_text_confirmation[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 463,212,237,38 }, { 0,0,100,100 }, "ARE YOU SURE YOU WANT TO", { 255,255,255,255 }, { 1,0,0,0 });
		int size = text.length();
		ui_text_confirmation[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 640 - (6 * size),247,237,38 }, { 0,0,100,100 }, text, { 255,255,255,255 }, { 1,0,0,0 });

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
void HUD::HUDUpdateSelection(std::list<Entity*> listEntities, Building* building_selected) {
	int i = 0;
	bool viking = true;
	HUDDeleteListTroops();
	HUDDeleteSelectedTroop();
	HUDDeleteActionButtons();
	for (std::list<Entity*>::iterator it = listEntities.begin(); it != listEntities.end(); it++) {
		if (it._Ptr->_Myval->type == EntityType::UNIT) {
			Unit* unit = (Unit*)it._Ptr->_Myval;
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
	SDL_Rect r = { 825,618,30,41 };
	SDL_Rect r2 = { 828,645,17,9 };

	for (int j = 0; j < i; j++) {

		if (j == 6) {
			r.y += 50;
			r2.y += 50;
			r.x = 825;
			r2.x = 828;
		}
		hud_list_troops[j] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, (UI*)ui_ingame, r, GetSpritePortrait(0, type_of_troops[j]), "Troop", { 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 },
			nullptr, 0, false, -1.0F, 1);
		if (number_of_troops[j] > 1)
			hud_number_troops[j] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, r2, { 0,0,100,100 }, std::to_string(number_of_troops[j]), { 255,255,255,255 });
		r.x += 34;
		r2.x += 34;
	}

	SDL_Rect position_name = { 725,603,30,30 };
	if (!listEntities.empty()) {
		if (listEntities.begin()._Ptr->_Myval->type == EntityType::UNIT) {
			thing_selected = listEntities.begin()._Ptr->_Myval;
			Unit* unit = (Unit*)listEntities.begin()._Ptr->_Myval;
			hud_selected_troop = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, (UI*)ui_ingame, { 640,613,76,105 }, GetSpritePortrait(1, unit->unitType), "Troop", { 0,0,0,0 }, { 0,0,0,0 }, false,
				{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
			hud_stats_selected_troop[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, position_name, { 0,0,100,100 }, "Unit", { 0,0,0,255 });
			bool combat_unit = false;
			switch (unit->unitType) {
			case UnitType::MONK:
				hud_stats_selected_troop[0]->SetString("Monk");
				type_thing_selected = Type_Selected::Monk;
				position_name.x += 18;
				hud_stats_selected_troop[0]->SetRect(position_name);
				break;
			case UnitType::PRIEST:
				hud_stats_selected_troop[0]->SetString("Priest");
				type_thing_selected = Type_Selected::Priest;
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
			}
			if (combat_unit == true) {
				CombatUnit* cunit = (CombatUnit*)unit;
				hud_stats_selected_troop[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,622,30,30 }, { 0,0,100,100 }, "Damage:", { 0,0,0,255 });
				hud_stats_selected_troop[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 780,622,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetDamageValue()), { 0,0,0,255 });
				hud_stats_selected_troop[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,636,30,30 }, { 0,0,100,100 }, "Range:", { 0,0,0,255 });
				hud_stats_selected_troop[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 768,636,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetRangeValue()), { 0,0,0,255 });
				hud_stats_selected_troop[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,650,30,30 }, { 0,0,100,100 }, "Speed:", { 0,0,0,255 });
				hud_stats_selected_troop[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 760,650,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetSpeedValue()), { 0,0,0,255 });
				hud_stats_selected_troop[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,664,30,30 }, { 0,0,100,100 }, "Level:", { 0,0,0,255 });
				hud_stats_selected_troop[8] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 760,664,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetLevel()), { 0,0,0,255 });
				hud_stats_selected_troop[9] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,678,30,30 }, { 0,0,100,100 }, "Exp:", { 0,0,0,255 });
				hud_stats_selected_troop[10] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 747,678,30,30 }, { 0,0,100,100 }, std::to_string(cunit->GetExperience()), { 0,0,0,255 });
				hud_stats_selected_troop[11] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,703,30,10 }, { 0,0,100,100 }, "Health:", { 0,0,0,255 });
				hud_stats_selected_troop[12] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 773,703,30,10 }, { 0,0,100,100 }, std::to_string(unit->GetHealth()), { 0,0,0,255 });
			}
			else {
				hud_stats_selected_troop[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,703,30,10 }, { 0,0,100,100 }, "Health:", { 0,0,0,255 });
				hud_stats_selected_troop[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 773,703,30,10 }, { 0,0,100,100 }, std::to_string(unit->GetHealth()), { 0,0,0,255 });
			}
		}
	}
	else if (building_selected != nullptr) {
		thing_selected = building_selected;
		hud_selected_troop = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, (UI*)ui_ingame, { 640,613,76,105 }, GetSpritePortraitBuilding(0, building_selected->GetBuildingType(),
			building_selected->civilization), "Building", { 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
		hud_stats_selected_troop[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, position_name, { 0,0,100,100 }, "Building", { 0,0,0,255 });
		switch (building_selected->GetBuildingType()) {
		case BuildingType::FORTRESS:
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
		hud_stats_selected_troop[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,622,30,30 }, { 0,0,100,100 }, "Influence:", { 0,0,0,255 });
		hud_stats_selected_troop[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 790,622,30,30 }, { 0,0,100,100 }, std::to_string(building_selected->GetInfluence()), { 0,0,0,255 });
		hud_stats_selected_troop[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,636,30,30 }, { 0,0,100,100 }, "Damage:", { 0,0,0,255 });
		hud_stats_selected_troop[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 778,636,30,30 }, { 0,0,100,100 }, std::to_string(building_selected->GetDamage()), { 0,0,0,255 });
		hud_stats_selected_troop[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,650,30,30 }, { 0,0,100,100 }, "Max Cap:", { 0,0,0,255 });
		hud_stats_selected_troop[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 780,650,30,30 }, { 0,0,100,100 }, std::to_string(building_selected->GetMaxCap()), { 0,0,0,255 });
		hud_stats_selected_troop[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 720,703,30,10 }, { 0,0,100,100 }, "Health:", { 0,0,0,255 });
		hud_stats_selected_troop[8] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, (UI*)ui_ingame, { 773,703,30,10 }, { 0,0,100,100 }, std::to_string(building_selected->GetHealth()), { 0,0,0,255 });
		if (building_selected->civilization == CivilizationType::GREEK)
			viking = false;
	}
	ManageActionButtons(true, viking);
}

// Called when deleting the list of troops in the HUD
void HUD::HUDDeleteListTroops() {
	for (int i = 12; i >= 0; i--) {
		number_of_troops[i] = 0;
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
	for (int i = 4; i >= 0; i--) {
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
		if (hud_stats_selected_troop[5] != nullptr) {
			CombatUnit* cunit = (CombatUnit*)thing_selected;
			hud_stats_selected_troop[2]->SetString(std::to_string(cunit->GetDamageValue()));
			hud_stats_selected_troop[4]->SetString(std::to_string(cunit->GetRangeValue()));
			hud_stats_selected_troop[6]->SetString(std::to_string(cunit->GetSpeedValue()));
			hud_stats_selected_troop[8]->SetString(std::to_string(cunit->GetLevel()));
			hud_stats_selected_troop[10]->SetString(std::to_string(cunit->GetExperience()));
			hud_stats_selected_troop[12]->SetString(std::to_string(cunit->GetHealth()));
		}
		else {
			Unit* unit = (Unit*)thing_selected;
			hud_stats_selected_troop[2]->SetString(std::to_string(unit->GetHealth()));
		}
	}
	else if (thing_selected->type == EntityType::BUILDING) {
		Building* building = (Building*)thing_selected;
		hud_stats_selected_troop[2]->SetString(std::to_string(building->GetInfluence()));
		hud_stats_selected_troop[4]->SetString(std::to_string(building->GetDamage()));
		hud_stats_selected_troop[6]->SetString(std::to_string(building->GetMaxCap()));
		hud_stats_selected_troop[8]->SetString(std::to_string(building->GetHealth()));
	}
}

//Called when creating or updating the action buttons
void HUD::ManageActionButtons(bool create_buttons, bool viking) {
	if (create_buttons == true) {
		switch (type_thing_selected) {
		case Type_Selected::Assassin:
		{
			CombatUnit* cunit = (CombatUnit*)thing_selected;
			int exp_needed = cunit->GetExperienceToLevel(cunit->GetLevel());
			hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,36,36 }, { 16,185,36,36 }, "Move", { 98,185,36,36 }, { 57,185,36,36 }, false, { 0,0,0,0 },
				App->scene, (int)UI_Audio::MAIN_MENU);
			hud_button_actions[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 241,613,36,36 }, { 16,143,36,36 }, "Attack", { 98,143,36,36 }, { 57,143,36,36 }, false, { 0,0,0,0 },
				App->scene, (int)UI_Audio::MAIN_MENU);
			if (exp_needed > cunit->GetExperience()) {
				hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 282,613,36,36 }, { 16,102,36,36 }, "Upgrade", { 98,102,36,36 }, { 57,102,36,36 }, false, { 0,0,0,0 },
					App->scene, (int)UI_Audio::MAIN_MENU);
			}
			else {
				hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 282,613,36,36 }, { 98,442,36,36 });
			}
			break;
		}
		case Type_Selected::Monk:
			hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,36,36 }, { 16,185,36,36 }, "Move", { 98,185,36,36 }, { 57,185,36,36 }, false, { 0,0,0,0 },
				App->scene, (int)UI_Audio::MAIN_MENU);
			break;
		case Type_Selected::Fortress:
		{
			Building* building = (Building*)thing_selected;
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (viking == true) {
					if (App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_temple < 8 && research_temple == true) {
						hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,604,99,60 }, { 2,2,99,60 }, "Produce_Temple", { 2,124,99,60 }, { 2,63,99,60 },
							false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else {
						hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,604,99,60 }, { 329,313,99,60 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_encampment < 7 && research_encampment == true) {
						hud_button_actions[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 104,11,99,51 }, "Produce_Encampment", { 104,133,99,51 },
							{ 104,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else {
						hud_button_actions_unclickable[1] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,322,99,51 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_monastery < 5 && research_monastery == true) {
						hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 206,10,99,52 }, "Produce_Monastery", { 206,132,99,52 },
							{ 206,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else {
						hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,321,99,52 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
				}
				else {
					if (App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_temple < 8 && research_temple == true) {
						hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,602,99,62 }, { 308,0,99,62 }, "Produce_Temple", { 308,122,99,62 },
							{ 308,61,99,62 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else {
						hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,602,99,62 }, { 329,246,99,62 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_encampment < 7 && research_encampment == true) {
						hud_button_actions[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 410,11,99,51 }, "Produce_Encampment", { 410,133,99,51 },
							{ 410,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else {
						hud_button_actions_unclickable[1] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,257,99,51 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_monastery < 5 && research_monastery == true) {
						hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 512,10,99,52 }, "Produce_Monastery", { 512,132,99,52 },
							{ 512,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else {
						hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,256,99,52 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
				}
				if (App->entityManager->getPlayer()->GetFaith() >= 600 && building->buildingAction == BuildingAction::NOTHING) {
					hud_button_actions[3] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 512,621,67,41 }, { 75,540,67,41 }, "Produce_Victory", { 75,632,67,41 },
						{ 75,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else {
					hud_button_actions_unclickable[3] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
				if (building->buildingAction == BuildingAction::NOTHING) {
					hud_button_actions[4] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 584,626,36,36 }, { 16,227,36,36 }, "Research", { 98,227,36,36 },
						{ 57,227,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU);
				}
				else {
					hud_button_actions_unclickable[4] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 584,626,36,36 }, { 98,484,36,36 });
				}
			}
			break;
		}
		case Type_Selected::Temple:
		{
			Building* building = (Building*)thing_selected;
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 337,382,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
					{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
			}
			break;
		}
		case Type_Selected::Encampment:
		{
			Building* building = (Building*)thing_selected;
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (App->entityManager->getPlayer()->GetFaith() >= 100 && App->entityManager->getPlayer()->GetSacrifices() >= 10 && building->buildingAction == BuildingAction::NOTHING) {
					hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 215,185,67,41 }, "Produce_Assassin", { 215,275,67,41 },
						{ 215,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else {
					hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 549,382,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
				hud_button_actions_unclickable[1] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,613,67,41 }, { 478,382,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
					{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
			}
			break;
		}
		case Type_Selected::Monastery:
		{
			Building* building = (Building*)thing_selected;
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (App->entityManager->getPlayer()->GetFaith() >= 50 && building->buildingAction == BuildingAction::NOTHING) {
					hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 73,185,67,41 }, "Produce_Monk", { 73,275,67,41 },
						{ 73,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else {
					hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 407,382,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
				if (App->entityManager->getPlayer()->GetFaith() >= 40 && building->buildingAction == BuildingAction::NOTHING) {
					hud_button_actions[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,613,67,41 }, { 3,540,67,41 }, "Produce_Sacrifices", { 3,632,67,41 },
						{ 3,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else {
					hud_button_actions_unclickable[1] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,613,67,41 }, { 3,678,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
				if (App->entityManager->getPlayer()->GetFaith() >= 40 && building->buildingAction == BuildingAction::NOTHING) {
					hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 344,613,67,41 }, { 146,540,67,41 }, "Produce_Prayers", { 146,632,67,41 },
						{ 146,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else {
					hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,613,67,41 }, { 146,678,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
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
			CombatUnit* cunit = (CombatUnit*)thing_selected;
			int exp_needed = cunit->GetExperienceToLevel(cunit->GetLevel());
			if (hud_button_actions[2] == nullptr && exp_needed <= cunit->GetExperience()) {
				if (hud_button_actions_unclickable[2] != nullptr) {
					App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
					hud_button_actions_unclickable[2] = nullptr;
				}
				hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 282,613,36,36 }, { 16,102,36,36 }, "Upgrade", { 98,102,36,36 }, { 57,102,36,36 }, false, { 0,0,0,0 },
					App->scene, (int)UI_Audio::MAIN_MENU);
			}
			else if (hud_button_actions_unclickable[2] == nullptr && exp_needed > cunit->GetExperience()) {
				if (hud_button_actions[2] != nullptr) {
					App->gui->DeleteUIElement(hud_button_actions[2]);
					hud_button_actions[2] = nullptr;
				}
				hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 282,613,36,36 }, { 98,442,36,36 });
			}
			break;
		}
		case Type_Selected::Fortress:
		{
			Building* building = (Building*)thing_selected;
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (viking == true) {
					if (hud_button_actions[0] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_temple < 8 && research_temple == true) {
						if (hud_button_actions_unclickable[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
							hud_button_actions_unclickable[0] = nullptr;
						}
						hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,604,99,60 }, { 2,2,99,60 }, "Produce_Temple", { 2,124,99,60 }, { 2,63,99,60 },
							false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else if (hud_button_actions_unclickable[0] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 200 || App->entityManager->getPlayer()->num_temple >= 8 ||
						research_temple == false)) {
						if (hud_button_actions[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[0]);
							hud_button_actions[0] = nullptr;
						}
						hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,604,99,60 }, { 329,313,99,60 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (hud_button_actions[1] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_encampment < 7 && research_encampment == true) {
						if (hud_button_actions_unclickable[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[1]);
							hud_button_actions_unclickable[1] = nullptr;
						}
						hud_button_actions[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 104,11,99,51 }, "Produce_Encampment", { 104,133,99,51 },
							{ 104,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else if (hud_button_actions_unclickable[1] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 200 || App->entityManager->getPlayer()->num_encampment >= 7 ||
						research_encampment == false)) {
						if (hud_button_actions[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[1]);
							hud_button_actions[1] = nullptr;
						}
						hud_button_actions_unclickable[1] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,322,99,51 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (hud_button_actions[2] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_monastery < 5 && research_monastery == true) {
						if (hud_button_actions_unclickable[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
							hud_button_actions_unclickable[2] = nullptr;
						}
						hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 206,10,99,52 }, "Produce_Monastery", { 206,132,99,52 },
							{ 206,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else if (hud_button_actions_unclickable[2] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 200 || App->entityManager->getPlayer()->num_monastery >= 5 ||
						research_monastery == false)) {
						if (hud_button_actions[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[2]);
							hud_button_actions[2] = nullptr;
						}
						hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,321,99,52 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
				}
				else {
					if (hud_button_actions[0] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_temple < 8 && research_temple == true) {
						if (hud_button_actions_unclickable[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
							hud_button_actions_unclickable[0] = nullptr;
						}
						hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,602,99,62 }, { 308,0,99,62 }, "Produce_Temple", { 308,122,99,62 }, { 308,61,99,62 },
							false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else if (hud_button_actions_unclickable[0] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 200 || App->entityManager->getPlayer()->num_temple >= 8 ||
						research_temple == false)) {
						if (hud_button_actions[0] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[0]);
							hud_button_actions[0] = nullptr;
						}
						hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,602,99,62 }, { 329,246,99,62 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (hud_button_actions[1] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_encampment < 7 && research_encampment == true) {
						if (hud_button_actions_unclickable[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[1]);
							hud_button_actions_unclickable[1] = nullptr;
						}
						hud_button_actions[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 304,613,99,51 }, { 410,11,99,51 }, "Produce_Encampment", { 410,133,99,51 },
							{ 410,72,99,51 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else if (hud_button_actions_unclickable[1] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 200 || App->entityManager->getPlayer()->num_encampment >= 7 ||
						research_encampment == false)) {
						if (hud_button_actions[1] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[1]);
							hud_button_actions[1] = nullptr;
						}
						hud_button_actions_unclickable[1] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 304,613,99,51 }, { 430,257,99,51 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
					if (hud_button_actions[2] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 200 && App->entityManager->getPlayer()->num_monastery < 5 && research_monastery == true) {
						if (hud_button_actions_unclickable[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
							hud_button_actions_unclickable[2] = nullptr;
						}
						hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 408,612,99,52 }, { 512,10,99,52 }, "Produce_Monastery", { 512,132,99,52 },
							{ 512,71,99,52 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
					}
					else if (hud_button_actions_unclickable[2] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 200 || App->entityManager->getPlayer()->num_monastery >= 5 ||
						research_monastery == false)) {
						if (hud_button_actions[2] != nullptr) {
							App->gui->DeleteUIElement(hud_button_actions[2]);
							hud_button_actions[2] = nullptr;
						}
						hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 408,612,99,52 }, { 532,256,99,52 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
							{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
					}
				}
				if (hud_button_actions[3] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 600 && building->buildingAction == BuildingAction::NOTHING) {
					if (hud_button_actions_unclickable[3] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[3]);
						hud_button_actions_unclickable[3] = nullptr;
					}
					hud_button_actions[3] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 512,621,67,41 }, { 75,540,67,41 }, "Produce_Victory", { 75,632,67,41 },
						{ 75,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else if (hud_button_actions_unclickable[3] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 600 || building->buildingAction != BuildingAction::NOTHING)) {
					if (hud_button_actions[3] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[3]);
						hud_button_actions[3] = nullptr;
					}
					hud_button_actions_unclickable[3] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 512,621,67,41 }, { 75,678,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
				if (hud_button_actions[4] == nullptr && building->buildingAction == BuildingAction::NOTHING) {
					if (hud_button_actions_unclickable[4] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[4]);
						hud_button_actions_unclickable[4] = nullptr;
					}
					hud_button_actions[4] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 584,626,36,36 }, { 16,227,36,36 }, "Research", { 98,227,36,36 },
						{ 57,227,36,36 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU);
				}
				else if (hud_button_actions_unclickable[4] == nullptr && building->buildingAction != BuildingAction::NOTHING) {
					if (hud_button_actions[4] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[4]);
						hud_button_actions[4] = nullptr;
					}
					hud_button_actions_unclickable[4] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 584,626,36,36 }, { 98,484,36,36 });
				}
			}
			break;
		}
		case Type_Selected::Encampment:
		{
			Building* building = (Building*)thing_selected;
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (hud_button_actions[0] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 100 && App->entityManager->getPlayer()->GetSacrifices() >= 10 &&
					building->buildingAction == BuildingAction::NOTHING) {
					if (hud_button_actions_unclickable[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
						hud_button_actions_unclickable[0] = nullptr;
					}
					hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 215,185,67,41 }, "Produce_Assassin", { 215,275,67,41 },
						{ 215,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else if (hud_button_actions_unclickable[0] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 100 || App->entityManager->getPlayer()->GetSacrifices() < 10 ||
					building->buildingAction != BuildingAction::NOTHING)) {
					if (hud_button_actions[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[0]);
						hud_button_actions[0] = nullptr;
					}
					hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 549,382,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
			}
			break;
		}
		case Type_Selected::Monastery:
		{
			Building* building = (Building*)thing_selected;
			if (building->buildingStatus == BuildingStatus::FINISHED) {
				if (hud_button_actions[0] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 50 && building->buildingAction == BuildingAction::NOTHING) {
					if (hud_button_actions_unclickable[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[0]);
						hud_button_actions_unclickable[0] = nullptr;
					}
					hud_button_actions[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 200,613,67,41 }, { 73,185,67,41 }, "Produce_Monk", { 73,275,67,41 },
						{ 73,230,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else if (hud_button_actions_unclickable[0] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 50 || building->buildingAction != BuildingAction::NOTHING)) {
					if (hud_button_actions[0] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[0]);
						hud_button_actions[0] = nullptr;
					}
					hud_button_actions_unclickable[0] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 200,613,67,41 }, { 407,382,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
				if (hud_button_actions[1] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 40 && building->buildingAction == BuildingAction::NOTHING) {
					if (hud_button_actions_unclickable[1] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[1]);
						hud_button_actions_unclickable[1] = nullptr;
					}
					hud_button_actions[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 272,613,67,41 }, { 3,540,67,41 }, "Produce_Sacrifices", { 3,632,67,41 },
						{ 3,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else if (hud_button_actions_unclickable[1] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 20 || building->buildingAction != BuildingAction::NOTHING)) {
					if (hud_button_actions[1] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[1]);
						hud_button_actions[1] = nullptr;
					}
					hud_button_actions_unclickable[1] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 272,613,67,41 }, { 3,678,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
				}
				if (hud_button_actions[2] == nullptr && App->entityManager->getPlayer()->GetFaith() >= 40 && building->buildingAction == BuildingAction::NOTHING) {
					if (hud_button_actions_unclickable[2] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions_unclickable[2]);
						hud_button_actions_unclickable[2] = nullptr;
					}
					hud_button_actions[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_ingame, { 344,613,67,41 }, { 146,540,67,41 }, "Produce_Prayers", { 146,632,67,41 },
						{ 146,586,67,41 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::MAIN_MENU, false, -1.0F, 1);
				}
				else if (hud_button_actions_unclickable[2] == nullptr && (App->entityManager->getPlayer()->GetFaith() < 20 || building->buildingAction != BuildingAction::NOTHING)) {
					if (hud_button_actions[2] != nullptr) {
						App->gui->DeleteUIElement(hud_button_actions[2]);
						hud_button_actions[2] = nullptr;
					}
					hud_button_actions_unclickable[2] = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, ui_ingame, { 344,613,67,41 }, { 146,678,67,41 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false,
						{ 0,0,0,0 }, nullptr, 0, false, -1.0F, 1);
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
		}
	}
	else if (type_of_portrait == 1) {
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
		}
	}
	return sprite;

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

//Called when clicking the research button
void HUD::ActivateResearchMenu() {
	if (ui_research_window == nullptr) {
		ui_research_window = (WindowUI*)App->gui->CreateUIElement(Type::WINDOW, nullptr, { 410,200,459,268 }, { 790,408,459,168 });
		ui_button_research[0] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 520,400,237,38 }, { 787,240,237,38 }, "CLOSE RESEARCH", { 787,342,237,38 },
			{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
		ui_text_research[0] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,412,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 });
		ui_text_research[1] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,212,237,38 }, { 0,0,100,100 }, "RESEARCH", { 255,255,255,255 }, { 1,0,0,0 });
		if (research_monastery == false) {
			ui_button_research[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 580,340,117,24 }, { 834, 125, 117, 24 }, "RESEARCH MONASTERY", { 834,149,117,24 },
				{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
			ui_text_research[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 609,345,237,38 }, { 0,0,100,100 }, "Research", { 0,0,0,255 });
			ui_text_research[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 600,272,237,38 }, { 0,0,100,100 }, "MONASTERY", { 255,255,255,255 });
			ui_text_research[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 513,307,237,38 }, { 0,0,100,100 }, "Produces monks, sacrifices and prayers", { 255,255,255,255 });
		}
		else if (research_encampment == false || research_temple == false) {
			if (research_encampment == false && research_temple == false) {
				ui_button_research[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 450,340,117,24 }, { 834, 125, 117, 24 }, "RESEARCH ENCAMPMENT", { 834,149,117,24 },
					{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
				ui_text_research[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 479,345,237,38 }, { 0,0,100,100 }, "Research", { 0,0,0,255 });
				ui_text_research[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 465,272,237,38 }, { 0,0,100,100 }, "ENCAMPMENT", { 255,255,255,255 });
				ui_text_research[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 446,307,237,38 }, { 0,0,100,100 }, "Produces assassins", { 255,255,255,255 });
				ui_button_research[2] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 710,340,117,24 }, { 834, 125, 117, 24 }, "RESEARCH TEMPLE", { 834,149,117,24 },
					{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
				ui_text_research[5] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 739,345,237,38 }, { 0,0,100,100 }, "Research", { 0,0,0,255 });
				ui_text_research[6] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 745,272,237,38 }, { 0,0,100,100 }, "TEMPLE", { 255,255,255,255 });
				ui_text_research[7] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 673,307,237,38 }, { 0,0,100,100 }, "Useless in the vertical slice", { 255,255,255,255 });
			}
			else if (research_encampment == false) {
				ui_button_research[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 580,340,117,24 }, { 834, 125, 117, 24 }, "RESEARCH ENCAMPMENT", { 834,149,117,24 },
					{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
				ui_text_research[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 609,345,237,38 }, { 0,0,100,100 }, "Research", { 0,0,0,255 });
				ui_text_research[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 595,272,237,38 }, { 0,0,100,100 }, "ENCAMPMENT", { 255,255,255,255 });
				ui_text_research[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 576,307,237,38 }, { 0,0,100,100 }, "Produces assassins", { 255,255,255,255 });
			}
			else {
				ui_button_research[1] = (ButtonUI*)App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 580,340,117,24 }, { 834, 125, 117, 24 }, "RESEARCH TEMPLE", { 834,149,117,24 },
					{ 834,101,117,24 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE);
				ui_text_research[2] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 609,345,237,38 }, { 0,0,100,100 }, "Research", { 0,0,0,255 });
				ui_text_research[3] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 615,272,237,38 }, { 0,0,100,100 }, "TEMPLE", { 255,255,255,255 });
				ui_text_research[4] = (TextUI*)App->gui->CreateUIElement(Type::TEXT, nullptr, { 553,307,237,38 }, { 0,0,100,100 }, "Useless in the vertical slice", { 255,255,255,255 });
			}
		}
	}
	App->scene->paused_game = true;
}

//Called when clicking close button in the research menu
void HUD::DeactivateResearchMenu() {
	if (ui_research_window != nullptr) {
		App->gui->DeleteUIElement(ui_research_window);
		ui_research_window = nullptr;
		for (int i = 2; i >= 0; i--) {
			if (ui_button_research[i] != nullptr) {
				App->gui->DeleteUIElement(ui_button_research[i]);
				ui_button_research[i] = nullptr;
			}
		}
		for (int i = 7; i >= 0; i--) {
			if (ui_text_research[i] != nullptr) {
				App->gui->DeleteUIElement(ui_text_research[i]);
				ui_text_research[i] = nullptr;
			}
		}
	}
	App->scene->paused_game = false;
}
