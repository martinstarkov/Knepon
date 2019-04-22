#pragma once

class Hitbox {
public:
	Vector2D min;
	Vector2D max;
	Vector2D oldMin;
	Vector2D oldMax;
	Vector2D size;
	Hitbox(Rectangle dimensions);
};
