#include "UIRestaurant.h"

#include "../structure/GameObject.h"
#include "../structure/GameManager.h"
#include "../components/Transform.h"
#include "../components/Image.h"
#include "../components/Warning.h"
#include "../objects/Money.h"
#include "../objects/Reputation.h"
#include "../gameObjects/DailyMenu.h"
#include "../gameObjects/Bin.h"
#include "../gameObjects/Dialogue.h"
#include "../gameObjects/ButtonGO.h"

#include "../utils/checkML.h"

UIRestaurant::UIRestaurant() : Scene() {
	lastTime = sdl->currRealTime();

	// instancia manager del dinero
	GameObject* moneyContainer = new GameObject(this);
	moneyTxt = GameManager::instance()->getMoney();

	// pens� en hacerlo pasando un struct como par�metro, pero el struct ten�a que redefinirse demasiadas veces,
	// as� que Cleon me dijo que pasara directamente la informaci�n del struct como par�metro

	// icono de reputaci�n
	createIcon("REPUTATION_ICON", Vector(ICONX, ICONY), ICONSIZE, ICONSIZE, 0, grp_ICONS);

	// icono de dinero
	createIcon("MONEY_ICON", Vector(ICONX, ICONY * 2 + ICONSIZE), ICONSIZE, ICONSIZE, 0, grp_ICONS);

	// icono de objetivo diario
	createIcon("TARGET_ICON", Vector(ICONX, ICONY * 3 + ICONSIZE * 2), ICONSIZE, ICONSIZE, 0, grp_ICONS);

	//men� del d�a
	menu = new DailyMenu(this, "DAILY_MENU", Vector((sdlutils().width() / 2) - 239.5f, sdl->height() / 15), 479.0f, 640.0f, []() {});
	menuToggled = true;
	toggleDailyMenu();
	menu->getComponent<ButtonComp>()->setActive(false);

	// icono de men� del d�a
	//createIcon("DAILY_MENU_BUTTON", Vector(sdl->width() - 70, sdl->height() - 70), ICONSIZE, ICONSIZE, 0, grp_ICONS);
	new ButtonGO(this, "DAILY_MENU_BUTTON", "DAILY_MENU_BUTTON", Vector(sdl->width() - 70, sdl->height() - 70), ICONSIZE, ICONSIZE, 
		[&]() {
			toggleDailyMenu();
		});
	// inventario (fondo)
	createIcon("INVENTORY_ICON", Vector(ICONX, sdl->height() - 302 - ICONX), 82, 302, 0, grp_ICONS);

	// inventario (platos)
	inventory = new Inventory(this);

	// gesti�n de la cantidad de dinero
	intMoney = moneyTxt->getMoney();
	std::string strMoney = std::to_string(intMoney);

	font = new Font(FONT_PATH, FONTSIZE);
	moneyTextTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x000000FF));
	moneyText = createIcon(moneyTextTexture, Vector(80, ICONY * 2 + ICONSIZE - 5), strMoney.length() * FONTSIZE / 2, FONTSIZE, 0, _ecs::grp_ICONS);
	moneyTextImage = new Image(moneyText, moneyTextTexture);

	// render de estrellas vac�as
	for (int i = 0; i < stars.size(); i++) 
		createIcon("EMPTY_STAR", Vector(80 + i * 40, 25), 30, 32, 0, grp_ICONS);


	reputation = GameManager::instance()->getReputation();

	fullStarTexture = &((*sdl).images().at("STAR"));
	actReputation = reputation->getReputation();

	// inicializa array de estrellas (define qu� estrellas se muestran y cu�les no)
	for (int i = 0; i < stars.size(); i++) {
		stars[i] = true;
	}

	float frequency = 1000;
	GameObject* thiefExclamation = createIcon("EXCLAMATION", Vector(640, 85), 32, 32);
	new Warning(thiefExclamation, frequency);

	intObjective = 30;
	objectiveTextTexture = new Texture(sdl->renderer(), std::to_string(intObjective), *font, build_sdlcolor(0x000000FF));
	createIcon(objectiveTextTexture, Vector(80, ICONY * 3 + ICONSIZE * 2), std::to_string(intObjective).length() * FONTSIZE / 2, FONTSIZE, 0, _ecs::grp_ICONS);
	
	// new Dialogue(this, Vector(500, 200), 500, 0.01 * 1000, { "Al venir al mundo fueron delicadamente mecidas por las manos de la lustral Doniazada, su buena tia.", "Hola hola hola hola me llamo \n Pedro"});

	new Clock(this);
}

UIRestaurant::~UIRestaurant() {
	delete font;
	delete moneyTextTexture;
	delete objectiveTextTexture;
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
	if (intMoney != moneyTxt->getMoney()) {
		intMoney = moneyTxt->getMoney();
		std::string strMoney = std::to_string(intMoney);
		
		delete moneyTextTexture;
		moneyTextTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x000000FF));
		moneyText->getComponent<Transform>()->setW(strMoney.length() * FONTSIZE / 2);
		moneyTextImage->setTexture(moneyTextTexture);
	}
}

void UIRestaurant::update() {
	Scene::update(); // cleon: falta el delta de tiempo (frametime)
	showMoneyText();
	//checkTime();
	reputationManager();
	//updateClock();
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
	// si la reputaci�n es mayor de ochenta
	if (actReputation > REP5) {
		stars[0] = true;
		stars[1] = true;
		stars[2] = true;
		stars[3] = true;
		stars[4] = true;
	}

	// si la reputaci�n es mayor de sesenta
	else if (actReputation > REP4) {
		stars[0] = true;
		stars[1] = true;
		stars[2] = true;
		stars[3] = true;
		stars[4] = false;
	}

	// si la reputaci�n es mayor de cuarenta
	else if (actReputation > REP3) {
		stars[0] = true;
		stars[1] = true;
		stars[2] = true;
		stars[3] = false;
		stars[4] = false;
	}

	// si la reputaci�n es mayor de veinte
	else if (actReputation > REP2) {
		stars[0] = true;
		stars[1] = true;
		stars[2] = false;
		stars[3] = false;
		stars[4] = false;
	}

	// si la reputaci�n es mayor de cero
	else if (actReputation > REP1) {
		stars[0] = true;
		stars[1] = false;
		stars[2] = false;
		stars[3] = false;
		stars[4] = false;
	}

	// si la reputaci�n es cero o menor
	else if (actReputation < REP0) {
		stars[0] = false;
		stars[1] = false;
		stars[2] = false;
		stars[3] = false;
		stars[4] = false;
	}
}

void UIRestaurant::checkRenderStar() {
	for (int i = 0; i < stars.size(); i++) {
		if (stars[i]) {
			// desplazamiento de 80, y 40 por estrella
			renderStar(80 + i * 40, 25);
		}
	}
}

void UIRestaurant::render() {
	Scene::render();
	checkRenderStar();
}