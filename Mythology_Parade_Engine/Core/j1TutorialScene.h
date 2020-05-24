#ifndef __j1TUTORIALSCENE_H__
#define __j1TUTORIALSCENE_H__

#include "j1Module.h"
#include "j1Timer.h"
#include "SDL/include/SDL.h"
#include"p2Point.h"

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

public:

	SDL_Texture* tutorial_tex;
	TutorialMessageData* tutorial_message_data;

};

#endif // __j1TUTORIALSCENE_H__