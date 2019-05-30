#include "common.h"

GameObject::GameObject(std::string aName, Vector2D aPosition) : name(aName), position(aPosition), originalPosition(aPosition) {
}

DGameObject::DGameObject(std::string aName, Vector2D aPosition, std::string aTexture, SDL_RendererFlip aDirection) : texture(aTexture), direction(aDirection), GameObject(aName, aPosition) {
	TextureManager::load(texture);
	if (aName != "player") {
		GameWorld::drawableObjects.push_back(this);
	}
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

bool DUGameObject::staticAABBCheck(GameObject box) {
	if ((position.x + size.x > box.getPosition().x && position.x < box.getPosition().x + box.getSize().x) && (position.y + size.y > box.getPosition().y && position.y < box.getPosition().y + box.getSize().y)) {
		return true;
	}
	return false;
}

bool DUGameObject::broadPhaseCheck(Rectangle bp, GameObject box) {
	if ((box.getPosition().x + box.getSize().x > bp.position.x && box.getPosition().x < bp.position.x + bp.size.x) && (box.getPosition().y + box.getSize().y > bp.position.y && box.getPosition().y < bp.position.y + bp.size.y)) {
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
			} else if (xEntryDistance > 0) {
				normal = { -1, 0 };
			} else {
				normal = { 0, 0 };
			}
		} else if (xEntry < yEntry) {
			if (yEntryDistance < 0) {
				normal = { 0, 1 };
			} else if (yEntryDistance > 0) {
				normal = { 0, -1 };
			} else {
				normal = { 0, 0 };
			}
		} else {
			normal = { 0, 0 };
		}
		return entryTime;
	}

}

struct CollisionInformation {
	double time;
	GameObject object;
	Vector2D normal;
};

bool compareCollisionInformation(CollisionInformation i1, CollisionInformation i2) {
	return (i1.time < i2.time);
}

void DUGameObject::update(double dt) {

	bool collision = false;
	Vector2D newPosition = position + velocity * dt;

	Rectangle broadPhaseRectangle = *broadPhaseBox(newPosition);
	std::vector<CollisionInformation> times;
	for (auto& object : GameWorld::customObject) {

		if (staticAABBCheck(*object)) {

		}

		if (broadPhaseCheck(broadPhaseRectangle, *object)) {

			Vector2D normal;
			double collisionTime = sweepAABB(*object, normal, dt);
			
			times.push_back({ collisionTime, *object, normal });
			double remainingTime = 1.0 - collisionTime;

			if (collisionTime < 1) {

				//double dotprod = (velocity.x * normal.y + velocity.y * normal.x) * remainingTime;
				//velocity.x = dotprod * normal.y;
				//velocity.y = dotprod * normal.x;
				//position = position + velocity * dt * remainingTime;
				collision = true;
				
			}
		}

	}
	
	if (!collision) {
		position = position + velocity * dt;
	} else {
		CollisionInformation info = *std::min_element(times.begin(), times.end(), compareCollisionInformation);
		double collisionTime = info.time;
		Vector2D normal = info.normal;
		position = position + velocity * dt * collisionTime;
		double remainingTime = 1.0 - collisionTime;
		if (collisionTime < 1) {
			std::cout << "normal : " << normal.toString() << std::endl;
			double dotprod = (velocity.x * normal.y + velocity.y * normal.x) * remainingTime;
			velocity.x = dotprod * normal.y;
			velocity.y = dotprod * normal.x;
			position = position + velocity * dt * remainingTime;

		}
	}
}