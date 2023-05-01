#include "ButtonComp.h"

#include "../Utilities/InputHandler.h"

#include "../Utilities/checkML.h"

ButtonComp::ButtonComp(GameObject* parent, string hl, std::function<void()> callback) : Component(parent, id),buttonSound( &sdlutils().soundEffects().at("CLICK_BUTTON")),
	buttonHoverSound(&sdlutils().soundEffects().at("SELECT")), transform(parent->getComponent<Transform>()), highlighted(false),selected(false)
{
	tf = parent->getComponent<Transform>();

	highlight = &sdlutils().images().at(hl);
	_callback = callback;
	dest.x = tf->getPos().getX();
	dest.y = tf->getPos().getY();
	dest.w = tf->getW();
	dest.h = tf->getH();

}

void ButtonComp::handleEvents()
{
	int x, y; // corrdenadas del puntero del ratón
	SDL_GetMouseState(&x, &y);
	SDL_Rect mouseRect = { x, y, 1, 1 };

	SDL_Rect dest = { transform->getPos().getX(), transform->getPos().getY(), transform->getW(), transform->getH() };

	 if (ih().joysticksInitialised()) {
		ih().refresh();
		/*if (!selected) buttonHoverSound->play();*/

		//buttonHoverSound->play();
		if (highlighted) {
			if (ih().getButtonState(0, SDL_CONTROLLER_BUTTON_B)) {
				//ih().clean();
				//ih().setControls(false);
				playSound();
				_callback();
			}
		}
	}
	else {
		 if (ih().isKeyDown(SDL_SCANCODE_SPACE)) {
			 //if (!highlighted) buttonHoverSound->play();
			 if (highlighted) {
				 playSound();
				 //ih().setControls(true);
				 _callback();
			 }
		 }
	}
	//else {
}

bool ButtonComp::isHighlighted() {
	return highlighted;
}

void ButtonComp::setHighlighted(bool isH) {
	highlighted = isH;
}

void ButtonComp::render() {
	if (highlighted) {
		highlight->render(dest);
	}
}


void ButtonComp::playSound() {
	buttonSound->play();
}
