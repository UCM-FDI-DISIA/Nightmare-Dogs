#include "Restaurant.h"

#include "../../Structure/GameManager.h"
#include "../../Managers/DishCombinator.h"
#include "../../Managers/CollisionsManager.h"
#include "../../Components/MapCreator.h" 
#include "../../GameObjects/Player.h"
#include "../../Definitions/Paths_def.h"
#include "../../Managers/ClientsManager.h"
#include "../../Managers/DayManager.h"
#include "../../GameObjects/TimeOfDayObj.h"
#include "../../Structure/GameObject.h"
#include "../../GameObjects/OtherPlayer.h"

#include "../../Utilities/checkML.h"
#include <set>

Restaurant::Restaurant(): dc(DishCombinator::init()), pantry(nullptr), sdl(SDLUtils::instance()), restaurantMusic(&sdl->musics().at("RESTAURANT_MUSIC")),
pantryMusic(&sdl->musics().at("PANTRY_MUSIC")) {
	ui = new UIRestaurant();
	cm = new CollisionsManager(this);
	player = new Player(this, 0);
	new OtherPlayer(this, 0);
	pantryMusic->setMusicVolume(MUSIC_VOL);
	restaurantMusic->setMusicVolume(MUSIC_VOL);
}

Restaurant::~Restaurant() {
	delete ui;
	delete cm;
}

void Restaurant::reset() {
	getGameObject(_ecs::hdr_PLAYER)->setActives(true);
	player->getComponent<Transform>()->setPos(INITIAL_POS);
	player->getComponent<Transform>()->setOrientation(south);
	ui->reset();
	ui->nextDay();
}

void Restaurant::callAfterCreating() {

	new TimeOfDayObj(this, { 0,100 }, sdlutils().getLoadedTilesets().at("restaurantAfternoon"), sdlutils().getLoadedTilesets().at("restaurantNight"));
	new TimeOfDayObj(this, { 0,100 }, sdlutils().getLoadedTilesets().at("restaurantAfternoonTop"), sdlutils().getLoadedTilesets().at("restaurantNightTop"),Top);

	// clientsManager
	pantry = GameManager::get()->getPantry();

	GameObject* managerContainer = new GameObject(this);
	ClientsManager::init(managerContainer, menu(), 6 * 1000, 2, 3);


	dm = GameManager::get()->getDayManager();

	createMap();

	initRender();

	initComponent();

	ui->callAfterCreating();
	player->getComponent<PlayerMovementController>()->initP();
	player->getComponent<Transform>()->setPos(PANTRY_POS);
}


vector<_ecs::_dish_id> Restaurant::menu() const {
	// Men?del d�a aleatorio (lo rellena con 4 platos diferentes
	// entre s?y los pasa a un vector para poder acceder a ellos)
	set<int> aux;
	for (int i = 0; i < 4; i++) {
		if (!aux.insert(rand() % _ecs::NONE_DISH).second)
			i--;
	}
	vector<_ecs::_dish_id> menu;
	for (auto i = aux.begin(); i != aux.end(); ++i) menu.push_back((_ecs::_dish_id)*i);

	return menu;
}

void Restaurant::createMap() {
	Scene::createMap("assets/tilemaps/restaurant.tmx", Down, Vector());
	Scene::createMap("assets/tilemaps/restaurant_top_walls.tmx", Top, Vector());
	Scene::createMap("assets/tilemaps/restaurant_top_kitchenIsland.tmx", Middle, Vector(0, 370 * RESIZEFACTOR));
	Scene::createMap("assets/tilemaps/restaurant_top_table.tmx", Middle, Vector(0, 800 * RESIZEFACTOR));
	Scene::createMap("assets/tilemaps/restaurant_top_counter.tmx", Middle, Vector(0, 520 * RESIZEFACTOR));
}

void Restaurant::render() {
	renderLayer();
	if (ui != nullptr)
	ui->render();	
}

void Restaurant::initComponent() {
	Scene::initComponent();
	ui->initComponent();
}
void Restaurant::receive(const Message& m) {
	pantry->Scene::receive(m);
	_receive(m);
}
void Restaurant::_receive(const Message& m) {
	ui->receive(m);
	Scene::receive(m);
}
void Restaurant::update() {
	pantry->_update();
	_update();
}

void Restaurant::_update() {
	Scene::update();
	ui->update();
	cm->update();
	dm->checkDayFinished();
}

void Restaurant::handleEvents() {
	if (ih->isKeyDown(SDLK_p)) {
		GameManager::get()->pushScene((GameManager::get()->getScene(sc_PAUSEMENU)));
		restaurantMusic->pauseMusic();
	}
	else {
		Scene::handleEvents();
		ui->handleEvents();
	}
}

void Restaurant::refresh() {
	Scene::refresh();
	ui->refresh();
	pantry->Scene::refresh();
}