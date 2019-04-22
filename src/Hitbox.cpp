#include "common.h"

Hitbox::Hitbox(SDL_Rect rectangle) {
	oldMin = min = { rectangle.x, rectangle.y + rectangle.h };
	oldMax = max = { rectangle.x + rectangle.w, rectangle.y };
	size = { rectangle.w, rectangle.h };
}