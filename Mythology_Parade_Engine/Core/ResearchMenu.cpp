#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include"j1Gui.h"
#include "j1Scene.h"
#include "Player.h"
#include "ResearchMenu.h"
#include "TooltipData.h"

//Constructor
ResearchMenu::ResearchMenu(Player* player) {
	StartResearchMenu(player);
}

//Destructor
ResearchMenu::~ResearchMenu() {
	player = nullptr;
}

// Called before the first frame
void ResearchMenu::StartResearchMenu(Player* play) {

	player = play;
	ui_research_window = nullptr;
	for (int i = 0; i < 11; i++) {
			if (i < 2) {
				ui_text_research[i] = nullptr;
			}
		ui_image_research[i] = nullptr;
		ui_button_research[i] = nullptr;
	}
}

//Called when clicking the research button
void ResearchMenu::ActivateResearchMenu() {
	ui_research_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 130,10,1025,570 }, { 13,20,1025,570 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 },
		nullptr, 0, false, -1.0f, 2));
	ui_button_research[10] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 520,530,237,38 }, { 787,240,237,38 }, "CLOSE RESEARCH", Panel_Fade::panel_fade_in, { 787,342,237,38 },
		{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE));
	ui_text_research[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,542,237,38 }, { 0,0,100,100 }, "Close", Panel_Fade::panel_fade_in, { 0,0,0,255 }));
	ui_text_research[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,22,237,38 }, { 0,0,100,100 }, "RESEARCH", Panel_Fade::panel_fade_in, { 255,255,255,255 }, { 1,0,0,0 }));
	if (player->civilization == CivilizationType::VIKING)
		ui_image_research[10] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 566,46,157,107 }, { 663, 606, 157, 107 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	else if (player->civilization == CivilizationType::GREEK)
		ui_image_research[10] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 566,51,157,102 }, { 825, 611, 157, 102 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));

	CreateTempleButton();
	CreateEncampmentButton();
	CreateClericButton();
	CreateAssassinButton();
	CreateLawfulBeastButton();
	CreateChaoticBeastButton();
	CreateLawfulMiracleButton();
	CreateChaoticMiracleButton();
	CreateLawfulVictoryButton();
	CreateChaoticVictoryButton();
}

//Called when clicking close button in the research menu
void ResearchMenu::DeactivateResearchMenu() {
	if (ui_research_window != nullptr) {
		App->gui->DeleteUIElement(ui_research_window);
		ui_research_window = nullptr;
		for (int i = 10; i >= 0; i--) {
				if (i < 2) {
					if (ui_text_research[i] != nullptr) {
						App->gui->DeleteUIElement(ui_text_research[i]);
						ui_text_research[i] = nullptr;
					}
				}
			if (ui_image_research[i] != nullptr) {
				App->gui->DeleteUIElement(ui_image_research[i]);
				ui_image_research[i] = nullptr;
			}
			if (ui_button_research[i] != nullptr) {
				App->gui->DeleteUIElement(ui_button_research[i]);
				ui_button_research[i] = nullptr;
			}
		}
	}
}

//Return if research_window is active or not (nullptr)
bool ResearchMenu::CheckReasearchWindow() {
	if (ui_research_window == nullptr)
		return false;
	else
		return true;
}

//Called to create the button or image to research the temple;
void ResearchMenu::CreateTempleButton() {
	switch (player->civilization)
	{
	case CivilizationType::VIKING:
		if (player->research_temple == false) {
			ui_button_research[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 298,83,157,96 }, { 12, 617, 157, 96 }, "RESEARCH TEMPLE", Panel_Fade::panel_fade_in,
				{ 12,837,157,96 }, { 12,727,157,96 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::temple_unresearched));
		}
		else {
			ui_image_research[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 298,83,157,96 }, { 12, 947, 157, 96 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::temple));
		}
		break;
	case CivilizationType::GREEK:
		if (player->research_temple == false) {
			ui_button_research[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 298,80,157,99 }, { 337, 614, 157, 99 }, "RESEARCH TEMPLE", Panel_Fade::panel_fade_in,
				{ 337,834,157,99 }, { 337,724,157,99 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::temple_unresearched));
		}
		else {
			ui_image_research[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 298,80,157,99 }, { 337, 944, 157, 99 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::temple));
		}
		break;
	}
}

//Called to create the button or image to research the encampment;
void ResearchMenu::CreateEncampmentButton() {
	switch (player->civilization)
	{
	case CivilizationType::VIKING:
		if (player->research_encampment == false) {
			ui_button_research[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 831,99,157,82 }, { 174, 631, 157, 82 }, "RESEARCH ENCAMPMENT", Panel_Fade::panel_fade_in,
				{ 174,851,157,82 }, { 174,741,157,82 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::encampment_unresearched));
		}
		else {
			ui_image_research[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 831,99,157,82 }, { 174, 961, 157, 82 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::encampment));
		}
		break;
	case CivilizationType::GREEK:
		if (player->research_encampment == false) {
			ui_button_research[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 831,99,157,82 }, { 499, 631, 157, 82 }, "RESEARCH ENCAMPMENT", Panel_Fade::panel_fade_in,
				{ 499,851,157,82 }, { 499,741,157,82 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::encampment_unresearched));
		}
		else {
			ui_image_research[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 831,99,157,82 }, { 499, 961, 157, 82 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::encampment));
		}
		break;
	}
}

//Called to create the button or image to research the cleric;
void ResearchMenu::CreateClericButton() {
	if (player->research_cleric == false) {
		if (player->research_temple == true && player->GetPrayers() >= 5) {
			ui_button_research[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 196,197,140,99 }, { 12, 1171, 140, 99 }, "RESEARCH CLERIC", Panel_Fade::panel_fade_in,
				{ 12,1389,140,99 }, { 12,1279,140,99 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::cleric_unresearched));
		}
		else {
			ui_image_research[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 196,197,140,99 }, { 12, 1610, 140, 99 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::cleric_locked));
		}
	}
	else {
		ui_image_research[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 196,197,140,99 }, { 12, 1499, 140, 99 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::cleric_unlocked));
	}
}

//Called to create the button or image to research the assassin;
void ResearchMenu::CreateAssassinButton() {
	if (player->research_assassin == false) {
		if (player->research_encampment == true && player->GetSacrifices() >= 5) {
			ui_button_research[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 950,197,140,97 }, { 447, 1173, 140, 97 }, "RESEARCH ASSASSIN", Panel_Fade::panel_fade_in,
				{ 447,1391,140,97 }, { 447,1281,140,97 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::assassin_unresearched));
		}
		else {
			ui_image_research[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 950,197,140,97 }, { 447, 1612, 140, 97 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::assassin_locked));
		}
	}
	else {
		ui_image_research[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 950,197,140,97 }, { 447, 1501, 140, 97 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::assassin_unlocked));
	}
}

//Called to create the button or image to research the lawful beast;
void ResearchMenu::CreateLawfulBeastButton() {
	switch (player->civilization)
	{
	case CivilizationType::VIKING:
		if (player->research_lawful_beast == false) {
			if (player->research_cleric == true && player->GetPrayers() >= 10) {
				ui_button_research[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 216,330,140,99 }, { 157, 1171, 140, 99 }, "RESEARCH LAWFUL BEAST", Panel_Fade::panel_fade_in,
					{ 157,1389,140,99 }, { 157,1279,140,99 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::jotnar_unresearched));
			}
			else {
				ui_image_research[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 216,330,140,99 }, { 157, 1610, 140, 99 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
					false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::jotnar_locked));
			}
		}
		else {
			ui_image_research[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 216,330,140,99 }, { 157, 1499, 140, 99 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::jotnar_unlocked));
		}
		break;
	case CivilizationType::GREEK:
		if (player->research_lawful_beast == false) {
			if (player->research_cleric == true && player->GetPrayers() >= 10) {
				ui_button_research[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 216,330,140,99 }, { 302, 1171, 140, 99 }, "RESEARCH LAWFUL BEAST", Panel_Fade::panel_fade_in,
					{ 302,1389,140,99 }, { 302,1279,140,99 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::cyclop_unresearched));
			}
			else {
				ui_image_research[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 216,330,140,99 }, { 302, 1610, 140, 99 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
					false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::cyclop_locked));
			}
		}
		else {
			ui_image_research[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 216,330,140,99 }, { 302, 1499, 140, 99 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::cyclop_unlocked));
		}
		break;
	}
}

//Called to create the button or image to research the chaotic beast;
void ResearchMenu::CreateChaoticBeastButton() {
	switch (player->civilization)
	{
	case CivilizationType::VIKING:
		if (player->research_chaotic_beast == false) {
			if (player->research_assassin == true && player->GetSacrifices() >= 10) {
				ui_button_research[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 933,330,140,98 }, { 737, 1172, 140, 98 }, "RESEARCH CHAOTIC BEAST", Panel_Fade::panel_fade_in,
					{ 737,1390,140,98 }, { 737,1280,140,98 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::draugar_unresearched));
			}
			else {
				ui_image_research[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 933,330,140,98 }, { 737, 1611, 140, 98 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
					false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::draugar_locked));
			}
		}
		else {
			ui_image_research[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 933,330,140,98 }, { 737, 1500, 140, 98 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::draugar_unlocked));
		}
		break;
	case CivilizationType::GREEK:
		if (player->research_chaotic_beast == false) {
			if (player->research_assassin == true && player->GetSacrifices() >= 10) {
				ui_button_research[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 933,330,140,98 }, { 592, 1172, 140, 98 }, "RESEARCH CHAOTIC BEAST", Panel_Fade::panel_fade_in,
					{ 592,1390,140,98 }, { 592,1280,140,98 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::minotaur_unresearched));
			}
			else {
				ui_image_research[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 933,330,140,98 }, { 592, 1611, 140, 98 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
					false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::minotaur_locked));
			}
		}
		else {
			ui_image_research[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 933,330,140,98 }, { 592, 1500, 140, 98 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::minotaur_unlocked));
		}
		break;
	}
}

//Called to create the button or image to research the lawful miracle;
void ResearchMenu::CreateLawfulMiracleButton() {
	if (player->research_lawful_miracle == false) {
		if (player->research_lawful_beast == true && player->GetPrayers() >= 15) {
			ui_button_research[6] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 385,432,140,80 }, { 676, 720, 140, 80 }, "RESEARCH LAWFUL MIRACLE", Panel_Fade::panel_fade_in,
				{ 676,897,140,80 }, { 676,809,140,80 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::miracle_unresearched));
		}
		else {
			ui_image_research[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 385,432,140,80 }, { 676, 1074, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::miracle_locked));
		}
	}
	else {
		ui_image_research[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 385,432,140,80 }, { 676, 986, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::miracle_unlocked));
	}
}

//Called to create the button or image to research the chaotic miracle;
void ResearchMenu::CreateChaoticMiracleButton() {
	if (player->research_chaotic_miracle == false) {
		if (player->research_chaotic_beast == true && player->GetSacrifices() >= 15) {
			ui_button_research[7] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 762,432,140,80 }, { 821, 720, 140, 80 }, "RESEARCH CHAOTIC MIRACLE", Panel_Fade::panel_fade_in,
				{ 821,897,140,80 }, { 821,809,140,80 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::disaster_unresearched));
		}
		else {
			ui_image_research[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 762,432,140,80 }, { 821, 1074, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::disaster_locked));
		}
	}
	else {
		ui_image_research[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 762,432,140,80 }, { 821, 986, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::disaster_unlocked));
	}
}

//Called to create the button or image to research the lawful victory;
void ResearchMenu::CreateLawfulVictoryButton() {
	if (player->research_lawful_victory == false) {
		if (player->research_lawful_miracle == true && player->GetPrayers() >= 25) {
			ui_button_research[8] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 494,301,140,80 }, { 966, 720, 140, 80 }, "RESEARCH LAWFUL VICTORY", Panel_Fade::panel_fade_in,
				{ 966,897,140,80 }, { 966,809,140,80 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::victory1_unresearched));
		}
		else {
			ui_image_research[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 494,301,140,80 }, { 966, 1161, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::victory1_locked));
		}
	}
	else {
		ui_image_research[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 494,301,140,80 }, { 966, 986, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::victory1_unlocked));
	}
}

//Called to create the button or image to research the chaotic victory;
void ResearchMenu::CreateChaoticVictoryButton() {
	if (player->research_chaotic_victory == false) {
		if (player->research_chaotic_miracle == true && player->GetSacrifices() >= 25) {
			ui_button_research[9] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 655,301,140,80 }, { 966, 720, 140, 80 }, "RESEARCH CHAOTIC VICTORY", Panel_Fade::panel_fade_in,
				{ 966,897,140,80 }, { 966,809,140,80 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2, (int)TooltipsAvailable::victory2_unresearched));
		}
		else {
			ui_image_research[9] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 655,301,140,80 }, { 966, 1074, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::victory2_locked));
		}
	}
	else {
		ui_image_research[9] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 655,301,140,80 }, { 966, 986, 140, 80 }, "", Panel_Fade::panel_fade_in, { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2, (int)TooltipsAvailable::victory2_unlocked));
	}
}
