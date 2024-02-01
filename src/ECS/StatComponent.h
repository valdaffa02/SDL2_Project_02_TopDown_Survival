#pragma once

#include "Components.h"



class StatComponent : public Component
{
public:
	int health[2];		//index 0 for current health, index 1 for max health;
	int speed;
	int attackDmg;
	bool isAlive;

	StatComponent(int maxHealth, int speed, int attackDmg)
	{
		health[0] = health[1] = maxHealth;
		this->speed = speed;
		this->attackDmg = attackDmg;
	}

	~StatComponent()
	{

	}


	void init() override
	{
		isAlive = true;
	}

	void update() override
	{
		if (health[0] <= 0)
		{
			isAlive = false;
		}

		/*if (!isAlive)
		{
			entity->destroy();
		}*/
	}
};