#include "j1Minimap.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "FoWManager.h"

j1Minimap::j1Minimap() : j1Module() {
	name.append("minimap");
	corner = Corner::TOP_LEFT;
	height = 0;
	width = 0;
	map_height = 0;
	map_width = 0;
	margin = 0;
	scale = 0;
	texture_fow = nullptr;
	texture = nullptr;
	minimap_test_rect = { 0,0,4,4 };
	update_minimap_fow = true;
	reset_timer_fow = true;
	show_damage_area = false;

}

j1Minimap::~j1Minimap() {
}

bool j1Minimap::Awake(pugi::xml_node& config) {
	active = false;
	
	uint window_width, window_height;

	//width
	width = config.child("measurements").attribute("width").as_int();

	//corner
	std::string corner_string(config.child("measurements").attribute("corner").as_string());
	margin = config.child("measurements").attribute("margin").as_int();

	if (corner_string == "top_left") {
		corner = Corner::TOP_LEFT;
	}
	else if (corner_string == "top_right") {
		corner = Corner::TOP_RIGHT;
	}
	else if (corner_string == "bottom_left") {
		corner = Corner::BOTTOM_LEFT;
	}
	else if (corner_string == "bottom_right") {
		corner = Corner::BOTTOM_RIGHT;
	}

	return true;
}

bool j1Minimap::Start() {
	bool ret = true;
	uint window_width, window_height;
	App->win->GetWindowSize(window_width, window_height);

	map_width = App->map->data.tile_width * App->map->data.width;
	map_height = App->map->data.tile_height * App->map->data.height;
	scale = ((width) / ((float)map_width));
	height = (map_height)*scale;

	texture = SDL_CreateTexture(App->render->renderer, SDL_GetWindowPixelFormat(App->win->window), SDL_TEXTUREACCESS_TARGET, 1.05f * width, 1.05f * height);
	SDL_SetRenderTarget(App->render->renderer, texture);
	CreateMinimap();
	SDL_SetRenderTarget(App->render->renderer, NULL);

	//calculate position depending on the chosen corner
	switch (corner)
	{
	case Corner::TOP_LEFT:
		position.x = margin;
		position.y = margin;
		break;
	case Corner::TOP_RIGHT:
		position.x = window_width - width - margin;
		position.y = margin;
		break;
	case Corner::BOTTOM_LEFT:
		position.x = margin;
		position.y = window_height - height - margin;
		break;
	case Corner::BOTTOM_RIGHT:
		position.x = window_width - width - margin;
		position.y = window_height - height - margin;
		break;
	}

	return ret;
}

bool j1Minimap::PostUpdate() {
	if (reset_timer_fow == true) {
		reset_timer_fow = false;
		timer_fow.Start();
	}
	else if (timer_fow.ReadSec() >= 1) {
		update_minimap_fow = true;
		reset_timer_fow = true;
	}
	App->render->Blit(texture, position.x, position.y, NULL, 0.0, 0);
	for (unsigned i = 0; i < App->entityManager->entities.size(); i++)
	{
		for (std::list<Entity*>::iterator it = App->entityManager->entities[(EntityType)i].begin(); it != App->entityManager->entities[(EntityType)i].end(); it++)
		{
			Entity* ent = it._Ptr->_Myval;
			if (ent->type == EntityType::UNIT || ent->type == EntityType::BUILDING) {
				fPoint pos_map = ent->position;
				fPoint fow_pos = App->map->WorldToMap((int)ent->position.x, (int)ent->position.y);
				if (App->fowManager->CheckTileVisibility({ (int)fow_pos.x,(int)fow_pos.y }) || ent->shown_minimap == true) {
					iPoint pos_minimap = WorldToMinimap((int)pos_map.x, (int)pos_map.y);
					int x = pos_minimap.x;
					int y = pos_minimap.y;
					int red = 255;
					int blue = 255;
					int green = 255;
					int w = 4;
					int h = 4;
					if (ent->type == EntityType::UNIT) {
						green = 0;
						w = h = 2;
						if (ent->civilization == App->entityManager->getPlayer()->civilization) {
							red = 0;
						}
						else {
							blue = 0;
						}
					}
					else {
						Building* building = static_cast<Building*>(ent);
						if (building->buildingStatus != BuildingStatus::DESTROYED) {
							if (ent->civilization == App->entityManager->getPlayer()->civilization) {
								red = 0;
							}
							else {
								blue = 0;
								green = 100;
							}
							ent->shown_minimap = true;
						}
					}
					App->render->DrawQuad({ x,y,w,h }, red, green, blue, 255, true, false);
				}
			}
		}
	}
	
	if (update_minimap_fow == true) {
		SDL_DestroyTexture(texture_fow);
		texture_fow = nullptr;
		uint win_w, win_h;
		App->win->GetWindowSize(win_w, win_h);
		texture_fow = SDL_CreateTexture(App->render->renderer, SDL_GetWindowPixelFormat(App->win->window), SDL_TEXTUREACCESS_TARGET, 1.05f * width, 1.05f * height);
		SDL_SetTextureBlendMode(texture_fow, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(App->render->renderer, texture_fow);
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 0);
		SDL_RenderClear(App->render->renderer);
		SDL_RenderFillRect(App->render->renderer, NULL);
		App->render->DrawQuad({ 0,0,(int)win_w,(int)win_h }, 0, 0, 0, 255, false, false);
		for (int x = 0; x < App->map->data.width; x++) {
			for (int y = 0; y < App->map->data.height; y++) {
				if ((!App->fowManager->CheckTileVisibility({ x,y }))) {
					int mid_width = width / 2;
					float width_calculated = (float)mid_width / (float)(App->map->data.width);
					float height_calculated = (float)height / (float)(App->map->data.height * 2);
					int x2 = mid_width + ((x - y) * width_calculated);
					int y2 = (x + y) * height_calculated;
					if ((!App->fowManager->CheckTileVisibilityWithoutCountingShroud({ x,y }))) {
						App->render->DrawQuad({ x2,y2,1,1 }, 0, 0, 0, 255, true, false);
					}
					else {
						App->render->DrawQuad({ x2,y2,1,1 }, 0, 0, 0, 50, true, false);
					}
				}
			}
		}
		SDL_SetRenderTarget(App->render->renderer, NULL);
		update_minimap_fow = false;
	}

	App->render->Blit(texture_fow, position.x, position.y, NULL, 0.0, 0);

	return true;
}

bool j1Minimap::CreateMinimap() {

	bool ret = true;
	for (std::list<MapLayer*>::iterator it = App->map->data.downLayers.begin(); ret == true && it != App->map->data.downLayers.end(); it++)
	{
		MapLayer* layer = it._Ptr->_Myval;

		if (layer->properties.Get("Nodraw") != 0)
			continue;

		int half_width = map_width * 0.5f;

		for (int y = 0; y < App->map->data.height; ++y)
		{
			for (int x = 0; x < App->map->data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = App->map->GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = App->map->MapToWorld(x, y);
					App->render->Blit(tileset->texture, pos.x + half_width, pos.y, &r, 0.0f, (0.0), 2147483647, 2147483647, scale);
				}
			}
		}
	}

	for (std::list<MapLayer*>::iterator it = App->map->data.topLayers.begin(); ret == true && it != App->map->data.topLayers.end(); it++)
	{
		MapLayer* layer = it._Ptr->_Myval;

		if (layer->properties.Get("Nodraw") != 0)
			continue;

		int half_width = map_width * 0.5f;

		for (int y = 0; y < App->map->data.height; ++y)
		{
			for (int x = 0; x < App->map->data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = App->map->GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = App->map->MapToWorld(x, y);
					App->render->Blit(tileset->texture, pos.x + half_width, pos.y, &r, 0.0f, (0.0), 2147483647, 2147483647, scale);
				}
			}
		}
	}
	
	return true;
}

bool j1Minimap::CleanUp() 
{
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(texture_fow);
	return true;
}

iPoint j1Minimap::WorldToMinimap(int x, int y) {
	iPoint minimap_position;
	minimap_position.x = position.x + width * 0.5f + x * scale;
	minimap_position.y = position.y + y * scale;

	return minimap_position;
}

iPoint j1Minimap::ScreenToMinimapToWorld(int x, int y) {
	iPoint minimap_position;
	minimap_position.x = (x - position.x - width * 0.5f) / scale;
	minimap_position.y = (y - position.y) / scale;

	return minimap_position;
}

void j1Minimap::EntityAttacked(Entity* entity)
{
	if (show_damage_area == false) {
		if (entity->civilization == App->entityManager->getPlayer()->civilization) {
			fPoint pos = entity->position;
			SDL_Rect cam = App->render->camera;
			iPoint screen_pos=App->render->WorldToScreen((int)pos.x, (int)pos.y);
			if (screen_pos.x<0 || screen_pos.x>cam.w || screen_pos.y<0 || screen_pos.y>cam.h) {
				show_damage_area = true;
				damage_area = WorldToMinimap( (int)pos.x,(int)pos.y);
			}
		}
	}
}
