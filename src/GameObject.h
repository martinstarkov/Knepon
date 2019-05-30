#pragma once
#include "TextureManager.h"
#include "Vector2D.h"

extern const int TILE_WIDTH, TILE_HEIGHT;

struct Drawable {
	virtual void draw() = 0;
};
struct Updateable {
	virtual void update(double dt) = 0;
};

class Rectangle {
public:
	Rectangle() {}
	Rectangle(Vector2D aPosition, Vector2D aSize) : position(aPosition), size(aSize) {}
	Vector2D position = { 0, 0 };
	Vector2D size = { 0, 0 };
};

class GameObject {
public:
	GameObject(std::string aName, Vector2D aPosition);
	~GameObject() {}
	Vector2D getPosition() {
		return position;
	}
	void setPosition(Vector2D newPosition) {
		position = newPosition;
	}
	Vector2D getSize() {
		return size;
	}
	Vector2D getTilePosition() {
		return tilePosition;
	}
	void setTilePosition(Vector2D newTilePosition) {
		tilePosition = newTilePosition;
	}
	bool getOverlap() {
		return overlap;
	}
	std::string getName() {
		return name;
	}
protected:
	Vector2D size = { double(TILE_WIDTH), double(TILE_HEIGHT) };
	std::string name = "object";
	bool overlap = false;
	Vector2D position = { 0, 0 };
	Vector2D tilePosition = { 0, 0 };
};

class DGameObject : virtual public GameObject, public Drawable {
public:
	DGameObject(std::string aName, Vector2D aPosition, std::string aTexture, SDL_RendererFlip aDirection = SDL_FLIP_NONE);
	~DGameObject() {}
	virtual void draw();
protected:
	std::string texture = "";
	SDL_RendererFlip direction = SDL_FLIP_NONE;
};

class UGameObject : virtual public GameObject, public Updateable {
public:
	UGameObject(std::string aName, Vector2D aPosition, std::string aType);
	~UGameObject() {}
	virtual void update(double dt);
protected:
	std::string type = "";
};

class DUGameObject : virtual public DGameObject, virtual public UGameObject {
public:
	DUGameObject(std::string aName, Vector2D aPosition, std::string aType, std::string aTexture, Vector2D initSpeed = { 0, 0 }, SDL_RendererFlip aDirection = SDL_FLIP_NONE, Vector2D initVelocity = { 0, 0 });
	void update(double dt) override;

	Rectangle broadPhaseBox(double dt);
	bool DUGameObject::aabbCollisionCheck(GameObject object);
	double DUGameObject::sweepAABB(GameObject box, Vector2D& normal, double dt);
	bool broadPhaseCheck(Rectangle broadphaseBox, GameObject box);
	Vector2D getVelocity() {
		return velocity;
	}
	~DUGameObject() {}
protected:
	Vector2D velocity = { 0, 0 };
	Vector2D speed = { 0, 0 };
};