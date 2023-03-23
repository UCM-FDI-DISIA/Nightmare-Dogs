#pragma once
#include "../Structure/Scene.h"
#include "UIRestaurant.h"
#include "Pantry.h"

class DishCombinator;
class CollisionsManager;
class TextBox;
class DayManager;

class Restaurant : public Scene {
private:
	const float RESIZEFACTOR = 0.6666666667;
	Pantry* pantry;
	DishCombinator* dc;
	UIRestaurant* ui;
	CollisionsManager* cm;
	DayManager* dm;
	
	GameObject* player;

	vector<_ecs::_dish_id> menu() const;
	//vector<_ecs::_dish_id> dailyMenu;

public:
	Restaurant();
	~Restaurant();

	void reset();

	void callAfterCreating();

	UIRestaurant* getUI() { return ui; }
	Pantry* getPantry() { return pantry; }
	void linkPantry(Pantry* pantry);
	void render();
	void update();
	void handleEvents();
	void refresh();
	virtual void initComponent();

	float getResizeFactor() { return RESIZEFACTOR; }
	void createMap();
};