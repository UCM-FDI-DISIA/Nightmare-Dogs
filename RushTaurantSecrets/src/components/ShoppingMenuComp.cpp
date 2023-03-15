#include "ShoppingMenuComp.h"
#include "../structure/Scene.h"
#include "../scenes/UIMarket.h"
#include "../structure/GameManager.h"
#include "../scenes/SuperMarket.h"

ShoppingMenuComp::ShoppingMenuComp(GameObject* parent) :Component(parent, id),
sdl(SDLUtils::instance()), ih(InputHandler::instance()),
priceTex(nullptr), numberTex(nullptr), totalPriceTex(nullptr), ingTex(nullptr), ing(_ecs::NONE_ING) {

	menu = &((*sdl).images().at("SHOP_MENU"));
	font = new Font("assets/Fonts/8-bit Madness.ttf", 40);
	parent->setActive<ShoppingMenuComp>(false);

}

void ShoppingMenuComp::initComponent() {
	basket = parent->getScene()->getGameObject(hdr_SM_INVENTORY)->getComponent<BasketMarketComponent>();
	playerTransform = GameManager::instance()->getSupermarket()->getGameObject(hdr_PLAYER1)->getComponent<Transform>();
}

ShoppingMenuComp::~ShoppingMenuComp() {
	delete font;
	if (numberTex != nullptr)
		delete numberTex;
	if (priceTex != nullptr)
		delete priceTex;
	if (totalPriceTex != nullptr)
		delete totalPriceTex;
}
void ShoppingMenuComp::handleEvents() {

	if (ih->joysticksInitialised()) {
		if(ih->getButtonState(0, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) decreaseN();
		else if (ih->getButtonState(0, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) decreaseN();
		else if (ih->getButtonState(0, SDL_CONTROLLER_BUTTON_B && ing != NONE_ING)) {
			basket->addToBasket(ing, number, totalPrice);
			closeMenu();
		}
	}
	else if (ih->isKeyDown(SDLK_LEFT)) decreaseN(); //aumentar cantidad
	else if (ih->isKeyDown(SDLK_RIGHT)) increaseN(); //disminuir cantidad
	else if (ih->isKeyDown(SDLK_RETURN) && ing != NONE_ING) { //añadir a la cesta
		basket->addToBasket(ing, number, totalPrice);
		closeMenu();
	}

}
void ShoppingMenuComp::render() {

	Vector menuPos = { playerTransform->getPos().getX() + playerTransform->getW() / 2 - MENU_WIDTH / 2,
		playerTransform->getPos().getY() + PLAYER_OFFSETY - MENU_HEIGHT };
	menu->render(build_sdlrect(menuPos.getX(), menuPos.getY(), MENU_WIDTH, MENU_HEIGHT));
	string aux = to_string(totalPrice);
	numberTex->render(build_sdlrect(menuPos.getX() + NUM_OFFSETX, menuPos.getY() + NUM_OFFSETY, NUM_WIDTH, NUM_HEIGHT));
	priceTex->render(build_sdlrect(menuPos.getX() + PRICE_OFFSETX, menuPos.getY() + PRICE_OFFSETY, PRICE_WIDTH, PRICE_HEIGHT));
	totalPriceTex->render(build_sdlrect(menuPos.getX() + TOT_OFFSETX, menuPos.getY() + TOT_OFFSETY, TOT_WIDTH*aux.size(), TOT_HEIGHT));
	ingTex->render(build_sdlrect(menuPos.getX() + ING_OFFSETX, menuPos.getY() + ING_OFFSETY, ING_WIDTH, ING_HEIGHT));
}
void  ShoppingMenuComp::increaseN() {
	if (number >= 99)return;
	++number;
	changePrice();
}
void  ShoppingMenuComp::decreaseN() {
	if (number <= 0)return;
	--number;
	changePrice();
}
void  ShoppingMenuComp::openMenu(_ecs::_ingredients_id _id) {
	if (!active) {
		parent->setActive<ShoppingMenuComp>(true);
		ing = _id;
		number = 1;
		price = _ecs::MarketIngs[ing - HARINA].price;
		totalPrice = number * price;
		numberTex = new Texture(sdl->renderer(), to_string(number), *font, build_sdlcolor(0xf3e5c2ff));
		priceTex = new Texture(sdl->renderer(), to_string(price), *font, build_sdlcolor(0x504631ff));
		ingTex = &((*sdl).images().at(to_string(ing)));
		changePrice();
	}
	else closeMenu();
}
void  ShoppingMenuComp::closeMenu() {
	if (active) {
		delete priceTex;
		priceTex = nullptr;
		delete numberTex;
		numberTex = nullptr;
		delete totalPriceTex;
		totalPriceTex = nullptr;
		parent->setActive<ShoppingMenuComp>(false);
	}
}
void  ShoppingMenuComp::changePrice() {
	totalPrice = number * price;
	numberTex = new Texture(sdl->renderer(), to_string(number), *font, build_sdlcolor(0xf3e5c2ff));
	totalPriceTex = new Texture(sdl->renderer(), to_string(totalPrice), *font, build_sdlcolor(0x504631ff));
}