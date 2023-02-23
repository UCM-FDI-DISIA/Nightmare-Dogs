#include "UIRestaurant.h"

#include "../utils/checkML.h"

UIRestaurant::UIRestaurant() : Scene() {
	lastTime = sdl->currRealTime();

	// icono de reputaci�n
	GameObject* reputation = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_REPUTATION);
	new Transform(reputation, Vector(20, 20), Vector(0, 0), 64, 44, 0);
	new Image(reputation, &((*sdl).images().at("REPUTATION_ICON")));

	// icono de dinero
	GameObject* money = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_MONEY);
	new Transform(money, Vector(10, 76), Vector(0, 0), 64, 64, 0);
	new Image(money, &((*sdl).images().at("MONEY_ICON")));

	// icono de objetivo diario
	GameObject* target = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_TARGET);
	new Transform(target, Vector(17, 150), Vector(0, 0), 72, 68, 0);
	new Image(target, &((*sdl).images().at("TARGET_ICON")));

	// icono de men� del d�a
	GameObject* menu = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_MENU);
	new Transform(menu, Vector(sdl->width() - 70, sdl->height() - 70), Vector(0, 0), 54, 54, 0);
	new Image(menu, &((*sdl).images().at("DAILY_MENU")));

	// inventario (fondo)
	GameObject* inv = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_INVENTORY);
	new Transform(inv, Vector(20, sdl->height() - 244), Vector(0, 0), 80, 224, 0);
	new Image(inv, &((*sdl).images().at("INVENTORY_ICON")));

	// inventario (platos)
	inventory = new Inventory(this);

	// icono de reloj (temporizador)
	GameObject* clock = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_CLOCK);
	new Transform(clock, Vector(sdl->width() - 70, 20), Vector(0, 0), 60, 57, 0);
	new Image(clock, &((*sdl).images().at("CLOCK_ICON")));

	// !! �mostrar para indicar la tecla a pulsar para mostrar el men� de pausa?
	// icono de men� de pausa
	//GameObject* pause = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_PAUSE);
	//new Transform(pause, Vector(sdl->width() - 70, 20), Vector(0, 0), 50, 50, 0);
	//new Image(pause, &((*sdl).images().at("PAUSE_BUTTON")));

	// gesti�n de la cantidad de dinero
	f = new Font("assets/Fonts/Hamish.ttf", 50);
	moneyText = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_MONEY_TEXT);
	new Transform(moneyText, Vector(90, 80), Vector(0, 0), 80, 50);
	intMoney = moneyTxt->getMoney();
	std::string strMoney = std::to_string(intMoney);
	//moneyTextTexture = new Texture(sdl->renderer(), strMoney, *f, build_sdlcolor(0xFFC863ff));
	moneyTextTexture = new Texture(sdl->renderer(), strMoney, *f, build_sdlcolor(0x000000FF));
	moneyTextImage = new Image(moneyText, moneyTextTexture);

	// gesti�n del temporizador
	timeText = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_TIME_TEXT);
	new Transform(timeText, Vector(sdl->width() - 140, 25), Vector(0, 0), 50, 50);
	std::string strTime = std::to_string(time);
	timeTextTexture = new Texture(sdl->renderer(), strTime, *f, build_sdlcolor(0x000000FF));
	timeTextImage = new Image(timeText, timeTextTexture);

	// render de estrellas vac�as
	GameObject* estar1 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_EMPTY_STAR);
	new Transform(estar1, Vector(100, 25), Vector(0,0), 30, 32);
	new Image(estar1, &((*sdl).images().at("EMPTY_STAR")));

	GameObject* estar2 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_EMPTY_STAR);
	new Transform(estar2, Vector(140, 25), Vector(0, 0), 30, 32);
	new Image(estar2, &((*sdl).images().at("EMPTY_STAR")));

	GameObject* estar3 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_EMPTY_STAR);
	new Transform(estar3, Vector(180, 25), Vector(0, 0), 30, 32);
	new Image(estar3, &((*sdl).images().at("EMPTY_STAR")));

	GameObject* estar4 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_EMPTY_STAR);
	new Transform(estar4, Vector(220, 25), Vector(0, 0), 30, 32);
	new Image(estar4, &((*sdl).images().at("EMPTY_STAR")));

	GameObject* estar5 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_EMPTY_STAR);
	new Transform(estar5, Vector(260, 25), Vector(0, 0), 30, 32);
	new Image(estar5, &((*sdl).images().at("EMPTY_STAR")));

	// render de estrellas rellenas
	GameObject* star1 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_STAR);
	new Transform(star1, Vector(100, 25), Vector(0, 0), 30, 32);
	new Image(star1, &((*sdl).images().at("STAR")));

	GameObject* star2 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_STAR);
	new Transform(star2, Vector(140, 25), Vector(0, 0), 30, 32);
	new Image(star2, &((*sdl).images().at("STAR")));

	GameObject* star3 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_STAR);
	new Transform(star3, Vector(180, 25), Vector(0, 0), 30, 32);
	new Image(star3, &((*sdl).images().at("STAR")));

	GameObject* star4 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_STAR);
	new Transform(star4, Vector(220, 25), Vector(0, 0), 30, 32);
	new Image(star4, &((*sdl).images().at("STAR")));

	GameObject* star5 = new GameObject(this, _ecs::grp_ICONS, _ecs::hdr_STAR);
	new Transform(star5, Vector(260, 25), Vector(0, 0), 30, 32);
	new Image(star5, &((*sdl).images().at("STAR")));
}

void UIRestaurant::showMoneyText() {
	// si la cantidad de dinero ha variado, lo muestra por pantalla
	if (intMoney != moneyTxt->getMoney()) {
		intMoney = moneyTxt->getMoney();
		std::string strMoney = std::to_string(intMoney);
		delete(moneyTextTexture);
		moneyTextTexture = new Texture(sdl->renderer(), strMoney, *f, build_sdlcolor(0x000000FF));
		moneyTextImage->setTexture(moneyTextTexture);
	}
}

void UIRestaurant::update() {
	Scene::update();
	showMoneyText();
	checkTime();
}

void UIRestaurant::showTimeText() {
	std::string strTime = std::to_string(time);
	delete(timeTextTexture);
	timeTextTexture = new Texture(sdl->renderer(), strTime, *f, build_sdlcolor(0x000000FF));
	timeTextImage->setTexture(timeTextTexture);	
}

void UIRestaurant::checkTime() {
	timeT = sdl->currRealTime();
	if (timeT - lastTime >= 1000) {
		time += 1;
		showTimeText();
		lastTime = timeT;
		timeT = 0;
	}
}