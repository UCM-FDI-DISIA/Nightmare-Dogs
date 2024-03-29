#pragma once
#include "../Structure/TriggerComp.h"
#include "../Structure/GameObject.h"

class Ingredients;
class KitchenTriggerComp : public TriggerComp {
private:
	Ingredients* ing;
	InputHandler* ih;
public:

	KitchenTriggerComp(GameObject* parent, Vector pos_, float width_, float height_) ;
	virtual void onTriggerExit();
	virtual void isOverlapping();
};