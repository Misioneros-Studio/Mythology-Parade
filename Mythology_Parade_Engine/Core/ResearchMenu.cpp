#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include"j1Gui.h"
#include "j1Scene.h"
#include "Player.h"
#include "ResearchMenu.h"

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
	for (int i = 0; i < 12; i++) {
		if (i < 11) {
			if (i < 2) {
				ui_text_research[i] = nullptr;
			}
			ui_image_research[i] = nullptr;
		}
		ui_button_research[i] = nullptr;
	}
}

//Called when clicking the research button
void ResearchMenu::ActivateResearchMenu() {
	ui_research_window = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { 130,10,1025,570 }, { 13,20,1025,570 }, "", { 0,0,0,0 }, { 0,0,0,0 }, false, { 0,0,0,0 },
		nullptr, 0, false, -1.0f, 2));
	ui_button_research[11] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 520,512,237,38 }, { 787,240,237,38 }, "CLOSE RESEARCH", { 787,342,237,38 },
		{ 787,291,237,38 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE));
	ui_text_research[0] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 619,524,237,38 }, { 0,0,100,100 }, "Close", { 0,0,0,255 }));
	ui_text_research[1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, nullptr, { 583,22,237,38 }, { 0,0,100,100 }, "RESEARCH", { 255,255,255,255 }, { 1,0,0,0 }));
	CreateMonasteryButton();
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
		for (int i = 11; i >= 0; i--) {
			if (i < 11) {
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

//Called to create the button or image to research the monastery;
void ResearchMenu::CreateMonasteryButton() {
	if (player->research_monastery == false) {
		ui_button_research[0] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 613,70,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH MONASTERY",
			{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
	}
	else {
		ui_image_research[0] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 617,70,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the temple;
void ResearchMenu::CreateTempleButton() {
	if (player->research_temple == false) {
		if (player->research_monastery == true) {
			ui_button_research[1] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 305,140,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH TEMPLE",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,140,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[1] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,140,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the encampment;
void ResearchMenu::CreateEncampmentButton() {
	if (player->research_encampment == false) {
		if (player->research_monastery == true) {
			ui_button_research[2] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 949,140,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH ENCAMPMENT",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,140,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[2] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,140,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the cleric;
void ResearchMenu::CreateClericButton() {
	if (player->research_cleric == false) {
		if (player->research_temple == true) {
			ui_button_research[3] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 305,210,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH CLERIC",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,210,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[3] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,210,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the assassin;
void ResearchMenu::CreateAssassinButton() {
	if (player->research_assassin == false) {
		if (player->research_encampment == true) {
			ui_button_research[4] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 949,210,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH ASSASSIN",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,210,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[4] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,210,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the lawful beast;
void ResearchMenu::CreateLawfulBeastButton() {
	if (player->research_lawful_beast == false) {
		if (player->research_cleric == true) {
			ui_button_research[5] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 305,280,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH LAWFUL BEAST",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,280,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[5] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,280,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the chaotic beast;
void ResearchMenu::CreateChaoticBeastButton() {
	if (player->research_chaotic_beast == false) {
		if (player->research_assassin == true) {
			ui_button_research[6] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 949,280,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH CHAOTIC BEAST",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,280,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[6] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,280,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the lawful miracle;
void ResearchMenu::CreateLawfulMiracleButton() {
	if (player->research_lawful_miracle == false) {
		if (player->research_lawful_beast == true) {
			ui_button_research[7] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 305,350,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH LAWFUL MIRACLE",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,350,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[7] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,350,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the chaotic miracle;
void ResearchMenu::CreateChaoticMiracleButton() {
	if (player->research_chaotic_miracle == false) {
		if (player->research_chaotic_beast == true) {
			ui_button_research[8] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 949,350,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH CHAOTIC MIRACLE",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,350,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[8] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,350,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the lawful victory;
void ResearchMenu::CreateLawfulVictoryButton() {
	if (player->research_lawful_victory == false) {
		if (player->research_lawful_miracle == true) {
			ui_button_research[9] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 305,420,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH LAWFUL VICTORY",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[9] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,420,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[9] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 305,420,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}

//Called to create the button or image to research the chaotic victory;
void ResearchMenu::CreateChaoticVictoryButton() {
	if (player->research_chaotic_victory == false) {
		if (player->research_chaotic_miracle == true) {
			ui_button_research[10] = static_cast<ButtonUI*>(App->gui->CreateUIElement(Type::BUTTON, ui_research_window, { 949,420,54,54 }, { 1059, 13, 54, 54 }, "RESEARCH CHAOTIC VICTORY",
				{ 1183,13,54,54 }, { 1121,13,54,54 }, false, { 0,0,0,0 }, App->scene, (int)UI_Audio::CLOSE, false, -1.0f, 2));
		}
		else {
			ui_image_research[10] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,420,54,54 }, { 1121, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
				false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
		}
	}
	else {
		ui_image_research[10] = static_cast<ImageUI*>(App->gui->CreateUIElement(Type::IMAGE, ui_research_window, { 949,420,54,54 }, { 1059, 74, 54, 54 }, "", { 0,0,0,0 }, { 0,0,0,0 },
			false, { 0,0,0,0 }, nullptr, 0, false, -1.0f, 2));
	}
}
