#pragma once

struct Animation
{

	int index;
	int frames;
	int speed;

	Animation() {}
	Animation(int index, int frames, int speed)
	{
		/*
		* Index: Vertical index of the animation in spritesheet with the assumption 1 row = 1 animation; 0 is the first row
		*/
		this->index = index;
		this->frames = frames;
		this->speed = speed;
	}

};