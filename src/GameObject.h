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
struct EitherOr {
	virtual void runClassSpecific() = 0;
};

class Rectangle {
public:
	Rectangle(Vector2D aPosition, Vector2D aSize) : position(aPosition), size(aSize) {}
	Vector2D getPVector();
	Vector2D position;
	Vector2D size;
};

class GameObject : public EitherOr {
public:
	GameObject(std::string aName, Vector2D aPosition, Vector2D aSize, bool isInteractable = false);
	~GameObject() {}
	virtual void runClassSpecific() {}
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
	Vector2D getOriginalPosition() {
		return originalPosition;
	}
	void setTilePosition(Vector2D newTilePosition) {
		if (this != nullptr) {
			tilePosition = newTilePosition;
		}
	}
	bool getOverlap() {
		return overlap;
	}
	std::string getName() {
		return name;
	}
	bool isInteractable() {
		return interactable;
	}
protected:
	std::string name;
	Vector2D position;
	Vector2D originalPosition;
	Vector2D tilePosition = { 0, 0 };
	Vector2D size;
	bool interactable;
	bool overlap = false;
};

class DGameObject : public Drawable, virtual public GameObject {
public:
	DGameObject(std::string aName, Vector2D aPosition, Vector2D aSize, std::string aTexture, bool isInteractable = false, SDL_RendererFlip aDirection = SDL_FLIP_NONE);
	~DGameObject() {}
	virtual void draw();
	virtual void runClassSpecific() {
		draw();
	}
protected:
	std::string texture = "";
	SDL_RendererFlip direction = SDL_FLIP_NONE;
};

class UGameObject : public Updateable, virtual public GameObject {
public:
	UGameObject(std::string aName, Vector2D aPosition, Vector2D aSize, bool isInteractable = false, std::string aType = "");
	~UGameObject() {}
	virtual void update(double dt);
	virtual void runClassSpecific() {}
protected:
	std::string type;
};

class DUGameObject : virtual public DGameObject, virtual public UGameObject {
public:
	DUGameObject(std::string aName, Vector2D aPosition, Vector2D aSize, std::string aTexture, bool isInteractable = false, std::string aType = "", Vector2D aSpeed = { 0, 0 }, SDL_RendererFlip aDirection = SDL_FLIP_NONE, Vector2D aVelocity = { 0, 0 });
	~DUGameObject() {}
	void update(double dt) override;
	void runClassSpecific() override {}
	bool staticAABBCheck(GameObject box);
	Rectangle getMinkowskiDifference(GameObject box);
	Rectangle createBroadPhaseBox(Vector2D newPosition);
	bool broadPhaseCheck(Rectangle broadphaseBox, GameObject box);
	Vector2D getVelocity() {
		return velocity;
	}
	void setVelocity(Vector2D newVelocity) {
		velocity = newVelocity;
	}
	bool canInteractWithStuff() {
		return canInteract;
	}
	bool jumping = false;
	bool onGround = false;
protected:
	bool canInteract = false;
	Vector2D velocity = { 0, 0 };
	Vector2D speed = { 0, 0 };
	bool colliding = false;
};