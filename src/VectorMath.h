#pragma once
#define MY_PI 3.1415926535897

class VectorMath {
public:
	static VectorMath* operation() {
		if (instance == 0) {
			instance = new VectorMath();
		}
		return instance;
	}
	Vector2D add(Vector2D v1, Vector2D v2) {
		return { v1.x + v2.x, v1.y + v2.y };
	}
	Vector2D subtract(Vector2D v1, Vector2D v2) {
		return { v1.x - v2.x, v1.y - v2.y };
	}
	Vector2D multiplyByScalar(Vector2D v1, double scalar) {
		return { v1.x * scalar, v1.y * scalar };
	}
	double dotProduct(Vector2D v1, Vector2D v2) {
		return (v1.x + v2.x + v1.y + v2.y);
	}
	double angle(Vector2D v1, Vector2D v2) {
		return (acos(dotProduct(v1, v2)));
	}
	double angle(Vector2D vector) {
		return (atan(vector.y / vector.x));
	}
	double degrees(double radians) {
		return (radians * 180 / MY_PI);
	}
	double radians(double degrees) {
		return (degrees * MY_PI / 180);
	}
private:
	static VectorMath* instance;
};