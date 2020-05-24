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

#include"QuadTree.h"

j1TutorialScene::j1TutorialScene() : j1Module()
{
	name.append("tutorialscene");
	tutorial_message_data = nullptr;
	first_message_shown = second_message_shown = false;
	first_message_height = second_message_height = 0;
	for (int i = 0; i < 30; i++) {
		if (i < 3) {
			button_tutorial_message[i] = nullptr;
			window_tutorial_message[i] = nullptr;
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
	tutorial_message_data = new TutorialMessageData;
	tutorial_message_timer.Start();
	CreateTutorialMessage(0);
	CreateTutorialMessage(1);
	CreateTutorialMessage(2);
	message_number = 3;
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
	
	if (tutorial_message_timer.ReadSec() >= 5) {
		DeleteTutorialMessage(true, true, true);
		CreateTutorialMessage(message_number);
		LOG("%d", message_number);
		if (message_number < 12)
			message_number++;
		CreateTutorialMessage(message_number);
		if (message_number < 12)
			message_number++;
		CreateTutorialMessage(message_number);
		if (message_number < 12)
			message_number++;
		tutorial_message_timer.Start();
	}
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
	DeleteTutorialMessage(true, true, true);

	if (tutorial_message_data != nullptr) {
		delete tutorial_message_data;
		tutorial_message_data = nullptr;
	}

	return true;
}

// Called when creating a tutorial message
void j1TutorialScene::CreateTutorialMessage(int index)
{
	SDL_Rect color = { 255,255,255,255 };
	if (index == 5 || index == 7 || index == 9 || index == 11 || index == 12)
		color.y = color.w = 150;
	int number_message = 2;
	int y = 0;
	if (second_message_shown == false)
		number_message = 1;
	else
		y += second_message_height;
	if (first_message_shown == false)
		number_message = 0;
	else
		y += first_message_height;
	y += 5;
	uint w, h;
	App->win->GetWindowSize(w, h);
	TutorialMessage tutorial_message = tutorial_message_data->GetTutorialMessage(index);
	window_tutorial_message[number_message] = static_cast<WindowUI*>(App->gui->CreateUIElement(Type::WINDOW, nullptr, { (int)w- 295,y,290,(tutorial_message.lines * 18) },
		{ 1285,11,305,(tutorial_message.lines * 18) }));
	int j = 0;
	for (int i = 1; i <= tutorial_message.lines; i++) {
		int k = i + (number_message * 10);
		if (tutorial_message.has_title && i == 1) {
			text_tutorial_message[k - 1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, window_tutorial_message[number_message], { (int)w - 295,y + (18 * (i - 1)),290,18 }, { 0,0,0,0 },
				tutorial_message.title, Panel_Fade::no_one_fade, color));
			j--;
		}
		else {
			text_tutorial_message[k - 1] = static_cast<TextUI*>(App->gui->CreateUIElement(Type::TEXT, window_tutorial_message[number_message], { (int)w - 295,y + (18 * (i - 1)),290,18 }, { 0,0,0,0 },
				tutorial_message_data->GetLineTutorialMessage(i + j, tutorial_message), Panel_Fade::no_one_fade, color));
		}
	}
	if (number_message == 0) {
		first_message_shown = true;
		first_message_height = (tutorial_message.lines * 18)+5;
	}
	else if(number_message==1){
		second_message_shown = true;
		second_message_height = (tutorial_message.lines * 18)+5;
	}
}

// Called when deleting a tutorial message
void j1TutorialScene::DeleteTutorialMessage(bool first_message, bool second_message, bool third_message)
{
	if (third_message == true) {
		for (int i = 29; i >= 20; i--) {
			if (text_tutorial_message[i] != nullptr) {
				App->gui->DeleteUIElement(text_tutorial_message[i]);
				text_tutorial_message[i] = nullptr;
			}
		}
		if (button_tutorial_message[2] != nullptr) {
			App->gui->DeleteUIElement(button_tutorial_message[2]);
			button_tutorial_message[2] = nullptr;
		}
		if (window_tutorial_message[2] != nullptr) {
			App->gui->DeleteUIElement(window_tutorial_message[2]);
			window_tutorial_message[2] = nullptr;
		}
	}
	if(second_message==true){
		for (int i = 19; i >= 10; i--) {
			if (text_tutorial_message[i] != nullptr) {
				App->gui->DeleteUIElement(text_tutorial_message[i]);
				text_tutorial_message[i] = nullptr;
			}
		}
		if (button_tutorial_message[1] != nullptr) {
			App->gui->DeleteUIElement(button_tutorial_message[1]);
			button_tutorial_message[1] = nullptr;
		}
		if (window_tutorial_message[1] != nullptr) {
			App->gui->DeleteUIElement(window_tutorial_message[1]);
			window_tutorial_message[1] = nullptr;
		}
		second_message_shown = false;
	}
	if (first_message == true) {
		for (int i = 9; i >= 0; i--) {
			if (text_tutorial_message[i] != nullptr) {
				App->gui->DeleteUIElement(text_tutorial_message[i]);
				text_tutorial_message[i] = nullptr;
			}
		}
		if (button_tutorial_message[0] != nullptr) {
			App->gui->DeleteUIElement(button_tutorial_message[0]);
			button_tutorial_message[0] = nullptr;
		}
		if (window_tutorial_message[0] != nullptr) {
			App->gui->DeleteUIElement(window_tutorial_message[0]);
			window_tutorial_message[0] = nullptr;
		}
		first_message_shown = false;
	}
}
