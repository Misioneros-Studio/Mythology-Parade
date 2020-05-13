#pragma once

#ifndef __CLASSICANIMATION_H__
#define __CLASSICANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1App.h"

#define MAX_FRAMES 55

class Frame
{
public:
	float maxFrames;

	SDL_Rect frame;

	int pivotPositionX;
	int pivotPositionY;

private:
	int lastCollider = 0;

public:


};

class ClassicAnimation
{
public:
	bool loop = true;
	Frame frames[MAX_FRAMES];
	float speed = 1;

private:
	float framesPassed = 0;
	int currentFrame = 0;
	int lastFrame = 0;
	int loops = 0;


public:


	void PushBack(const SDL_Rect& rect, const int maxFrames, int pivotPositionX, int pivotPositionY) {

		frames[lastFrame].frame = rect;
		frames[lastFrame].maxFrames = maxFrames;
		frames[lastFrame].pivotPositionX = pivotPositionX;
		frames[lastFrame].pivotPositionY = pivotPositionY;


		lastFrame++;
	}


	Frame& GetCurrentFrame(float dt)
	{
		if (framesPassed * dt*speed < frames[currentFrame].maxFrames * dt*speed)
			framesPassed += dt*speed;
		else
		{
			framesPassed = 0;
			currentFrame++;
		}


		if (currentFrame >= lastFrame)
		{
			currentFrame = (loop) ? 0.0f : lastFrame - 1;
			loops++;
		}


		return frames[currentFrame];
	}


	SDL_Rect& GetCurrentFrameBox(float dt)
	{
		return GetCurrentFrame(dt).frame;
	}


	SDL_Rect& GetRect()
	{
		return this->frames[0].frame;
	}


	bool Finished() const
	{
		return loops > 0;
	}


	void ResetAnimation()
	{
		framesPassed = 0;
		lastFrame = 0;
		currentFrame = 0;
	}


};

#endif
