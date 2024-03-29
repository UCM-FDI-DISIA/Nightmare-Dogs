#pragma once
#include "CookingMachineComp.h"
#include "InventoryComp.h"
#include "../Scenes/HUD/UIRestaurant.h"
#include "Ingredients.h"
#include "../Structure/TriggerComp.h"
#include "Transform.h"
#include "Image.h"

#include "../Structure/GameManager.h"
#include "../Scenes/GameScenes/Restaurant.h"

/*gestiona la cocina cuando se colisiona con player*/
class CookingMachineTrigger :public TriggerComp
{
public:
	CookingMachineTrigger(GameObject* parent, Vector pos_, float width_, float height_) :
		TriggerComp(parent, pos_,width_,height_),
		cook(parent->getComponent<CookingMachineComp>()),
		inventory(GameManager::get()->getRestaurant()->getUI()->getInventory()->getComponent<InventoryComp>()) ,
		p(parent->getScene()->getGameObject(_ecs::hdr_PLAYER)->getComponent<Transform>()) , highlight(parent->getComponent<Image>()) {
		highlight->setActive(false);
	};
	
	virtual void isOverlapping();
	virtual void onTriggerExit();
private:
	InventoryComp* inventory;
	CookingMachineComp* cook;
	Transform* p;
	Image* highlight;

};

