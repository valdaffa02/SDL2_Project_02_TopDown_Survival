#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int range, int speed, Vector2D velocity) : range(range), speed(speed), velocity(velocity)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;

		//float shootingAngle = atan2(velocity.y, velocity.x);
		//std::cout << "Shooting Angle: " << shootingAngle << " radians" << std::endl;
	}

	void update() override
	{
		distance += speed;

		if (distance > range)
		{
			/*For Debugging
			std::cout << "Point of impact: " << transform->position.x << ", " << transform->position.y << std::endl;
			std::cout << "Last Vel: " << transform->velocity.x << ", " << transform->velocity.y << std::endl;
			std::cout << "Speed " << transform->speed << std::endl;*/
			entity->destroy();
		}

		
	}

private:

	TransformComponent* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;


};