#pragma once
#include "TriggerComp.h"
#include "../components/InventoryComp.h"
#include "../scenes/UIRestaurant.h"
#include "../components/Ingredients.h"
#include "../components/TriggerComp.h"
#include "../components/ClientState.h"
#include "../structure/Paths_def.h"

#include "../structure/GameManager.h"
#include "../scenes/Restaurant.h"

class ClientTrigger : public TriggerComp
{
	ClientState* clientState;
	InventoryComp* inventory;
	bool selected1;
public:
	ClientTrigger(GameObject* parent, Vector pos_, float width_, float height_) :
		TriggerComp(parent, pos_, width_, height_), selected1(false),
		//clientManager(ClientsManager::get()),
		clientState(parent->getComponent<ClientState>()),
		inventory(GameManager::instance()->getRestaurant()->getUI()->getInventory()->getComponent<InventoryComp>()) { };

	virtual void isOverlapping();

	int randomTable();

	bool inputMando();

	bool isSelected() { return selected1; }
};

