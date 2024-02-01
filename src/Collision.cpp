#include "Collision.h"
#include "ECS/ColliderComponent.h"


bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

//WIP
bool Collision::RayVsRect(const Vector2D& rayOrigin, Vector2D& rayDirection, const dynamicRect& rectangle, Vector2D& contactPoint, Vector2D& contactNormal, float& tHitNear)
{
	Vector2D tNear = {((rectangle.x - rayOrigin.x) / rayDirection.x), ((rectangle.y - rayOrigin.y) / rayDirection.y)};
	Vector2D tFar = {(((rectangle.x + rectangle.w) - rayOrigin.x) / rayDirection.x), (((rectangle.y + rectangle.h) - rayOrigin.y) / rayDirection.y)};


	if (tNear.x > tFar.x)
		std::swap(tNear.x, tFar.x);
	if (tNear.y > tFar.y)
		std::swap(tNear.y, tFar.y);

	if (tNear.x > tFar.y || tNear.y > tFar.x)
		return false;

	tHitNear = std::max(tNear.x, tNear.y);
	float tHitFar = std::min(tFar.x, tFar.y);

	if (tHitFar < 0)
		return false;

	contactPoint.x = rayOrigin.x + tHitNear * rayDirection.x;
	contactPoint.y = rayOrigin.y + tHitNear * rayDirection.y;

	if (tNear.x > tNear.y)
		if (rayDirection.x < 0)
			contactNormal = { 1, 0 };
		else
			contactNormal = { -1, 0 };
	if (tNear.x < tNear.y)
		if(rayDirection.y < 0)
			contactNormal = { 0, 1 };
		else
			contactNormal = { 0, -1 };

	return true;
}

//WIP
bool Collision::dynamicRectVsRect(const dynamicRect& in, const dynamicRect& target, Vector2D& contactPoint, Vector2D contactNormal, float& contactTime, float& fElapsedTime)
{
	/*if (in.velocity.x == 0 && in.velocity.y == 0)
		return false;
	*/

	dynamicRect expandedTarget;
	expandedTarget.x = target.x - in.w / 2;
	expandedTarget.y = target.y - in.h / 2;

	expandedTarget.w = target.w + in.w;
	expandedTarget.h = target.h + in.h;

	Vector2D rayOrigin = { static_cast<float>(in.x + in.w / 2), static_cast<float>(in.y + in.h / 2) };
	Vector2D rayDirection = { in.velocity };

	if (RayVsRect(rayOrigin, rayDirection * fElapsedTime, expandedTarget, contactPoint, contactNormal, contactTime))
	{
		if (contactTime <= 1.0f)
			return true;
	}

	return false;

}