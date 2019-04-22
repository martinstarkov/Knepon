#include "common.h"

Hitbox::Hitbox(Rectangle dimensions) {
	oldMin = min = { dimensions.x, dimensions.y + dimensions.h };
	oldMax = max = { dimensions.x + dimensions.w, dimensions.y };
	size = { dimensions.w, dimensions.h };
}