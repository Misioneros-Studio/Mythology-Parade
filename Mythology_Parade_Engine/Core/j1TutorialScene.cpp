#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "TutorialMessages.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1TutorialScene.h"
#include "j1FadeToBlack.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "EntityManager.h"
#include "Player.h"
#include"QuadTree.h"

j1TutorialScene::j1TutorialScene() : j1Module()
{
	name.append("tutorialscene");
	tutorial_message_data = nullptr;
	quest_done = fortress_selected = monk_created = unit_created = convert_or_kill = level_up = destroy_fortress = false;
	camera_first_position = { 0,0 };
	actual_message = 0;
	for (int i = 0; i < 36; i++) {
		if (i < 4) {
			window_tutorial_message[i] = nullptr;
			if (i < 2) {
				first_message_shown[i] = second_message_shown[i] = third_message_shown[i] = false;
				first_message_height[i] = second_message_height[i] = third_message_height[i] = 0;
			}
		}
		text_tutorial_message[i] = nullptr;
	}
}

// Destructor
j1TutorialScene::~j1TutorialScene()
{}

// Called before render is available
bool j1TutorialScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool j1TutorialScene::Start()
{
	App->render->camera.x = 578;
	App->render->camera.y = -1865;
	tutorial_message_data = new TutorialMessageData();
	tutorial_message_timer.Start();
	message_number = 0;
	return true;
}

// Called each loop iteration
bool j1TutorialScene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1TutorialScene::Update(float dt)
{
	
	if ((message_number == 1 && tutorial_message_timer.ReadSec() >= 6) || (message_number !=1 && message_number < 3 && tutorial_message_timer.ReadSec() >= 3)) {
		if (message_number == 1)
			DeleteTutorialMessage();
		if (message_number == 0)
			CreateTutorialMessage(message_number, true);
		else
			CreateTutorialMessage(message_number);
		message_number++;
		if (message_number == 3) {
			camera_first_position = { App->render->camera.x,App->render->camera.y };
		}
		tutorial_message_timer.Start();
	}
	else if (message_number == 3){
		if ((camera_first_position.x != App->render->camera.x || camera_first_position.y != App->render->camera.y) && quest_done == false) {
			quest_done = true;
		}
		if ((quest_done == false && tutorial_message_timer.ReadSec() >= 40)|| (quest_done == true && tutorial_message_timer.ReadSec() >= 20)) {
			DeleteTutorialMessage();
			CreateTutorialMessage(message_number);
			message_number++;
			CreateTutorialMessage(message_number);
			message_number++;
			quest_done = false;
		}
	}
	else if (message_number == 5) {
		if (fortress_selected == true){
			CreateTutorialMessage(message_number);
			message_number++;
		}
	}
	else if (message_number == 6) {
		if (monk_created == true){
			DeleteTutorialMessage();
			CreateTutorialMessage(message_number);
			message_number++;
			CreateTutorialMessage(message_number);
			message_number++;
			CreateTutorialMessage(message_number);
			message_number++;
		}
	}
	else if (message_number == 9) {
		if (App->entityManager->getPlayer()->research_assassin == true || App->entityManager->getPlayer()->research_cleric == true) {
			CreateTutorialMessage(message_number);
			message_number++;
		}
	}
	else if (message_number == 10) {
		if (unit_created == true) {
			DeleteTutorialMessage();
			CreateTutorialMessage(message_number);
			message_number++;
			CreateTutorialMessage(message_number);
			message_number++;
		}
	}
	else if (message_number == 12) {
		if (convert_or_kill == true) {
			CreateTutorialMessage(message_number);
			message_number++;
		}
	}
	else if (message_number == 13) {
		if (level_up == true) {
			DeleteTutorialMessage();
			CreateTutorialMessage(message_number);
			message_number++;
			tutorial_message_timer.Start();
		}
	}
	else if (message_number == 14) {
		if (tutorial_message_timer.ReadSec() >= 5) {
			CreateTutorialMessage(message_number);
			message_number++;
			CreateTutorialMessage(message_number);
			message_number++;
		}
	}
	else if (message_number == 16) {
		if(destroy_fortress==true)
			App->entityManager->getPlayer()->player_win = true;
	}

	monk_created = false;
	unit_created = false;
	convert_or_kill = false;
	level_up = false;
	destroy_fortress = false;
	return true;
}

// Called each loop iteration
bool j1TutorialScene::PostUpdate()
{

	return true;
}

// Called before quitting
bool j1TutorialScene::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(tutorial_tex);
	DeleteTutorialMessage();

	if (tutorial_message_data != nullptr) {
		delete tutorial_message_data;
		tutorial_message_data = nullptr;
	}

	return true;
}

// Called when creating a tutorial message
void j1TutorialScene::CreateTutorialMessage(int index, bool middle)
{
	SDL_Rect color = { 255,255,255,255 };
	int screen_position = 1;
	if (index == 5 || index == 7 || index == 9 || index == 11 || index == 12 || index == 14) {
		color.y = color.w = 100;
		screen_position = 0;
	}
	int number_message = 3;
	int y = 0;
	int x = 0;
	if (third_message_shown[screen_position] == false)
		number_message = 2;
	else
		y += third_message_height[screen_position];
	if (second_message_shown[screen_position] == false)
		number_message = 1;
	else
		y += second_message_height[screen_position];
	if (first_message_shown == false)
		number_message = 0;
	else
		y += first_message_height[screen_position];
	y += 5;
	uint w, h;
	App->win->GetWindowSize(w, h);
	if (middle == true) {
		x = (int)(w / 2);
		x -= (int)(295 / 2);
		y = (int)((h-130) / 2);
		screen_position = 0;
	}
	TutorialMessage tutorial_message = tutorial_message_data->GetTutorialMessage(index);
	window_tutorial_message[actual_message] = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, 
		{ ((int)w - 295) * screen_position + x ,y,290,(tutorial_message.lines * 18) }, { 1285,11,305,(tutorial_message.lines * 18) }));
	int j = 0;
	for (int i = 1; i <= tutorial_message.lines; i++) {
		int k = i + (actual_message * 9);
		if (tutorial_message.has_title && i == 1) {
			text_tutorial_message[k - 1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, window_tutorial_message[actual_message],
				{ ((int)w - 295) * screen_position + x ,y + (18 * (i - 1)),290,18 }, { 0,0,0,0 }, tutorial_message.title, Panel_Fade::no_one_fade, color));
			j--;
		}
		else {
			text_tutorial_message[k - 1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, window_tutorial_message[actual_message],
				{ ((int)w - 295) * screen_position + x ,y + (18 * (i - 1)),290,18 }, { 0,0,0,0 }, tutorial_message_data->GetLineTutorialMessage(i + j, tutorial_message),
				Panel_Fade::no_one_fade, { 255,255,255,255 }));
		}
	}
	if (middle == false) {
		if (number_message == 0) {
			first_message_shown[screen_position] = true;
			first_message_height[screen_position] = (tutorial_message.lines * 18) + 5;
		}
		else if (number_message == 1) {
			second_message_shown[screen_position] = true;
			second_message_height[screen_position] = (tutorial_message.lines * 18) + 5;
		}
		else if (number_message == 2) {
			third_message_shown[screen_position] = true;
			third_message_height[screen_position] = (tutorial_message.lines * 18) + 5;
		}
	}
	actual_message++;
}

// Called when deleting a tutorial message
void j1TutorialScene::DeleteTutorialMessage()
{
	actual_message = 0;
	for (int i = 35; i >= 0; i--) {
		if (i < 4) {
			if (window_tutorial_message[i] != nullptr) {
				App->gui->DeleteUIElement(window_tutorial_message[i]);
				window_tutorial_message[i] = nullptr;
			}
			if (i < 2) {
				third_message_shown[i] = second_message_shown[i] = first_message_shown[i] = false;
				third_message_height[i] = second_message_height[i] = first_message_height[i] = 0;
			}
		}
		if (text_tutorial_message[i] != nullptr) {
			App->gui->DeleteUIElement(text_tutorial_message[i]);
			text_tutorial_message[i] = nullptr;
		}
	}
}
