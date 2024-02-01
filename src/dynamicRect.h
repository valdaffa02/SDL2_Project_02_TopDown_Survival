#include <SDL.h>
#include "Vector2D.h"


struct dynamicRect : public SDL_Rect
{
	Vector2D velocity;

	dynamicRect() = default;
	dynamicRect(int x, int y, int w, int h, Vector2D velocity) : SDL_Rect::SDL_Rect{ x, y, w, h }, velocity(velocity) {}
	dynamicRect(SDL_Rect rectangle, Vector2D velocity) : SDL_Rect::SDL_Rect{ rectangle }, velocity(velocity) {}
};