#include "UIMarket.h"
#include "../../Structure/GameManager.h"
#include "../../Managers/Money.h"
#include "../../Managers/DayManager.h"

#include "../../Components/ShoppingMenuComp.h"
#include "../../Utilities/checkML.h"

void UIMarket::toggleDailyMenu() {
	menuToggled = !menuToggled;
	menu->getComponent<Transform>()->setActive(menuToggled);
	menu->getComponent<Image>()->setActive(menuToggled);
	menu->getComponent<DailyMenuComp>()->setActive(menuToggled);
}

UIMarket::UIMarket(Scene* market) : Scene(),market(market) {
	// instancia manager del dinero
	GameObject* moneyContainer = new GameObject(this);
	moneyTxt = GameManager::get()->getMoney();
	ih = InputHandler::instance();
	basketMarket = new BasketMarket(this);

	// Fuente
	font = new Font(FONT_PATH, FONTSIZE);
	outline = new Font(FONT_PATH, FONTSIZE);
	TTF_SetFontOutline(outline->getTTFFont(), 2);
	font1 = new Font(FONT_PATH, FONTSIZE + 10);
	outline1 = new Font(FONT_PATH, FONTSIZE + 10);
	TTF_SetFontOutline(outline1->getTTFFont(), 2);

	// Dinero
	intMoney = moneyTxt->getMoney();
	// icono
	createGameObjects(_ecs::grp_ICONS, "MONEY_ICON", Vector(ICONX, ICONY), ICONSIZE, ICONSIZE, 0);
	// Texto
	std::string strMoney = std::to_string(intMoney);
	moneyTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x3a3a50FF));
	moneyOutline = new Texture(sdl->renderer(), strMoney, *outline, build_sdlcolor(0xFFFFFFFF));
	moneyRect = { 80, (int)(ICONY - 2), moneyTexture->width() ,moneyTexture->height() };
	moneyOutlineRect = moneyRect;
	moneyOutlineRect.x -= CENTEROUTLINE / 2; moneyOutlineRect.w += CENTEROUTLINE;
	moneyOutlineRect.y -= CENTEROUTLINE / 2; moneyOutlineRect.h += CENTEROUTLINE;

	//objetivo diario
	createGameObjects(_ecs::grp_ICONS, "TARGET_ICON", Vector(ICONX - 2, ICONY + TARGETOFFSET), ICONSIZE +5, ICONSIZE + 5, 0);
	target = GameManager::get()->getDayManager()->getDailyObjective();
	targetText = to_string(target);
	targetTexture = new Texture(sdl->renderer(), targetText, *font, build_sdlcolor(0x3a3a50FF));
	targetOutline = new Texture(sdl->renderer(), targetText, *outline, build_sdlcolor(0xFFFFFFFF));
	targetRect = { 80, (int)(ICONY + TARGETOFFSET - 2), targetTexture->width() ,targetTexture->height() };
	targetOutlineRect = targetRect;
	targetOutlineRect.x -= CENTEROUTLINE / 2; targetOutlineRect.w += CENTEROUTLINE;
	targetOutlineRect.y -= CENTEROUTLINE / 2; targetOutlineRect.h += CENTEROUTLINE;

	// icono de cesta
	GameObject* aux=createGameObjects(_ecs::grp_ICONS, "BASKET_YELLOW", Vector(20, sdl->height() - 90), 68, 70, 0);
	(new ShowControlAuto(aux,{{ControlsType::key_Z,ControlsType::play_Triangle,ControlsType::xbox_Y,Vector(70,60),40,40}}))->setActive(true);

	//men�� de compra
	shopMenu = new GameObject(this, _ecs::grp_GENERAL, hdr_SHOP_MENU1);
	new ShoppingMenuComp(shopMenu);

	// icono de men?del d�a
	menuToggled = true;
	menu = new DailyMenu(this, "DAILY_MENU", Vector((sdlutils().width() / 2) - 239.5f, 30),
		479.0f, 640.0f, []() {});
	menu->getComponent<ButtonComp>()->setActive(false);
	toggleDailyMenu();
	
	accDay = 1;
	dayText = "DAY " + to_string(accDay);
	dayTexture = new Texture(sdl->renderer(), dayText, *font1, build_sdlcolor(0x3a3a50FF));
	dayOutline = new Texture(sdl->renderer(), dayText, *outline1, build_sdlcolor(0xFFFFFFFF));
	aux = createGameObjects(_ecs::grp_ICONS, "DAILY_MENU_BUTTON", Vector(sdl->width() - 70, sdl->height() - 70), ICONSIZE, ICONSIZE,0);
	(new ShowControlAuto(aux, { {ControlsType::key_V,ControlsType::play_Rectangle,ControlsType::xbox_X,Vector(-10,40),40,40} }))->setActive(true);

	buybutton = new ButtonGO(this, "BUY_BUTTON", "BUTTON_HIGHLIGHT", Vector(BUTTONS_X, BUTTONS_Y), BUTTONS_W, BUTTONS_H,
		[&]() {
			
		});
	buybutton->setActives(false);
}

UIMarket::~UIMarket() {
	delete font;
	delete outline;
	delete font1;
	delete outline1;
	delete moneyTexture;
	delete moneyOutline;
	delete targetTexture;
	delete targetOutline;
	delete dayTexture;
	delete dayOutline;
}

void UIMarket::reset() {
	target = GameManager::get()->getDayManager()->getDailyObjective();
	targetText = to_string(target);

	delete targetTexture;
	delete targetOutline;

	targetTexture = new Texture(sdl->renderer(), targetText, *font, build_sdlcolor(0x3a3a50FF));
	targetOutline = new Texture(sdl->renderer(), targetText, *outline, build_sdlcolor(0xFFFFFFFF));
	targetRect = { 80, (int)(ICONY + TARGETOFFSET - 2), targetTexture->width() ,targetTexture->height() };
	targetOutlineRect = targetRect;
	targetOutlineRect.x -= CENTEROUTLINE / 2; targetOutlineRect.w += CENTEROUTLINE;
	targetOutlineRect.y -= CENTEROUTLINE / 2; targetOutlineRect.h += CENTEROUTLINE;

	delete dayTexture;
	delete dayOutline;

	accDay = GameManager::get()->getDayManager()->getDay();
	dayText = "DAY " + to_string(accDay);
	dayTexture = new Texture(sdl->renderer(), dayText, *font1, build_sdlcolor(0x3a3a50FF));
	dayOutline = new Texture(sdl->renderer(), dayText, *outline1, build_sdlcolor(0xFFFFFFFF));

	basketMarket->nextDay();
}


void UIMarket::update() {
	Scene::update();
	showMoneyText();
}

GameObject* UIMarket::createGameObjects(_ecs::_grp_id grp, string textureName,
	Vector position, float width, float height, float rotation, _ecs::_hdr_id handler) {
	GameObject* gameObject = new GameObject(this, grp, handler);
	new Transform(gameObject, position, Vector(0, 0), width, height, rotation);
	new Image(gameObject, &((*sdl).images().at(textureName)));
	return gameObject;
}


void UIMarket::showMoneyText() {
	// si la cantidad de dinero ha variado, lo muestra por pantalla
	if (intMoney != moneyTxt->getMoney()) {
		intMoney = moneyTxt->getMoney();
		std::string strMoney = std::to_string(intMoney);
		
		delete moneyTexture;
		delete moneyOutline;
		moneyTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x3a3a50FF));
		moneyOutline = new Texture(sdl->renderer(), strMoney, *outline, build_sdlcolor(0xFFFFFFFF));
		moneyRect = { 80, (int)(ICONY - 2), moneyTexture->width() ,moneyTexture->height() };
		moneyOutlineRect = moneyRect;
		moneyOutlineRect.x -= CENTEROUTLINE / 2; moneyOutlineRect.w += CENTEROUTLINE;
		moneyOutlineRect.y -= CENTEROUTLINE / 2; moneyOutlineRect.h += CENTEROUTLINE;
	}
}

void UIMarket::handleEvents() {

	if (ih->joysticksInitialised() && ih->getButtonState(0, SDL_CONTROLLER_BUTTON_Y) || !ih->joysticksInitialised() && ih->isKeyDown(SDLK_z)) {
		// si el men?de cesta ya est?abierto, lo cierra
		if (basketMarket->getComponent<BasketMarketComponent>()->getBasketON())
			basketMarket->getComponent<BasketMarketComponent>()->setBasketON(false);
		// si el men?de cesta est?cerrado, lo abre
		else
			basketMarket->getComponent<BasketMarketComponent>()->setBasketON(true);
	}
	else if (ih->joysticksInitialised() && ih->getButtonState(0, SDL_CONTROLLER_BUTTON_X) || !ih->joysticksInitialised() && ih->isKeyDown(SDLK_v)) {
		toggleDailyMenu();
	}
	else
		Scene::handleEvents();

	if (buybutton->isActive()) {
		if (ih->isKeyDown(SDL_SCANCODE_DOWN)) {
			buybutton->getComponent<ButtonComp>()->setHighlighted(true);
		}
		else if (ih->isKeyDown(SDL_SCANCODE_UP)) {
			buybutton->getComponent<ButtonComp>()->setHighlighted(false);
		}
	}
}

void UIMarket::render() {
	moneyOutline->render(moneyOutlineRect);
	moneyTexture->render(moneyRect);

	targetOutline->render(targetOutlineRect);
	targetTexture->render(targetRect);

	dayOutline->render({ sdl->width() - 25 - dayTexture->width() - 2, 15 - 2, dayOutline->width(), dayOutline->height()});
	dayTexture->render({ sdl->width() - 25 - dayTexture->width(), 15, dayTexture->width(), dayTexture->height()});

	if (basketMarket->getComponent<BasketMarketComponent>()->getBasketON()) {
		basketMarket->getComponent<BasketMarketComponent>()->renderBasket();
	}

	Scene::render();
}

void UIMarket::setDailyMenu() {
	menu->getComponent<DailyMenuComp>()->setMenu(GameManager::instance()->getTodaysMenu());
}

BasketMarket* UIMarket::basketM() {
	return basketMarket;
}

ButtonGO* UIMarket::BuyButton() {
	return buybutton;
}
