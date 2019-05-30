#include "common.h"

GameObject::GameObject(std::string aName, Vector2D aPosition) : name(aName), position(aPosition) {
}

DGameObject::DGameObject(std::string aName, Vector2D aPosition, std::string aTexture, SDL_RendererFlip aDirection) : texture(aTexture), direction(aDirection), GameObject(aName, aPosition) {
	TextureManager::load(texture);
	if (this->getName() != "player") {
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

Rectangle DUGameObject::broadPhaseBox(double dt) {
	Rectangle broadphasebox;
	broadphasebox.position.x = velocity.x * dt > 0 ? position.x : position.x + velocity.x * dt;
	broadphasebox.position.y = velocity.y * dt > 0 ? position.y : position.y + velocity.y * dt;
	broadphasebox.size.x = velocity.x * dt > 0 ? velocity.x * dt + size.x : size.x - velocity.x * dt;
	broadphasebox.size.y = velocity.y * dt> 0 ? velocity.y * dt + size.y : size.y - velocity.y * dt;

	return broadphasebox;
}

bool DUGameObject::broadPhaseCheck(Rectangle bp, GameObject box) {
	if ((bp.position.x + bp.size.x > box.getPosition().x && bp.position.x < box.getPosition().x + box.getSize().x) && (bp.position.y + bp.size.y > box.getPosition().y && bp.position.y < box.getPosition().y + box.getSize().y)) {
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
struct CollisionInformation {
	double time;
	Vector2D normal;
	GameObject object;
	int id;
};

bool DUGameObject::aabbCollisionCheck(GameObject object) {
	if ((position.x + size.x > object.getPosition().x && position.x < object.getPosition().x + object.getSize().x) && (position.y + size.y > object.getPosition().y && position.y < object.getPosition().y + object.getSize().y)) {
		return true;
	}
	return false;
}

bool compareCollisionTime(CollisionInformation i1, CollisionInformation i2) {
	return (i1.time < i2.time);
}

void DUGameObject::update(double dt) {

	//GameObject object = *GameWorld::customObject[0];
	Rectangle bpBox = broadPhaseBox(dt);
	std::vector<CollisionInformation> collisions;
	for (auto& object : GameWorld::drawableObjects) {
		if (broadPhaseCheck(bpBox, *object)) {

			if (aabbCollisionCheck(*object)) {
				//resolve
				std::cout << "ALREADY COLLIDING!!! " << object->getName() << std::endl;

			} else {
				Vector2D normal;
				double collisionTime = sweepAABB(*object, normal, dt);
				//std::cout << "Collision time: " << collisionTime << std::endl;
				//position = position + velocity * dt * collisionTime;
				double remainingTime = 1.0 - collisionTime;
				if (collisionTime < 1) {
					std::cout << "WOULD COLLIDE!!!" << std::endl;
					collisions.push_back({ collisionTime, normal, *object, int(collisions.size()) });
					
					double dotprod = (velocity.x * normal.y + velocity.y * normal.x) * remainingTime;
					velocity.x = dotprod * normal.y;
					velocity.y = dotprod * normal.x;
					position = position + velocity * dt * remainingTime;
					
				}
			}
		}
	}
				/*
		else {
			position = position + velocity * dt;
		}
		*/
	//position = position + velocity * dt;

	if (collisions.size() > 0) {
		
		std::sort(collisions.begin(), collisions.end(), compareCollisionTime);

		//std::cout << "Collision occured: " << collisions.size() << std::endl;
		position = position + velocity * dt * collisions[0].time;

		//timeXCollision = (player.left - wall.right) / -player.velocity.x
		//timeYCollision = (wall.bottom - player.top) / player.velocity.y

		//double remainingTime = 1.0 - collisions[0].time;

		//double dotprod = (velocity.x * collisions[0].normal.y + velocity.y * collisions[0].normal.x) * remainingTime;
		//velocity.x = dotprod * collisions[0].normal.y;
		//velocity.y = dotprod * collisions[0].normal.x;
		//THE SLIDING IS WHAT CAUSES THE TUNNELING THROUGH CORNERS!!!

		//position = position + velocity * dt * remainingTime;
		//Rectangle remainingBroadPhaseRectangle = broadPhaseBox(dt);
		
		collisions.erase(collisions.begin());
		for (auto& information : collisions) {
			if (aabbCollisionCheck(information.object)) {
				Vector2D collisionDepth = { 0, 0 };
				if (information.normal.x == 1) {
					collisionDepth = { information.object.getPosition().x + information.object.getSize().x - position.x, 0 };
				} else if (information.normal.x == -1) {
					collisionDepth = { information.object.getPosition().x - (position.x + size.x), 0 };
				} else if (information.normal.y == 1) {
					collisionDepth = { 0, information.object.getPosition().y + information.object.getSize().y - position.y };
				} else if (information.normal.y == -1) {
					collisionDepth = { 0, information.object.getPosition().y - (position.y + size.y) };
				}
				if (collisionDepth.x != 0 || collisionDepth.y != 0) {
					position = position + collisionDepth;
				}
			}
		}
		
	} else {
		position = position + velocity * dt;
	}
}