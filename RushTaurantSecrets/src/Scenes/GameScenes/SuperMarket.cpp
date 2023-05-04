#include "SuperMarket.h"
#include "../../Structure/GameManager.h"
#include "../../GameObjects/OtherPlayer.h"
#include "../../GameObjects/ClientMarket.h"
#include "../../Utilities/checkML.h"
#include "../../Components/Route1.h"
#include "../../GameObjects/DancingClient.h"
#include "../../Components/Route2.h"

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
	Scene::handleEvents();
	uiMarket->handleEvents();
	
	if (ih->isKeyDown(SDLK_ESCAPE) && active) {
		GameManager::get()->pushScene((GameManager::get()->getScene(sc_PAUSEMENU)));
		restaurantMusic->pauseMusic();
	}
}

void SuperMarket::init() {
	cm = new CollisionsManager(this);
	player = new Player(this, 0);
	new OtherPlayer(this, 2);

	ClientMarket* clientMarket = new ClientMarket(this);
	new Route1(clientMarket);

	for (int i = 0; i < 3; ++i) {
		DancingClient* dancingClient = new DancingClient(this);
		new Route2(dancingClient, Vector(10 + i, 14));
	}
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

ButtonGO* SuperMarket::buyButton() {
	return uiMarket->BuyButton();
}

void SuperMarket::resumeMusic() {
	supermarketMusic->resumeMusic();
}
void SuperMarket::receive(const Message& message) {
	Scene::receive(message);
	uiMarket->receive(message);
}


void SuperMarket::initCoopMode(bool server) {
	active = false;
	if (!server) {
		getGameObject(_ecs::hdr_PLAYER)->getComponent<CharacterAnimator>()->setTexture("Player_2",18,18);
		getGameObject(_ecs::hdr_OTHERPLAYER)->getComponent<CharacterAnimator>()->setTexture("Player_1", 18, 18);
	}
}
void SuperMarket::quitCoopMode() {
	active = true;
	getGameObject(_ecs::hdr_PLAYER)->getComponent<CharacterAnimator>()->setTexture("Player_1", 18, 18);
	getGameObject(_ecs::hdr_OTHERPLAYER)->getComponent<CharacterAnimator>()->setTexture("Player_2", 18, 18);
}