#include "common.h"

GameObject::GameObject(std::string aName, Vector2D aPosition) : name(aName), position(aPosition) {
}

DGameObject::DGameObject(std::string aName, Vector2D aPosition, std::string aTexture, SDL_RendererFlip aDirection) : texture(aTexture), direction(aDirection), GameObject(aName, aPosition) {
	TextureManager::load(texture);
	GameWorld::drawableObjects.push_back(this);
}

UGameObject::UGameObject(std::string aName, Vector2D aPosition, std::string aType) : type(aType), GameObject(aName, aPosition) {
	GameWorld::updateableObjects.push_back(this);
}

DUGameObject::DUGameObject(std::string aName, Vector2D aPosition, std::string aType, std::string aTexture, Vector2D objSpeed, SDL_RendererFlip aDirection, Vector2D initVelocity) : speed(objSpeed), velocity(initVelocity), UGameObject(aName, aPosition, aType), DGameObject(aName, aPosition, aTexture, aDirection), GameObject(aName, aPosition) {
}

void DGameObject::draw() {
	TextureManager::draw(texture, { (int)floor(position.x), (int)floor(position.y), 16, 16 }, direction);
}

void UGameObject::update(double dt) {
}

Rectangle* DUGameObject::broadPhaseBox(Vector2D newPosition) {
	Vector2D rectanglePosition;
	if (velocity.x > 0) {
		rectanglePosition.x = position.x;
	} else {
		rectanglePosition.x = newPosition.x;
	}
	if (velocity.y > 0) {
		rectanglePosition.y = position.y;
	} else {
		rectanglePosition.y = newPosition.y;
	}
	return new Rectangle(rectanglePosition, +(position - newPosition) + size);
}

bool DUGameObject::broadPhaseCheck(Rectangle bPB, GameObject box) {
	if (((box.getPosition().x < bPB.position.x + bPB.size.x && box.getPosition().x > bPB.position.x) || (box.getPosition().x + box.getSize().x > bPB.position.x && box.getPosition().x + box.getSize().x < bPB.position.x + bPB.size.x)) && ((box.getPosition().y < bPB.position.y + bPB.size.y && box.getPosition().y > bPB.position.y) || (box.getPosition().y + box.getSize().y > bPB.position.y && box.getPosition().y + box.getSize().y < bPB.position.y + bPB.size.y))) {
		return true;
	}
	return false;
}

double DUGameObject::sweepAABB(GameObject box, Vector2D& normal, double dt) {

	double xEntryDistance, xExitDistance, yEntryDistance, yExitDistance;

	if (velocity.x > 0) {
		xEntryDistance = box.getPosition().x - (position.x + size.x);
		xExitDistance = (box.getPosition().x + box.getSize().x) - position.x;
	} else {
		xEntryDistance = (box.getPosition().x + box.getSize().x) - position.x;
		xExitDistance = box.getPosition().x - (position.x + size.x);
	}
	if (velocity.y > 0) {
		yEntryDistance = box.getPosition().y - (position.y + size.y);
		yExitDistance = (box.getPosition().y + box.getSize().y) - position.y;
	} else {
		yEntryDistance = (box.getPosition().y + box.getSize().y) - position.y;
		yExitDistance = box.getPosition().y - (position.y + size.y);
	}

	double xEntry, xExit, yEntry, yExit;

	if (velocity.x == 0) {
		xEntry = -std::numeric_limits<double>::infinity();
		xExit = std::numeric_limits<double>::infinity();
	} else {
		xEntry = xEntryDistance / (velocity.x * dt);
		xExit = xExitDistance / (velocity.x * dt);
	}

	if (velocity.y == 0) {
		yEntry = -std::numeric_limits<double>::infinity();
		yExit = std::numeric_limits<double>::infinity();
	} else {
		yEntry = yEntryDistance / (velocity.y * dt);
		yExit = yExitDistance / (velocity.y * dt);
	}
	
	// find the earliest/latest times of collision
	double entryTime = std::max(xEntry, yEntry);
	double exitTime = std::min(xExit, yExit);

	//no collision
	if (entryTime > exitTime || xEntry < 0 && yEntry < 0 || xEntry > 1 || yEntry > 1) {
		normal = { 0, 0 };
		return 1;
	} else {
		if (xEntry > yEntry) {
			if (xEntryDistance < 0) {
				normal = { 1, 0 };
			} else {
				normal = { -1, 0 };
			}
		} else {
			if (xEntryDistance < 0) {
				normal = { 0, 1 };
			} else {
				normal = { 0, -1 };
			}
		}
		return entryTime;
	}

}

void DUGameObject::update(double dt) {

	Vector2D newPosition = position + velocity * dt;

	Rectangle broadPhaseRectangle = *broadPhaseBox(newPosition);
	GameObject object = *GameWorld::customObject[0];

	if (broadPhaseCheck(broadPhaseRectangle, object)) {

		Vector2D normal;
		double collisionTime = sweepAABB(object, normal, dt);
		
		position = position + velocity * dt * collisionTime;

		double remainingTime = 1.0 - collisionTime;

		if (collisionTime < 1) {
			double dotprod = (velocity.x * normal.y + velocity.y * normal.x) * remainingTime;
			velocity.x = dotprod * normal.y;
			velocity.y = dotprod * normal.x;
			position = position + velocity * dt * remainingTime;
		}

	} else {
		position = position + velocity * dt;
	}
}