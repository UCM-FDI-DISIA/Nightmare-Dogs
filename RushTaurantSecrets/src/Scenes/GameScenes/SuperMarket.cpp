#include "SuperMarket.h"
#include "../../Structure/GameManager.h"
#include "../../GameObjects/OtherPlayer.h"

#include "../../Utilities/checkML.h"

 void SuperMarket::initComponent() {
	 Scene::initComponent();
	 uiMarket->initComponent();
}
void SuperMarket::render() {
	Scene::renderLayer();
	if (uiMarket != nullptr)
		uiMarket->render();
}
void SuperMarket::update() {
	Scene::update();
	cm->update();
	uiMarket->update();
}
void SuperMarket::handleEvents() {
	if (ih->isKeyDown(SDLK_1)) {
		GameManager::get()->changeScene((Scene*)GameManager::get()->getRestaurant());
		supermarketMusic->pauseMusic();
		restaurantMusic->play();
	}
	else {
		Scene::handleEvents();
		uiMarket->handleEvents();
	}
}

void SuperMarket::init() {
	cm = new CollisionsManager(this);
	player = new Player(this, 0);
	new OtherPlayer(this);
}

void SuperMarket::callAfterCreating() {
	cartelM = new CartelManager(this);
	createMap();
	initRender();
	initComponent();
	player->getComponent<PlayerMovementController>()->initP();

}

void SuperMarket::createMap() {
	Scene::createMap("assets/tilemaps/supermarket.tmx", Down, Vector());
	Scene::createMap("assets/tilemaps/supermarket_top_walls.tmx", Top, Vector());
	Scene::createMap("assets/tilemaps/supermarket_top_bottom.tmx", Middle, Vector(0, 23*48 * RESIZEFACTOR));
	Scene::createMap("assets/tilemaps/supermarket_top_middle.tmx", Middle, Vector(0, 11*48 * RESIZEFACTOR));
}
SuperMarket::~SuperMarket() {
	delete uiMarket;
	delete cm;
	delete cartelM;
}

void SuperMarket::reset() {
	Transform* playerTransform = player->getComponent<Transform>();
	playerTransform->setPos(INITIAL_POS);
	playerTransform->setOrientation(west);

}

BasketMarket* SuperMarket::getBM() {
	return uiMarket->basketM();
}

void SuperMarket::receive(const Message& message) {
	Scene::receive(message);
	uiMarket->receive(message);
}