#pragma once
#include "../components/CookingMachineComp.h"
#include "../components/InventoryComp.h"
#include "../scenes/UIRestaurant.h"
#include "../components/Ingredients.h"
#include "../components/TriggerComp.h"
#include "../components/Transform.h"
#include "../components/Image.h"

#include "../structure/GameManager.h"
#include "../scenes/Restaurant.h"

/*gestiona la cocina cuando se colisiona con player*/
class CookingMachineTrigger :public TriggerComp
{
public:
	CookingMachineTrigger(GameObject* parent, Vector pos_, float width_, float height_) :
		TriggerComp(parent, pos_,width_,height_),
		cook(parent->getComponent<CookingMachineComp>()),
		inventory(GameManager::instance()->getRestaurant()->getUI()->getInventory()->getComponent<InventoryComp>()) ,
		p(parent->getScene()->getGameObject(_ecs::hdr_PLAYER1)->getComponent<Transform>()) , highlight(parent->getComponent<Image>()) {
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

