#ifndef __j1LOGOSCENE__
#define __j1LOGOSCENE__

#include "j1Module.h"
#include "j1Timer.h"

struct SDL_Texture;

class j1LogoScene : public j1Module
{
public:

	j1LogoScene();

	// Destructor
	virtual ~j1LogoScene();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Change to Title Scene
	void ChangeToTitleScene();

private:
	SDL_Texture* debug_tex;
	j1Timer timer_logo;
	int angle = 0;
	int sfx_logo;
};

#endif // __j1LOGOSCENE_H__