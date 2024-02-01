#pragma once
#include "Components.h"
#include "../Vector2D.h"

#include <SDL.h>

class TransformComponent : public Component
{
public:

	Uint32 lastCollisionTime;	//this is a placeholder

	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	enum Face { FACE_FRONT=0, FACE_BACK=1, FACE_LEFT=2, FACE_RIGHT=3 };
	
	Face face = FACE_FRONT;

	int speed = 2;

	bool blocked = false;

	TransformComponent()
	{
		position.Zero();
		lastCollisionTime = 0;
	}

	TransformComponent(int sc)
	{
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.Zero();
	}

	TransformComponent(float x, float y, int height, int width, float scale)
	{
		position.x = x;
		position.y = y;
		this->height = height;
		this->width = width;
		this->scale = scale;
	}

	TransformComponent(float x, float y, int height, int width, float scale, int speed)
	{
		position.x = x;
		position.y = y;
		this->height = height;
		this->width = width;
		this->scale = scale;
		this->speed = speed;
	}

	void init() override
	{
		velocity.Zero();
		
	}

	void update() override
	{
		velocity.Normalize();

		//Vector2D prevPos = position;
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;

		/*For Debugging only
		if (prevPos.x != position.x || prevPos.y != position.y)
		{
			std::cout << "The X Calculation: " << position.x << " = " << prevPos.x << "(" << velocity.x << "*" << speed << ")" << std::endl;
			std::cout << "The Y Calculation: " << position.y << " = " << prevPos.y << "(" << velocity.y << "*" << speed << ")" << std::endl;
		}*/

		if (velocity.x > 0)
		{
			face = FACE_RIGHT;
		}
		else if (velocity.x < 0)
		{
			face = FACE_LEFT;
		}
		else if (velocity.y > 0)
		{
			face = FACE_FRONT;
		}
		else if (velocity.y < 0)
		{
			face = FACE_BACK;
		}

		/*if (prevPos.x != position.x || prevPos.y != position.y)
		{
			std::cout << "The X Calculation: " << position.x << " = " << prevPos.x << "(" << velocity.x << "*" << speed << ")" << std::endl;
			std::cout << "The Y Calculation: " << position.y << " = " << prevPos.y << "(" << velocity.y << "*" << speed << ")" << std::endl;
		}*/
	}

};