#include "Pantry.h"
#include "Restaurant.h"
#include "EndOfDayScene.h"
#include "../../Structure/GameManager.h"
#include "../../Components/MapCreator.h" 
#include "../../GameObjects/Player.h"
#include "../../Managers/ThievesManager.h"
#include "../../GameObjects/TimeOfDayObj.h"
#include "EndOfDayScene.h"
#include "../../GameObjects/OtherPlayer.h"
#include "../../Utilities/checkML.h"

Pantry::Pantry(UIPantry* pUI) : rest(nullptr), pantryUI(pUI), sdl(SDLUtils::instance()), restaurantMusic(&sdl->musics().at("RESTAURANT_MUSIC")), 
pantryMusic(&sdl->musics().at("PANTRY_MUSIC"))
{ 
	init();
	pantryMusic->setMusicVolume(MUSIC_VOL);
	restaurantMusic->setMusicVolume(MUSIC_VOL);
}

Pantry::~Pantry() {
	delete collisionsManager;
	delete pantryUI;
}


void Pantry::init() {
	collisionsManager = new CollisionsManager(this);

	player = new Player(this, 0);
	new OtherPlayer(this, 1);
	new TimeOfDayObj(this, { 0,100 }, sdlutils().getLoadedTilesets().at("pantryAfternoon"), sdlutils().getLoadedTilesets().at("pantryNight"));
	// el update no se ejecuta hasta que se est?en la escena
	// por lo que no se crean ni se destruyen ladrones cuandon no se est?en la despensa
	GameObject* managerContainer = new GameObject(this);
	ThievesManager::init(managerContainer, 2, 6, GameManager::get()->getHasKill(), 2, 20, 30);

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
	initRender();
	player->getComponent<PlayerMovementController>()->initP();

}

void Pantry::render() {
	Scene::renderLayer();
	pantryUI->render();
}

void Pantry::update() {
	rest->_update();
	collisionsManager->update();
	_update();
}

void Pantry::_update() {
	Scene::update();
}


void Pantry::handleEvents() {
	if (ih->isKeyDown(SDLK_p)) {
		GameManager::get()->pushScene((GameManager::get()->getScene(sc_PAUSEMENU)));
		restaurantMusic->pauseMusic();
	}
}

void Pantry::reset() {
	getGameObject(_ecs::hdr_PLAYER)->setActives(false);
}

void Pantry::refresh() {
	Scene::refresh();
	rest->Scene::refresh();
}

void Pantry::receive(const Message& m) {
	rest->_receive(m);
	Scene::receive(m);
}