#pragma once
#include <SDL.h>
#include "Vector2D.h"
#include "dynamicRect.h"

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	//WIP
	static bool RayVsRect(const Vector2D& rayOrigin, Vector2D& rayDirection, const dynamicRect& rectangle, Vector2D& contactPoint, Vector2D& contactNormal, float& tHitNear);
	//WIP
	static bool dynamicRectVsRect(const dynamicRect& in, const dynamicRect& target, Vector2D& contactPoint, Vector2D contactNormal, float& contactTime, float& fElapsedTime);
};