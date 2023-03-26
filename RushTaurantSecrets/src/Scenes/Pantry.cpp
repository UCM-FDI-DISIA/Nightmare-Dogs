#include "Pantry.h"
#include "Restaurant.h"
#include "../Structure/GameManager.h"
#include "../Components/MapCreator.h" 
#include "../GameObjects/Player.h"
#include "../Managers/ThiefsManager.h"
#include "../Utilities/checkML.h"

Pantry::Pantry() :rest(nullptr) { init(); }
Pantry::~Pantry() {
	delete collisionsManager;
}

void Pantry::reset() {
	player->getComponent<PlayerMovementController>()->initP();
}

void Pantry::init() {
	collisionsManager = new CollisionsManager(this);

	player = new Player(this, 0);
	// el update no se ejecuta hasta que se est?en la escena
	// por lo que no se crean ni se destruyen ladrones cuandon no se est?en la despensa
	GameObject* managerContainer = new GameObject(this);
	ThiefsManager::init(managerContainer, 2, 6, GameManager::get()->getHasKill(), 4 * 1000, 2);

	initRender();
	Scene::initComponent();
}

void Pantry::createMap() {
	Scene::createMap("assets/tilemaps/pantry.tmx", Down, Vector());
	Scene::createMap("assets/tilemaps/pantry_top_walls.tmx", Top, Vector());
	Scene::createMap("assets/tilemaps/pantry_top_bottom.tmx", Middle, Vector(0, 14 * 48 * RESIZEFACTOR));
	Scene::createMap("assets/tilemaps/pantry_top_middle.tmx", Middle, Vector(0, 11 * 48 * RESIZEFACTOR));
	Scene::createMap("assets/tilemaps/pantry_top_bottom_2.tmx", Middle, Vector(0, 16 * 48 * RESIZEFACTOR));
}


void Pantry::callAfterCreating() {
	rest = GameManager::get()->getRestaurant();
	createMap();
}

void Pantry::render() {
	Scene::renderLayer();
}

void Pantry::update() {
	rest->_update();
	Scene::update();
	collisionsManager->update();
}

void Pantry::_update() {
	Scene::update();
	//collisionsManager->update();
}



void Pantry::handleEvents() {
	if (ih->isKeyDown(SDLK_1)) {
		GameManager::get()->changeScene(GameManager::get()->getRestaurant());
	}
	else {
		Scene::handleEvents();
	}
}