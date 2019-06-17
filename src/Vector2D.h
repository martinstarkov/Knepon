#pragma once

struct Vector2D {
public:
	double x, y;
	Vector2D operator +(Vector2D vec2) {
		return { x + vec2.x, y + vec2.y };
	}
	Vector2D operator +(double scalar) {
		return { x + scalar, y + scalar };
	}
	Vector2D operator -() {
		return { -x, -y };
	}
	Vector2D operator -(Vector2D vec2) {
		return { x - vec2.x, y - vec2.y };
	}
	Vector2D operator -(double scalar) {
		return { x - scalar, y - scalar };
	}
	Vector2D abs() {
		return { std::abs(x), std::abs(y) };
	}
	Vector2D floor() {
		return { std::floor(x), std::floor(y) };
	}
	Vector2D operator /(double scalar) {
		return { x / scalar, y / scalar };
	}
	Vector2D operator /(Vector2D vec2) {
		return { x / vec2.x, y / vec2.y };
	}
	Vector2D operator *(double scalar) {
		return { x * scalar, y * scalar };
	}
	//cross product
	double operator *(Vector2D vec2) {
		return (x * vec2.y - y * vec2.x);
	}
	Vector2D operator ++() {
		return { ++x, ++y };
	}
	Vector2D operator --() {
		return { --x, --y };
	}
	double magnitude() {
		return sqrt(x * x + y * y);
	}
	double dotProduct(Vector2D vec2) {
		return x * vec2.x + y * vec2.y;
	}
	Vector2D normalize() {
		return *this / magnitude();
	}
	Vector2D tangent() {
		return { -y, x };
	}
	static double slope(Vector2D vec1, Vector2D vec2) {
		return (vec2.y - vec1.y) / (vec2.x - vec1.x);
	}
	double distance(Vector2D vec2) {
		return std::abs((*this - vec2).magnitude());
	}
	bool isZero() {
		return x == 0 && y == 0;
	}
	std::string toString() {
		return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
	}
	void print() {
		std::cout << "Vector: " << toString() << std::endl;
	}
};