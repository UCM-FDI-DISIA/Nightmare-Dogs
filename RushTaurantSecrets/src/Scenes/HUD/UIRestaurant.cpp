#include "UIRestaurant.h"

#include "../../Structure/GameObject.h"
#include "../../Structure/GameManager.h"
#include "../../Components/Transform.h"
#include "../../Components/Image.h"
#include "../../Components/Warning.h"
#include "../../Managers/Money.h"
#include "../../Managers/Reputation.h"
#include "../../GameObjects/DailyMenu.h"
#include "../../GameObjects/Bin.h"
#include "../../GameObjects/Dialogue.h"
#include "../../GameObjects/ButtonGO.h"
#include "../../GameObjects/FreeText.h"
#include "../../Components/Streak.h"
#include "../../Managers/DayManager.h"
#include "../../Utilities/checkML.h"

UIRestaurant::UIRestaurant() : Scene() {
	lastTime = sdl->currRealTime();

	// instancia manager del dinero
	GameObject* moneyContainer = new GameObject(this);
	moneyTxt = GameManager::get()->getMoney();

	// Reputaci�n
	reputation = GameManager::get()->getReputation();
	// icono 
	createIcon("REPUTATION_ICON", Vector(ICONX, ICONY), ICONSIZE, ICONSIZE, 0, grp_ICONS);
	// estrellas vac�as
	for (int i = 0; i < stars.size(); i++)
		createIcon("EMPTY_STAR", Vector(80 + i * 40, 25), 30, 32, 0, grp_ICONS);

	fullStarTexture = &((*sdl).images().at("STAR"));
	actReputation = reputation->getReputation();

	// inicializa array de estrellas (define qu?estrellas se muestran y cu�les no)
	for (int i = 0; i < stars.size(); i++) {
		stars[i] = true;
	}


	// Fuente
	font = new Font(FONT_PATH, FONTSIZE);
	font1 = new Font(FONT_PATH, FONTSIZE - 5);
	outline = new Font(FONT_PATH, FONTSIZE);
	outline1 = new Font(FONT_PATH, FONTSIZE - 5);
	TTF_SetFontOutline(outline->getTTFFont(), 2);
	TTF_SetFontOutline(outline1->getTTFFont(), 2);


	// Dinero
	startingMoney = currentMoney = moneyTxt->getMoney();
	// icono
	createIcon("MONEY_ICON", Vector(ICONX, ICONY * 2 + ICONSIZE), ICONSIZE, ICONSIZE, 0, grp_ICONS);
	createIcon("TARGET_ICON", Vector(ICONX, ICONY * 3 + ICONSIZE * 2), ICONSIZE, ICONSIZE, 0, grp_ICONS);
	// Texto
	std::string strMoney = std::to_string(currentMoney);
	moneyTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x3a3a50FF));
	moneyOutline = new Texture(sdl->renderer(), strMoney, *outline, build_sdlcolor(0xFFFFFFFF));
	moneyRect = { 80, (int)(ICONY * 2 + ICONSIZE - 5), moneyTexture->width() ,moneyTexture->height() };
	moneyOutlineRect = moneyRect;
	moneyOutlineRect.x -= CENTEROUTLINE / 2; moneyOutlineRect.w += CENTEROUTLINE;
	moneyOutlineRect.y -= CENTEROUTLINE / 2; moneyOutlineRect.h += CENTEROUTLINE;
	// Se crea para que no de problemas
	objectiveTexture = new Texture(sdl->renderer(), "a", *font, build_sdlcolor(0x3a3a50FF));
	objectiveOutline = new Texture(sdl->renderer(), "a", *outline, build_sdlcolor(0xFFFFFFFF));

	// inventario (fondo)
	GameObject* aux = createIcon("INVENTORY_ICON", Vector(ICONX, sdl->height() - 302 - ICONX), 82, 302, 0, grp_ICONS);
	(new ShowControlAuto(aux, { {ControlsType::key_LEFT,ControlsType::play_LS,ControlsType::xbox_LS,Vector(25,-10),40,40},{ControlsType::key_RIGHT,ControlsType::play_RS,ControlsType::xbox_RS,Vector(60,-10),40,40} }))->setActive(true);

	// inventario (platos)
	inventory = new Inventory(this);

	// Exclamaci�n de la despensa
	
	GameObject* thiefExclamation = createIcon("EXCLAMATION", Vector(640, 85), 32, 32);
	new Warning(thiefExclamation);
	

	// Racha de cobros
	GameObject* streak = new GameObject(this, _ecs::grp_HUD, _ecs::hdr_STREAK);
	new Streak(streak, 10, Vector(820, 260), 430, 30, 2, font);

	// Reloj
	clock = new Clock(this);

	// icono de men?del d�a
	menuToggled = true;
	menu = new DailyMenu(this, "DAILY_MENU", Vector((sdlutils().width() / 2) - 239.5f, 30),
		479.0f, 640.0f, []() {});
	menu->getComponent<ButtonComp>()->setActive(false);
	toggleDailyMenu();
    aux = new GameObject(this, _ecs::grp_ICONS);
	new Transform(aux, Vector(sdl->width() - 70, sdl->height() - 70), Vector(0, 0), ICONSIZE, ICONSIZE);
	new Image(aux, "DAILY_MENU_BUTTON");
	(new ShowControlAuto(aux, { {ControlsType::key_V,ControlsType::play_Rectangle,ControlsType::xbox_X,Vector(-10,40),40,40} }))->setActive(true);

	accDay = 1;
	dayText = "DAY " + to_string(accDay);
	dayTexture = new Texture(sdl->renderer(), dayText, *font1, build_sdlcolor(0x3a3a50FF));
	dayOutline = new Texture(sdl->renderer(), dayText, *outline1, build_sdlcolor(0xFFFFFFFF));
}

UIRestaurant::~UIRestaurant() {
	delete font;
	delete outline;
	delete moneyTexture;
	delete moneyOutline;
	delete objectiveTexture;
	delete objectiveOutline;
	delete dayTexture;
	delete dayOutline;
	delete font1;
	delete outline1;
}

GameObject* UIRestaurant::dataIcon(Texture* texture, Vector position, float width, float height, float rotation,
	_ecs::_grp_id grp = _ecs::grp_GENERAL, _ecs::_hdr_id handler = _ecs::hdr_INVALID) {

	GameObject* gameObject = new GameObject(this, grp, handler);
	new Transform(gameObject, position, Vector(0, 0), width, height, rotation);
	new Image(gameObject, texture);

	return gameObject;
}

GameObject* UIRestaurant::createIcon(string textureName, Vector position, float width, float height, float rotation,
	_ecs::_grp_id grp, _ecs::_hdr_id handler) {

	return dataIcon(&((*sdl).images().at(textureName)), position, width, height, rotation, grp, handler);
}

GameObject* UIRestaurant::createIcon(Texture* texture, Vector position, float width, float height, float rotation,
	_ecs::_grp_id grp, _ecs::_hdr_id handler) {

	return dataIcon(texture, position, width, height, rotation, grp, handler);
}

void UIRestaurant::toggleDailyMenu()
{
	menuToggled = !menuToggled;
	menu->getComponent<Transform>()->setActive(menuToggled);
	menu->getComponent<Image>()->setActive(menuToggled);
	menu->getComponent<DailyMenuComp>()->setActive(menuToggled);
}

void UIRestaurant::showMoneyText() {
	// si la cantidad de dinero ha variado, lo muestra por pantalla
	if (currentMoney != moneyTxt->getMoney()) {
		moneyDiff += moneyTxt->getMoney() - startingMoney;
		currentMoney = moneyTxt->getMoney();
		std::string strMoney = std::to_string(currentMoney);

		delete moneyTexture;
		delete moneyOutline;
		moneyTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x3a3a50FF));
		moneyOutline = new Texture(sdl->renderer(), strMoney, *outline, build_sdlcolor(0xFFFFFFFF));
		moneyRect = { 80, (int)(ICONY * 2 + ICONSIZE - 5), moneyTexture->width() ,moneyTexture->height() };
		moneyOutlineRect = moneyRect;
		moneyOutlineRect.x -= CENTEROUTLINE / 2; moneyOutlineRect.w += CENTEROUTLINE;
		moneyOutlineRect.y -= CENTEROUTLINE / 2; moneyOutlineRect.h += CENTEROUTLINE;

		delete objectiveTexture;
		delete objectiveOutline;
		std::string strObj = std::to_string(moneyDiff) + "/" + std::to_string(intObjective);
		objectiveTexture = new Texture(sdl->renderer(), strObj, *font, build_sdlcolor(0x3a3a50FF));
		objectiveOutline = new Texture(sdl->renderer(), strObj, *outline, build_sdlcolor(0xFFFFFFFF));
		objectiveRect = { 80, (int)(ICONY * 3 + ICONSIZE * 2), objectiveTexture->width(), objectiveTexture->height() };
		objectiveOutlineRect = objectiveRect;
		objectiveOutlineRect.x -= CENTEROUTLINE / 2; objectiveOutlineRect.w += CENTEROUTLINE;
		objectiveOutlineRect.y -= CENTEROUTLINE / 2; objectiveOutlineRect.h += CENTEROUTLINE;
	}
}

void UIRestaurant::update() {
	Scene::update();
	showMoneyText();
	reputationManager();
	ClockComponent::get()->update();
}


void UIRestaurant::handleEvents() {
	if (ih->joysticksInitialised() && ih->getButtonState(0, SDL_CONTROLLER_BUTTON_X) || !ih->joysticksInitialised() && ih->isKeyDown(SDLK_v)) {
		toggleDailyMenu();
	}
	else Scene::handleEvents();
}
void UIRestaurant::renderStar(int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = 30;
	dest.h = 32;

	// renderiza la textura
	fullStarTexture->render(dest);
}

void UIRestaurant::reputationManager() {
	// comprueba si la reputaci�n ha cambiado
	actReputation = reputation->getReputation();
	checkStarsArray();
}

void UIRestaurant::checkStarsArray() {
	stars[0] = actReputation > REP1;
	stars[1] = actReputation > REP2;
	stars[2] = actReputation > REP3;
	stars[3] = actReputation > REP4;
	stars[4] = actReputation > REP5;
}

void UIRestaurant::checkRenderStar() {
	for (int i = 0; i < stars.size(); i++) {
		if (stars[i]) {
			// desplazamiento de 80, y 40 por estrella
			renderStar(80 + i * 40, 25);
		}
	}
}

void UIRestaurant::setDailyMenu() {
	menu->getComponent<DailyMenuComp>()->setMenu(GameManager::instance()->getTodaysMenu());
}

void UIRestaurant::render() {
	moneyOutline->render(moneyOutlineRect);
	moneyTexture->render(moneyRect);

	objectiveOutline->render(objectiveOutlineRect);
	objectiveTexture->render(objectiveRect);
	
	dayOutline->render({ sdl->width() - 25 - dayTexture->width() - 2, 120 - 2, dayOutline->width(), dayOutline->height() });
	dayTexture->render({ sdl->width() - 25 - dayTexture->width(), 120, dayTexture->width(), dayTexture->height() });
	
	Scene::render();
	checkRenderStar();
	ClockComponent::get()->render();
}


void UIRestaurant::reset() {
	startingMoney = moneyTxt->getMoney();
	currentMoney = -1;
	showMoneyText();

	delete dayTexture;
	delete dayOutline;

	accDay = GameManager::get()->getDayManager()->getDay();
	dayText = "DAY " + to_string(accDay);
	dayTexture = new Texture(sdl->renderer(), dayText, *font1, build_sdlcolor(0x3a3a50FF));
	dayOutline = new Texture(sdl->renderer(), dayText, *outline1, build_sdlcolor(0xFFFFFFFF));
}

void UIRestaurant::nextDay() {
	intObjective = GameManager::get()->getDayManager()->getDailyObjective(); moneyDiff = 0;

	delete objectiveTexture;
	delete objectiveOutline;
	std::string strObj = std::to_string(moneyDiff) + "/" + std::to_string(intObjective);
	objectiveTexture = new Texture(sdl->renderer(), strObj, *font, build_sdlcolor(0x3a3a50FF));
	objectiveOutline = new Texture(sdl->renderer(), strObj, *outline, build_sdlcolor(0xFFFFFFFF));
	objectiveRect = { 80, (int)(ICONY * 3 + ICONSIZE * 2), objectiveTexture->width(), objectiveTexture->height() };
	objectiveOutlineRect = objectiveRect;
	objectiveOutlineRect.x -= CENTEROUTLINE / 2; objectiveOutlineRect.w += CENTEROUTLINE;
	objectiveOutlineRect.y -= CENTEROUTLINE / 2; objectiveOutlineRect.h += CENTEROUTLINE;

	delete dayTexture;
	delete dayOutline;

	accDay = GameManager::get()->getDayManager()->getDay();
	dayText = "DAY " + to_string(accDay);
	dayTexture = new Texture(sdl->renderer(), dayText, *font1, build_sdlcolor(0x3a3a50FF));
	dayOutline = new Texture(sdl->renderer(), dayText, *outline1, build_sdlcolor(0xFFFFFFFF));

	ClockComponent::get()->nextDay();

	Scene::nextDay();
}