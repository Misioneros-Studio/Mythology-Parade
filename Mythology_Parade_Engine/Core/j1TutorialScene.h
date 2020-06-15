#ifndef __j1TUTORIALSCENE_H__
#define __j1TUTORIALSCENE_H__

#include "j1Module.h"
#include "j1Timer.h"
#include "SDL/include/SDL.h"
#include"p2Point.h"
#include "j1Timer.h"

struct SDL_Rect;



struct SDL_Texture;
class ImageUI;
class WindowUI;
class ButtonUI;
class TextUI;
class QuadTree;
class Entity;
class Building;
enum class UnitType;
enum BuildingType;
enum CivilizationType;
class HUD;
class ResearchMenu;
class TutorialMessageData;


class j1TutorialScene : public j1Module
{
public:

	j1TutorialScene();

	// Destructor
	virtual ~j1TutorialScene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

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

	// Called when creating a tutorial message
	void CreateTutorialMessage(int, bool = false);

	// Called when deleting a tutorial message
	void DeleteTutorialMessage();

public:

	SDL_Texture* tutorial_tex;
	bool fortress_selected;
	bool monk_created;
	bool unit_created;
	bool convert_or_kill;
	bool level_up;
	bool destroy_fortress;

private:
	TutorialMessageData* tutorial_message_data;
	WindowUI* window_tutorial_message[4];
	TextUI* text_tutorial_message[36];
	bool first_message_shown[2];
	bool second_message_shown[2];
	bool third_message_shown[2];
	int first_message_height[2];
	int second_message_height[2];
	int third_message_height[2];
	j1Timer tutorial_message_timer;
	int message_number;
	iPoint camera_first_position;
	bool quest_done;
	int actual_message;
};

#endif // __j1TUTORIALSCENE_H__