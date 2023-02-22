#include "TriggerComp.h"
#include "Transform.h"
#include "../structure/GameObject.h"
//#include "../structure/CollisionsManager.h"

#include "../utils/checkML.h"

using namespace std;
TriggerComp::TriggerComp(GameObject* parent, Vector pos_, float width_, float height_) :Component(parent, id), ih(InputHandler::instance())
{
	transform_ = parent->getComponent<Transform>();
	other_ = nullptr;
	overlap_ = false;
	pos = pos_;
	width = width_;
	height = height_;
}

void TriggerComp::Overlap(GameObject* other) {
	if (other == nullptr && overlap_ == true) {
		onTriggerExit();
		overlap_ = false;
		other_ = nullptr;
	}
	else if (other != nullptr && overlap_ == false) {
		onTriggerEnter();
		other_ = other;
		overlap_ = true;
	}
	else if (other != nullptr && overlap_ == true)
		isOverlapping();
}
SDL_FRect TriggerComp::getRect() {
	return { pos.getX() + transform_->getPos().getX(), pos.getY() + transform_->getPos().getY(), width, height };
};