#include "SuperCashRegisterTriggerComp.h"

#include "../Scenes/HUD/UIRestaurant.h"
#include "../Structure/GameManager.h"
#include "../Scenes/GameScenes/Restaurant.h"
#include "../Components/ShoppingMenuComp.h"
#include "../Scenes/GameScenes/SuperMarket.h"
#include "../Structure/Game.h"

#include "../Utilities/checkML.h"

SuperCashRegisterTriggerComp::SuperCashRegisterTriggerComp(GameObject* parent, Vector pos_, float width_, float height_) :
	TriggerComp(parent, pos_, width_, height_), sdl(SDLUtils::instance()), restaurantMusic(&sdl->musics().at("RESTAURANT_MUSIC"))
{
	highlight = parent->getComponent<Image>();
	highlight->setActive(false);
	money = GameManager::get()->getMoney();
	restaurantMusic->setMusicVolume(MUSIC_VOL);

};

void SuperCashRegisterTriggerComp::isOverlapping() {
	highlight->setActive(true);

	if (bM == nullptr) {
		auto scene = dynamic_cast<SuperMarket*>(parent->getScene());
		bM = scene->getBM();
		bMC = bM->getComponent<BasketMarketComponent>();
		if (!bMC->getBasketON())
			bMC->setBasketON(true);
		buybutton = scene->buyButton();
		buybutton->setActives(true);
	}

	// cleon: mejor en 1 if
	if (ih->joysticksInitialised()) {
		if (ih->getButtonState(0, SDL_CONTROLLER_BUTTON_A)) {
			if (buybutton->getComponent<ButtonComp>()->isHighlighted()) {
				sendToClien();
				money->subtractMoney(money->getPrize());
				money->setPrize(0);
				payAndLeave();
			}
		}
	}
 	else if (ih->isKeyDown(SDLK_SPACE)) {
		if (buybutton->getComponent<ButtonComp>()->isHighlighted()) {
			sendToClien();
			money->subtractMoney(money->getPrize());
			money->setPrize(0);
			payAndLeave();
		}		
	}
}

void SuperCashRegisterTriggerComp::onTriggerExit() {
	highlight->setActive(false);
	bMC->setBasketON(false);
	bM = nullptr;
	bMC = nullptr;

	buybutton->getComponent<ButtonComp>()->setHighlighted(false);
	buybutton->setActives(false);
	buybutton = nullptr;
}

void SuperCashRegisterTriggerComp::payAndLeave() {
	money->subtractMoney(money->getPrize());
	money->setPrize(0);
	vector<pair<_ecs::_ingredients_id, int>> ing;
	ing = GameManager::get()->getSupermarket()->getUI()->getGameObject(hdr_SM_INVENTORY)->getComponent<BasketMarketComponent>()->getIngredients();
	GameManager::get()->setIngredients(ing);
	GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_RESTAURANT));
	GameManager::get()->getScene(_ecs::sc_RESTAURANT)->reset();
	restaurantMusic->play();
}

void SuperCashRegisterTriggerComp::sendToClien() {
	Message m;
	m.id = Message::msg_TO_RESTAURANT;
	Game::get()->getCoopHandler()->send(m);
}

void SuperCashRegisterTriggerComp::receive(const Message& message) { 
	if (message.id == Message::msg_TO_RESTAURANT) {
		money->subtractMoney(money->getPrize());
		money->setPrize(0);
		payAndLeave();
	}
}