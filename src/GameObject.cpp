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

bool DUGameObject::collides(GameObject& obj) {
	if ( (position.x < obj.getPosition().x + obj.getSize().x && position.x + size.x < obj.getPosition().x) || 
		(position.x > obj.getPosition().x + obj.getSize().x && position.x + size.x > obj.getPosition().x) || 
		(position.y + size.y < obj.getPosition().y && position.y < obj.getPosition().y + obj.getSize().y) || 
		(position.y > obj.getPosition().y + obj.getSize().y && position.y + size.y > obj.getPosition().y) ) { 
		return false;
	} else {
		return true;
	}
}

Vector2D DUGameObject::lineIntersectAABB(GameObject* object, std::vector<Vector2D> cornerPoints, std::vector<Vector2D> newCornerPoints) {
	Vector2D collisionPoint = { 0, 0 };
	int tempPoints = 0;
	int indexOfCorner = 0;
	double collisionPointDistance = std::numeric_limits<double>::infinity();
	for (int i = 0; i < newCornerPoints.size(); i++) {
		Vector2D newPosition = newCornerPoints[i];
		Vector2D position = cornerPoints[i];
		double slope = Vector2D::slope(position, newPosition);
		double y = 0.0, x = 0.0;
		std::vector<Vector2D> collidingPoints;

		y = slope * (object->getPosition().x - position.x) + position.y;
		if (y > object->getPosition().y && y < object->getPosition().y + object->getSize().y) {
			collidingPoints.push_back({ object->getPosition().x, y });
		}
		x = (object->getPosition().y - position.y) / slope + position.x;
		if (x > object->getPosition().x && x < object->getPosition().x + object->getSize().x) {
			collidingPoints.push_back({ x, object->getPosition().y });
		}
		y = slope * (object->getPosition().x + object->getSize().x - position.x) + position.y;
		if (y > object->getPosition().y && y < object->getPosition().y + object->getSize().y) {
			collidingPoints.push_back({ object->getPosition().x + object->getSize().x, y });
		}
		x = (object->getPosition().y + object->getSize().y - position.y) / slope + position.x;
		if (x > object->getPosition().x && x < object->getPosition().x + object->getSize().x) {
			collidingPoints.push_back({ x, object->getPosition().y + object->getSize().y });
		}
		if (collidingPoints.size() > 0) {
			tempPoints++;
			double minimumDistance = std::numeric_limits<double>::infinity();
			Vector2D closestPoint = { 0, 0 };
			for (auto& point : collidingPoints) {
				double distance = position.distance(point);
				if (distance < minimumDistance) {
					minimumDistance = distance;
					closestPoint = point;
				}
			}
			double distance = closestPoint.distance(position);
			if (distance < collisionPointDistance) {
				collisionPointDistance = distance;
				collisionPoint = closestPoint;
				indexOfCorner = i;
			}
		}
	}
	if (tempPoints == 0) {
		//no collision occured with any of the lines
		collisionPoint = newCornerPoints[0];
	} else {
		switch (indexOfCorner) {
			case 0: {
				collisionPoint = collisionPoint;
				//std::cout << "Short point was top left" << std::endl;
				break;
			}
			case 1: {
				collisionPoint = { collisionPoint.x - size.x, collisionPoint.y };
				//std::cout << "Short point was top right" << std::endl;
				break;
			}
			case 2: {
				collisionPoint = collisionPoint - size;
				//std::cout << "Short point was bottom right" << std::endl;
				break;
			}
			case 3: {
				collisionPoint = { collisionPoint.x, collisionPoint.y - size.y };
				//std::cout << "Short point was bottom left" << std::endl;
				break;
			}
		}
	}
	return collisionPoint;
}
Box* DUGameObject::ptr;
Vector2D* DUGameObject::point;
void DUGameObject::update(double dt) {

	//add buffer to check lowest distance of ALL objects that player collided with in the frame
	//give sideways velocity (push) if player velocity * dt isnt equivalent to the distance to newPosition

	bool collision = false;
	std::vector<Vector2D> collidingPoints;
	std::vector<Vector2D> cornerPoints;
	std::vector<Vector2D> newCornerPoints;

	cornerPoints.push_back(position);
	cornerPoints.push_back({ position.x + size.x, position.y });
	cornerPoints.push_back(position + size);
	cornerPoints.push_back({ position.x, position.y + size.y });

	for (auto& point : cornerPoints) {
		newCornerPoints.push_back(point + velocity * dt);
	}

	//GameObject* object = GameWorld::customObject[0];
	ptr = new Box({ 0, 0 }, { abs(newCornerPoints[0].x + size.x - position.x), abs(newCornerPoints[0].y + size.y - position.y) });

	//draw square
	if (velocity.x < 0) {
		ptr->position.x = newCornerPoints[0].x;
	} else {
		ptr->position.x = position.x;
	}
	if (velocity.y < 0) {
		ptr->position.y = newCornerPoints[0].y;
	} else {
		ptr->position.y = position.y;
	}
	/*
	//atm, a bug exists where if player is exactly lined up on y axis with object, then it can go through (no collision is detected because < and > but not =)
	std::vector<double> collisionXBuffer;
	std::vector<double> collisionYBuffer;
	collisionXBuffer.clear();
	collisionYBuffer.clear();
	for (auto& object : GameWorld::drawableObjects) {
		//check if object is inside square
		if (((object->getPosition().x < ptr->position.x + ptr->size.x && object->getPosition().x + object->getSize().x > ptr->position.x) || (object->getPosition().x + object->getSize().x > ptr->position.x && object->getPosition().x < ptr->position.x + ptr->size.x) || (object->getPosition().x == ptr->position.x && object->getPosition().x + object->getSize().x == ptr->position.x + ptr->size.x)) && ((object->getPosition().y < ptr->position.y + ptr->size.y && object->getPosition().y + object->getSize().y > ptr->position.y) || (object->getPosition().y + object->getSize().y > ptr->position.y && object->getPosition().y < ptr->position.y + ptr->size.y))) {
			//find best point of collision
			Vector2D pointOfCollision = lineIntersectAABB(object, cornerPoints, newCornerPoints);
			if (pointOfCollision.x == newCornerPoints[0].x && pointOfCollision.y == newCornerPoints[0].y) {
				//player didnt move
				//position = newCornerPoints[0];
			} else {
				//player would collide, therefore offset position by that amount
				std::cout << "Collision with: " << object->getName() << ", Position: " << position.toString() << ", Dirt Position: " << object->getPosition().toString() << std::endl;
				if ((position.y > object->getPosition().y && position.y < object->getPosition().y + object->getSize().y) || (position.y + size.y > object->getPosition().y && position.y + size.y < object->getPosition().y + object->getSize().y)) {
					//std::cout << "horizontal collision" << std::endl;
					collisionXBuffer.push_back(pointOfCollision.x);
					//position.x = pointOfCollision.x;
					//position.y = position.y + (newCornerPoints[0].y - pointOfCollision.y);
				}
				if ((position.x > object->getPosition().x && position.x < object->getPosition().x + object->getSize().x) || (position.x + size.x > object->getPosition().x && position.x + size.x < object->getPosition().x + object->getSize().x)) {
					//std::cout << "vertical collision" << std::endl;
					collisionYBuffer.push_back(pointOfCollision.y);
					//position.y = pointOfCollision.y;
					//position.x = position.x + (newCornerPoints[0].x - pointOfCollision.x);
				}

				collision = true;
			}
		}
	}
	double minimumX = 555555555555;
	double finalX = 0;
	for (auto pointX : collisionXBuffer) {
		if (abs(pointX - position.x) < minimumX) {
			minimumX = abs(pointX - position.x);
			finalX = pointX;
		}
	}
	double minimumY = 555555555555;
	double finalY = 0;
	for (auto pointY : collisionYBuffer) {
		if (abs(pointY - position.y) < minimumY) {
			minimumY = abs(pointY - position.y);
			finalY = pointY;
		}
	}
	Vector2D finalPos = { finalX, finalY };
	if (finalX == 0) {
		finalPos.x = position.x + velocity.x * dt;
	}
	if (finalY == 0) {
		finalPos.y = position.y + velocity.y * dt;
	}
	std::cout << "Final Coordinates : " << finalPos.x << "," << finalPos.y << std::endl;
	position = finalPos;

	*/

	std::vector<double> collisionXBuffer;
	std::vector<GameObject*> cornerBuffer;
	for (auto& object : GameWorld::drawableObjects) {
		//check if object is inside square
		if (((object->getPosition().x < ptr->position.x + ptr->size.x && object->getPosition().x + object->getSize().x > ptr->position.x) || (object->getPosition().x + object->getSize().x > ptr->position.x && object->getPosition().x < ptr->position.x + ptr->size.x) || (object->getPosition().x == ptr->position.x && object->getPosition().x + object->getSize().x == ptr->position.x + ptr->size.x)) && ((object->getPosition().y < ptr->position.y + ptr->size.y && object->getPosition().y + object->getSize().y > ptr->position.y) || (object->getPosition().y + object->getSize().y > ptr->position.y && object->getPosition().y < ptr->position.y + ptr->size.y))) {
			//find best point of collision
			Vector2D pointOfCollision = lineIntersectAABB(object, cornerPoints, newCornerPoints);
			
			if (pointOfCollision.x == newCornerPoints[0].x) {
				//player didnt move
				//cornerBuffer.push_back(object);
				//position = newCornerPoints[0];
			} else {
				//std::cout << object->getName() << " inside movement square" << std::endl;
				//player would collide, therefore offset position by that amount
				if ((position.y > object->getPosition().y && position.y < object->getPosition().y + object->getSize().y) || (position.y + size.y > object->getPosition().y && position.y + size.y < object->getPosition().y + object->getSize().y)) {
					//std::cout << "horizontal collision" << std::endl;
					collisionXBuffer.push_back(pointOfCollision.x);
					//position.x = pointOfCollision.x;
					//position.y = position.y + (newCornerPoints[0].y - pointOfCollision.y);
				}

			}
		}
	}
	double minimumX = 555555555555;
	double finalX = 0;
	for (auto pointX : collisionXBuffer) {
		if (abs(pointX - position.x) < minimumX) {
			minimumX = abs(pointX - position.x);
			finalX = pointX;
		}
	}
	if (finalX == 0) {
		finalX = position.x + velocity.x * dt;
	} else {
		//std::cout << "Resolved X collision" << std::endl;
	}
	if (cornerBuffer.size() < 1) {
		position.x = finalX;
		//std::cout << "moving X" << std::endl;
	}



	std::vector<double> collisionYBuffer;
	for (auto& object : GameWorld::drawableObjects) {
		//check if object is inside square
		if (((object->getPosition().x < ptr->position.x + ptr->size.x && object->getPosition().x + object->getSize().x > ptr->position.x) || (object->getPosition().x + object->getSize().x > ptr->position.x && object->getPosition().x < ptr->position.x + ptr->size.x) || (object->getPosition().x == ptr->position.x && object->getPosition().x + object->getSize().x == ptr->position.x + ptr->size.x)) && ((object->getPosition().y < ptr->position.y + ptr->size.y && object->getPosition().y + object->getSize().y > ptr->position.y) || (object->getPosition().y + object->getSize().y > ptr->position.y && object->getPosition().y < ptr->position.y + ptr->size.y))) {
			//find best point of collision
			Vector2D pointOfCollision = lineIntersectAABB(object, cornerPoints, newCornerPoints);
			if (pointOfCollision.y == newCornerPoints[0].y) {
				//player didnt move
				//position = newCornerPoints[0];
				//cornerBuffer.push_back(object);
			} else {
				//player would collide, therefore offset position by that amount

				if ((position.x > object->getPosition().x && position.x < object->getPosition().x + object->getSize().x) || (position.x + size.x > object->getPosition().x && position.x + size.x < object->getPosition().x + object->getSize().x)) {
					//std::cout << "vertical collision" << std::endl;
					collisionYBuffer.push_back(pointOfCollision.y);
					//position.y = pointOfCollision.y;
					//position.x = position.x + (newCornerPoints[0].x - pointOfCollision.x);
				}

			}
		}
	}

	double minimumY = 555555555555;
	double finalY = 0;
	for (auto pointY : collisionYBuffer) {
		if (abs(pointY - position.y) < minimumY) {
			minimumY = abs(pointY - position.y);
			finalY = pointY;
		}
	}
	if (finalY == 0 && cornerBuffer.size() < 2) {
		finalY = position.y + velocity.y * dt;
	} else {
		//std::cout << "Resolved Y collision" << std::endl;
	}
	if (cornerBuffer.size() < 2) {
		position.y = finalY;
		//std::cout << "moving Y" << std::endl;
	}
	std::cout << "Corner buffer: " << cornerBuffer.size() << std::endl;
	/*
	if (cornerBuffer.size() == 2) {
		Vector2D point1 = cornerBuffer[0];
		position = cornerBuffer[0] - velocity * dt;
			//position  + velocity * dt - +(position - cornerBuffer[0]);
		DUGameObject::point = &point1;
		std::cout << "Corner Buffer Size: " << cornerBuffer.size() << std::endl;
	} else if (cornerBuffer.size() == 3) {
		
	} else {
		Vector2D point1 = { 200, 200 };
		DUGameObject::point = &point1;
	}
	*/
}