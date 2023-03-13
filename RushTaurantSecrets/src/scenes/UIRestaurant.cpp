#include "UIRestaurant.h"

#include "../structure/GameObject.h"
#include "../components/Transform.h"
#include "../components/Image.h"
#include "../objects/Money.h" // cambiar cuando se cambie la clase Money
#include "../objects/Reputation.h"
#include "../gameObjects/Bin.h"
#include "../objects/Reputation.h"
#include "../components/Warning.h"

#include "../utils/checkML.h"

UIRestaurant::UIRestaurant() : Scene() {
	lastTime = sdl->currRealTime();
	numFullClock = 0;

	// instancia manager del dinero
	GameObject* moneyContainer = new GameObject(this);
	moneyTxt = Money::init(moneyContainer, 200);

	// pens� en hacerlo pasando un struct como par�metro, pero el struct ten�a que redefinirse demasiadas veces,
	// as� que Cleon me dijo que pasara directamente la informaci�n del struct como par�metro

	// icono de reputaci�n
	createIcon("REPUTATION_ICON", Vector(ICONX, ICONY), ICONSIZE, ICONSIZE, 0, grp_ICONS, hdr_REPUTATION);

	// icono de dinero
	createIcon("MONEY_ICON", Vector(ICONX, ICONY * 2 + ICONSIZE), ICONSIZE, ICONSIZE, 0, grp_ICONS, hdr_MONEY);

	// icono de objetivo diario
	createIcon("TARGET_ICON", Vector(ICONX, ICONY * 3 + ICONSIZE * 2), ICONSIZE, ICONSIZE, 0, grp_ICONS, hdr_TARGET);

	// icono de men� del d�a
	createIcon("DAILY_MENU_BUTTON", Vector(sdl->width() - 70, sdl->height() - 70), ICONSIZE, ICONSIZE, 0, grp_ICONS, hdr_MENU);

	// inventario (fondo)
	createIcon("INVENTORY_ICON", Vector(ICONX, sdl->height() - 244), 80, 228, 0, grp_ICONS, hdr_INVENTORY);

	// reloj (momento del d�a)
	createIcon("CLOCK", Vector(sdl->width() - ICONX - ICONSIZE * 2, ICONY), ICONSIZE * 2, ICONSIZE * 2, 0, grp_ICONS, hdr_CLOCK);

	// aguja del reloj
	arrow = createIcon("ARROW", Vector(sdl->width() - ICONX - ICONSIZE - 8, ICONY), ICONSIZE / 3, ICONSIZE, 0, grp_ICONS, hdr_ARROW);

	// inventario (platos)
	inventory = new Inventory(this);

	// gesti�n de la cantidad de dinero
	intMoney = moneyTxt->getMoney();
	std::string strMoney = std::to_string(intMoney);

	font = new Font(FONT_PATH, FONTSIZE);
	moneyTextTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x000000FF));

	moneyText = createIcon(moneyTextTexture, Vector(80, ICONY * 2 + ICONSIZE - 5), strMoney.length() * FONTSIZE / 2, FONTSIZE, 0, _ecs::grp_ICONS, _ecs::hdr_MONEY_TEXT);

	// render de estrellas vac�as
	for (int i = 0; i < stars.size(); i++) 
		createIcon("EMPTY_STAR", Vector(80 + i * 40, 25), 30, 32, 0, grp_ICONS, hdr_EMPTY_STAR);


	reputation = Reputation::instance();

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

void UIRestaurant::showMoneyText() {
	// si la cantidad de dinero ha variado, lo muestra por pantalla
	if (intMoney != moneyTxt->getMoney()) {
		intMoney = moneyTxt->getMoney();
		std::string strMoney = std::to_string(intMoney);
		delete(moneyTextTexture);
		moneyTextTexture = new Texture(sdl->renderer(), strMoney, *font, build_sdlcolor(0x000000FF));
		moneyTextImage->setTexture(moneyTextTexture);
		transform->setW(strMoney.length() * FONTSIZE / 2);
	}
}

void UIRestaurant::update() {
	Scene::update(); // cleon: falta el delta de tiempo (frametime)
	showMoneyText();
	//checkTime();
	reputationManager();
	updateClock();
}

void UIRestaurant::showTimeText() {
	std::string strTime = std::to_string(time);
	delete(timeTextTexture);
	timeTextTexture = new Texture(sdl->renderer(), strTime, *font, build_sdlcolor(0x000000FF));
	timeTextImage->setTexture(timeTextTexture);	
}

void UIRestaurant::checkTime() {
	// NO BORRAR (tomo los antiguos m�todos de tiempo como referencia)
	timeT = sdl->currRealTime();
	if (timeT - lastTime >= 1000) {
		time += 1;
		showTimeText();
		lastTime = timeT;
		timeT = 0;
	}
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
	if (actReputation > REP5) { // cleon: 80, 60... �no! esto provoca guerras y pandemias. CONSTANTES. Constantes son paz y amor. -> corregido
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

void UIRestaurant::updateClock() {
	timeT = sdl->currRealTime();
	if (timeT - lastTime >= TIME_CLOCK_REFRESH) {
		time += 1;
		auto transformArrow = arrow->getComponent<Transform>();
		transformArrow->setRot(transformArrow->getRot() + ANGLE_UPDATE);
		Vector posA;
		if (transformArrow->getRot() <= ANGLE) 
			posA = transformArrow->getPos() + Vector(1, 1);
		else if (transformArrow->getRot() <= ANGLE * 2)
			posA = transformArrow->getPos() + Vector(-1, 1);
		else if (transformArrow->getRot() <= ANGLE * 3)
			posA = transformArrow->getPos() + Vector(-1, -1);
		else
			posA = transformArrow->getPos() + Vector(1, -1);
		transformArrow->setPos(posA);
		lastTime = timeT;
		timeT = 0;

		if (transformArrow->getRot() == 0) numFullClock++;
	}
}