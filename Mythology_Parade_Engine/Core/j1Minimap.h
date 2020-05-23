#ifndef _j1MINIMAP_H
#define _j1MINIMAP_H
#include "j1Module.h"
#include "p2Point.h"
#include "j1Timer.h"
#include "SDL/include/SDL.h"

enum class Corner {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};


class Entity;
class j1Minimap : public j1Module {
public:
	j1Minimap();
	~j1Minimap();

	bool Start();
	bool Awake(pugi::xml_node& config);
	bool PostUpdate();
	bool CleanUp();

	bool CreateMinimap();
	iPoint WorldToMinimap(int x, int y);
	iPoint ScreenToMinimapToWorld(int x, int y);
	void EntityAttacked(Entity*);

public:
	iPoint position;
	int width;
	int height;
	float scale;
	SDL_Texture* texture;
	bool show_damage_area;
	iPoint damage_area;
private:
	int map_width;
	int map_height;
	int margin;
	Corner corner;
	SDL_Rect minimap_test_rect;
	j1Timer timer_fow;
	bool reset_timer_fow;
	bool update_minimap_fow;
	SDL_Texture* texture_fow;

};

#endif // !_j1MINIMAP_H