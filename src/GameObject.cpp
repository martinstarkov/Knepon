#include "common.h"

GameObject::GameObject(std::string aName, Vector2D aPosition, Vector2D aSize, bool isInteractable) : name(aName), position(aPosition), originalPosition(aPosition), size(aSize), interactable(isInteractable) {
}

DGameObject::DGameObject(std::string aName, Vector2D aPosition, std::string aTexture, Vector2D aSize, bool isInteractable, SDL_RendererFlip aDirection) : texture(aTexture), direction(aDirection), GameObject(aName, aPosition, aSize, isInteractable) {
	TextureManager::load(texture);
	if (aName != "player") {
		GameWorld::drawableObjects.push_back(this);
	}
}

UGameObject::UGameObject(std::string aName, Vector2D aPosition, Vector2D aSize, bool isInteractable, std::string aType) : type(aType), GameObject(aName, aPosition, aSize, isInteractable) {
	if (aName != "player") {
		GameWorld::updateableObjects.push_back(this);
	}
}

DUGameObject::DUGameObject(std::string aName, Vector2D aPosition, std::string aTexture, Vector2D aSize, bool isInteractable, std::string aType, Vector2D aSpeed, SDL_RendererFlip aDirection, Vector2D aVelocity) : speed(aSpeed), velocity(aVelocity), UGameObject(aName, aPosition, aSize, isInteractable, aType), DGameObject(aName, aPosition, aTexture, aSize, isInteractable, aDirection), GameObject(aName, aPosition, aSize, isInteractable) {
}

void DGameObject::draw() {
	TextureManager::draw(texture, { ((int)floor(position.x) - Game::getInstance()->camera.x), ((int)floor(position.y) - Game::getInstance()->camera.y), 16, 16 }, direction);
}

void UGameObject::update(double dt) {
}

Rectangle DUGameObject::createBroadPhaseBox(Vector2D newPosition) {
	Vector2D rectanglePosition;
	rectanglePosition.x = velocity.x > 0 ? position.x : newPosition.x;
	rectanglePosition.y = velocity.y > 0 ? position.y : newPosition.y;
	return *new Rectangle(rectanglePosition, (position - newPosition).abs() + size);
}

bool DUGameObject::staticAABBCheck(GameObject box) {
	if ((position.x + size.x > box.getPosition().x && position.x < box.getPosition().x + box.getSize().x) && (position.y + size.y > box.getPosition().y && position.y < box.getPosition().y + box.getSize().y)) {
		return true;
	}
	return false;
}

Rectangle DUGameObject::getMinkowskiDifference(GameObject box) {
	Vector2D mPosition, mSize;
	mPosition.x = position.x - (box.getPosition().x + box.getSize().x);
	mPosition.y = position.y - (box.getPosition().y + box.getSize().y);
	mSize.x = size.x + box.getSize().x;
	mSize.y = size.y + box.getSize().y;
	return *new Rectangle(mPosition, mSize);
}

Vector2D Rectangle::getPVector() {
	double minDist = abs(position.x);
	Vector2D boundsPoint = { position.x, 0 };
	if (abs(position.x + size.x) < minDist) {
		minDist = abs(position.x + size.x);
		boundsPoint = { position.x + size.x, 0 };
	}
	if (abs(position.y + size.y) < minDist) {
		minDist = abs(position.y + size.y);
		boundsPoint = { 0, position.y + size.y };
	}
	if (abs(position.y) < minDist) {
		minDist = abs(position.y);
		boundsPoint = { 0, position.y };
	}
	return boundsPoint;
}

bool DUGameObject::broadPhaseCheck(Rectangle bp, GameObject box) {
	if ((box.getPosition().x + box.getSize().x > bp.position.x && box.getPosition().x < bp.position.x + bp.size.x) && (box.getPosition().y + box.getSize().y > bp.position.y && box.getPosition().y < bp.position.y + bp.size.y)) {
		return true;
	}
	return false;
}

void DUGameObject::update(double dt) {

	Vector2D newPosition = position + velocity;


	Rectangle bpb = createBroadPhaseBox(newPosition);

	std::vector<GameObject*> potentialColliders;
	std::vector<GameObject*> potentialInteractables;

	for (auto& box : GameWorld::getCurrentLevel()->collideableLevelObjects) {
		//for (auto& box : row) {
			if (box != nullptr) {
				if (broadPhaseCheck(bpb, *box)) {
					if (box->isInteractable()) {
						potentialInteractables.push_back(box);
					} else {
						potentialColliders.push_back(box);
					}
				}
			}
		//}
	}

	position.x = position.x + velocity.x;

	for(auto& box : potentialColliders) {

		Rectangle rect = getMinkowskiDifference(*box);

		if (rect.position.x <= 0 && rect.position.x + rect.size.x >= 0 && rect.position.y <= 0 && rect.position.y + rect.size.y >= 0) {

			Vector2D penetrationVector = rect.getPVector();

			position.x = position.x - penetrationVector.x;

		}

	}

	position.y = position.y + velocity.y;

	std::vector<Vector2D> yCollisions;

	for (auto& box : potentialColliders) {

		Rectangle rect = getMinkowskiDifference(*box);

		if (rect.position.x <= 0 && rect.position.x + rect.size.x >= 0 && rect.position.y <= 0 && rect.position.y + rect.size.y >= 0) {

			Vector2D penetrationVector = rect.getPVector();

			yCollisions.push_back(penetrationVector);

			position.y = position.y - penetrationVector.y;
		}

	}

	onGround = false;
	jumping = true;

	if (velocity.y != 0) {
		if (yCollisions.size() > 0) {
			for (auto& pV : yCollisions) {
				if (pV.y > 0) {
					//std::cout << "Hit the ground!" << std::endl;
					onGround = true;
					jumping = false;
					velocity.y = 0;
					break;
				}
				if (pV.y < 0) {
					velocity.y = 0;
				}
			}
		}
	}

	canInteract = false;

	for (auto& box : potentialInteractables) {

		Rectangle rect = getMinkowskiDifference(*box);

		if (rect.position.x <= 0 && rect.position.x + rect.size.x >= 0 && rect.position.y <= 0 && rect.position.y + rect.size.y >= 0) {
			
			canInteract = true;

		}

	}

	tilePosition = findTilePosition();

}