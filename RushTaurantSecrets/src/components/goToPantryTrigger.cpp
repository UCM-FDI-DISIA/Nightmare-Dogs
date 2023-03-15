#include "goToPantryTrigger.h"

#include "../sdlutils/InputHandler.h"

#include "../structure/GameManager.h"
#include "../structure/Scene.h"
#include "../structure/GameObject.h"
#include "../components/Transform.h"


#include "../utils/checkML.h"

goToPantryTrigger::goToPantryTrigger(GameObject* parent, Vector pos_, float width_, float height_) : TriggerComp(parent, pos_, width_, height_) { };

void goToPantryTrigger::onTriggerEnter() {
	GameManager::instance()->changeScene((Scene*)GameManager::instance()->getPantry());
	GameManager::instance()->getCurrentScene()->getGameObject(_ecs::hdr_PLAYER1)->getComponent<Transform>()->setPos(Vector(800, 550));
}
