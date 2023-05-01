#pragma once
#include "../../GameObjects/Player.h"
#include "../../Managers/CollisionsManager.h"
#include "../../Structure/Scene.h"
#include "../../Managers/CartelManager.h"
#include "../../Components/MapCreator.h"
#include "../../Scenes/HUD/UIMarket.h"

class SuperMarket: public Scene {
private:
	const int MUSIC_VOL = 30;

	const float RESIZEFACTOR = 0.6666666667;
	const Vector INITIAL_POS = { 1136, 165 };

	CollisionsManager* cm;
	GameObject* map;
	GameObject* mapTop;
	UIMarket* uiMarket;
	CartelManager* cartelM;
	Player* player;

	SDLUtils* sdl;
	Music* restaurantMusic;
	Music* supermarketMusic;
public:
	SuperMarket(UIMarket* marketUI = nullptr) : uiMarket(marketUI == nullptr ? new UIMarket(this) : marketUI),
		cm(nullptr), map(nullptr), mapTop(nullptr), cartelM(nullptr), player(nullptr), 
		sdl(SDLUtils::instance()), restaurantMusic(&sdl->musics().at("RESTAURANT_MUSIC")), supermarketMusic(&sdl->musics().at("SUPERMARKET_MUSIC"))
	{ 
		init();
		restaurantMusic->setMusicVolume(MUSIC_VOL);
		supermarketMusic->setMusicVolume(MUSIC_VOL);
	}
	~SuperMarket();
	
	void callAfterCreating();

	virtual void initComponent();
	void render();
	void update();
	void handleEvents();
	void init();
	void reset();
	void receive(const Message& message);

	UIMarket* getUI() { return uiMarket; }
	CartelManager* getCartelManager() { return cartelM; }

	float getResizeFactor() { return RESIZEFACTOR; }
	void createMap();

	BasketMarket* getBM();
	ButtonGO* buyButton();
};