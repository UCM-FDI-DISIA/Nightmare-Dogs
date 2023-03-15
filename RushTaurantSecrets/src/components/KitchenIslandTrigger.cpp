#include "KitchenIslandTrigger.h"
#include "../structure/Scene.h"
#include "../utils/checkML.h"

KitchenIslandTrigger::KitchenIslandTrigger(GameObject* parent, Vector pos_, float width_, float height_, int i_, int orient_) :
	TriggerComp(parent, pos_, width_, height_), i(i_), orient(orient_), ki(parent->getScene()->getGameObject(_ecs::hdr_KITCHENISLAND)->getComponent<KitchenIslandComp>()) {}

void KitchenIslandTrigger::isOverlapping() {
	auto trans_ = other_->getComponent<Transform>();
	float aux = trans_->getPos().getX() + trans_->getW() / 2;
	if (trans_->getOrientation() != orient && aux > transform_->getPos().getX() && aux < transform_->getPos().getX() + width)
	{
		KitchenIslandComp::Player p;
		if (parent->getScene()->getGameObject(_ecs::hdr_PLAYER1) == other_) //si es player 1
			p = KitchenIslandComp::Player::p1;
		else p = KitchenIslandComp::Player::p2;

		ki->selectedIng(i, p);
		if (ih->joysticksInitialised()) {
			if (!ih->getButtonState(0, SDL_CONTROLLER_BUTTON_A)) return;
		}
		else if (!ih->isKeyDown(SDLK_SPACE)) return;
		ki->pickIngredient(i, p);
	}
	else if (trans_->getOrientation() == orient) {
		unselect();
	}
}
void KitchenIslandTrigger::onTriggerExit() {
	unselect();
}

void KitchenIslandTrigger::unselect() {
	if (parent->getScene()->getGameObject(_ecs::hdr_PLAYER1) == other_) //si es player 1
		ki->unselectIng(i, KitchenIslandComp::Player::p1);
	else ki->unselectIng(i, KitchenIslandComp::Player::p2);
}