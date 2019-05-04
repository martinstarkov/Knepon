#pragma once

class Hitbox {
public:
	Hitbox(SDL_Rect rectangle);

	Vector2D min;
	Vector2D max;
	Vector2D oldMin;
	Vector2D oldMax;
	Size size;
};
