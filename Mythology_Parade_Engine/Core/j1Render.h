#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include"p2DynArray.h"
#include<unordered_map>

struct RenderLayerInfo 
{
	RenderLayerInfo();
	RenderLayerInfo(SDL_Texture* s_texture, int s_x, int s_y, const SDL_Rect* s_section = NULL, float s_speed = 1.0f, double s_angle = 0, int s_pivot_x = INT_MAX, int s_pivot_y = INT_MAX);
	~RenderLayerInfo();

	SDL_Texture* texture;
	int x;
	int y;
	SDL_Rect section;
	float speed;
	double angle;
	int pivot_x;
	int pivot_y;

};

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;

	// Draw & Blit
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

private:
	//Renders an item of the renderQueue
	bool RenderQueueItems(SDL_Texture* texture, int x, int y, SDL_Rect section = {0, 0, 0, 0}, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;

	//We can use unordered_multimap instead of dynarray, but means more work
	std::unordered_multimap<int,RenderLayerInfo*> renderQueue;

};

#endif // __j1RENDER_H__