#pragma once


#include "Components.h"
#include "../Vector2D.h"


class TrackerComponent : public Component
{
public:
	TransformComponent* transform;
	Vector2D position;

	Vector2D targetPosition;
	Entity* target;
	Vector2D direction;

	TrackerComponent(Entity* target)
	{
		this->target = target;
	}

	~TrackerComponent()
	{

	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		position = transform->position;
		direction.Zero();
	}

	void update() override
	{
		targetPosition = target->getComponent<TransformComponent>().position;
		position = transform->position;
		changeVelocity();
	}

	void changeVelocity()
	{
		direction = targetPosition - position;
		direction.Normalize();

		transform->velocity = direction;
		//std::cout << "direction: " << direction.x << ", " << direction.y << std::endl;
		//std::cout << "targetPos: " << targetPosition.x << ", " << targetPosition.y << std::endl;

	}
};