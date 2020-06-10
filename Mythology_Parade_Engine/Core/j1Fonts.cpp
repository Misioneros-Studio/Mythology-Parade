#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Fonts.h"

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

j1Fonts::j1Fonts() : j1Module()
{
	name.append("fonts");
	default_font = nullptr;
	default_title = nullptr;
	surface = nullptr;
	texture = nullptr;
}

// Destructor
j1Fonts::~j1Fonts()
{}

// Called before render is available
bool j1Fonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if(TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default_font = Load(path, size);
		default_title = Load(path, 24);
	}
	active = true;

	return ret;
}

// Called before quitting
bool j1Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	for (std::list<TTF_Font*>::iterator it = fonts.begin(); it != fonts.end(); it++)
	{
		TTF_CloseFont(it._Ptr->_Myval);
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const j1Fonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if(font == nullptr)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
		font = nullptr;
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

// Print text using font
SDL_Texture* j1Fonts::Print(const char* text, SDL_Color color, TTF_Font* font)
{

	texture = nullptr;

	surface = TTF_RenderText_Blended(font? font : default_font, text, color);

	if(surface == nullptr || surface->format == nullptr || surface->pixels == nullptr)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else if (surface != nullptr)
	{
		texture = App->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
		surface = nullptr;
	}

	return texture;
}

// calculate size of a text
bool j1Fonts::CalcSize(const char* text, int& width, int& height, TTF_Font* font) const
{
	bool ret = false;

	if(TTF_SizeText((font) ? font : default_font, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}