#include "PantryExitTrigger.h"

#include "../structure/GameManager.h"
#include "../structure/Scene.h"
#include "../structure/GameObject.h"
#include "../components/Transform.h"

PantryExitTrigger::PantryExitTrigger(GameObject* parent, Vector pos_, float width_, float height_) : TriggerComp(parent, pos_, width_, height_), parent(parent) {
	ih = InputHandler::instance();
};


void PantryExitTrigger::onTriggerEnter() {
	GameManager::instance()->changeScene((Scene*)GameManager::instance()->getRestaurant());
	GameManager::instance()->getCurrentScene()->getGameObject(_ecs::hdr_PLAYER1)->getComponent<Transform>()->setPos(Vector(640, 130));
}
