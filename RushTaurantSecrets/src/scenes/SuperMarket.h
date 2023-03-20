#pragma once
#include "../gameObjects/Player.h"
#include "../structure/CollisionsManager.h"
#include "../structure/Scene.h"
#include "../structure/CartelManager.h"
#include "../components/MapCreator.h"
#include "../scenes/UIMarket.h"

class SuperMarket: public Scene {
private:
	const float RESIZEFACTOR = 0.6666666667;

	CollisionsManager* cm;
	GameObject* map;
	GameObject* mapTop;
	UIMarket* uiMarket;
	CartelManager* cartelM;
	Player* player;
public:
	SuperMarket(UIMarket* marketUI = nullptr) : uiMarket(marketUI == nullptr ? new UIMarket(this) : marketUI),
		cm(nullptr), map(nullptr), mapTop(nullptr), cartelM(nullptr), player(nullptr) { init(); }
	~SuperMarket();
	void reset();
	
	void callAfterCreating();

	virtual void initComponent();
	void render();
	void update();
	void handleEvents();
	void init();
	UIMarket* getUI() { return uiMarket; }
	CartelManager* getCartelManager() { return cartelM; }

	float getResizeFactor() { return RESIZEFACTOR; }
	void createMap();

};