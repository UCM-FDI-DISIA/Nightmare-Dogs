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
	Scene::handleEvents();
	uiMarket->handleEvents();
	
	if (ih->isKeyDown(SDLK_p)) {
		GameManager::get()->pushScene((GameManager::get()->getScene(sc_PAUSEMENU)));
		//->pauseMusic();
	}
}

void SuperMarket::init() {
	cm = new CollisionsManager(this);
	player = new Player(this, 0);
	new OtherPlayer(this, 2);
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

void SuperMarket::receive(const Message& message) {

	if (message.id == Message::msg_TO_RESTAURANT) {
		vector<pair<_ecs::_ingredients_id, int>> ing;
		ing = GameManager::get()->getSupermarket()->getUI()->getGameObject(hdr_SM_INVENTORY)->getComponent<BasketMarketComponent>()->getIngredients();
		GameManager::get()->setIngredients(ing);
		GameManager::get()->changeScene(GameManager::get()->getScene(_ecs::sc_RESTAURANT));
		GameManager::get()->getScene(_ecs::sc_RESTAURANT)->reset();
		restaurantMusic->play();
	}
	Scene::receive(message);
	uiMarket->receive(message);
}