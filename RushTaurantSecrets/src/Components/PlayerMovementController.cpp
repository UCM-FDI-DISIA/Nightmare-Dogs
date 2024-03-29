#include "PlayerMovementController.h"

#include "../Structure/Game.h"

#include "../Utilities/checkML.h"

PlayerMovementController::PlayerMovementController(GameObject* parent, int _player) : Component(parent, id), player(_player), time(0) {
	transform = parent->getComponent<Transform>();
	input = InputHandler::instance();
}

PlayerMovementController::~PlayerMovementController() {
	input->clean();
}

void PlayerMovementController::handleEvents() {
	if (!keyboard)
	{
		//input->refresh();
		// eje x mando 1
		if ((input->xvalue(0, 1) > 0 || input->xvalue(0, 1) < 0) && !input->keyDownEvent()) {
			speed.setX(offset * input->xvalue(0, 1));
			transform->setMovState(walking);
			if (input->xvalue(0, 1) < 0)
				transform->setOrientation(west);
			else
				transform->setOrientation(east);
		}
		// eje y mando 1
		else if (input->yvalue(0, 1) > 0 || input->yvalue(0, 1) < 0 && !input->keyDownEvent()) {
			speed.setY(offset * input->yvalue(0, 1));
			transform->setMovState(walking);
			if (input->yvalue(0, 1) < 0)
				transform->setOrientation(north);
			else
				transform->setOrientation(south);
		}
		else if (input->xvalue(0, 1) == 0 && input->yvalue(0, 1) == 0) {
			transform->setMovState(idle);
		}
		if (xbox) {
			if (input->getHatEvent()) {
				// derecha
				if (input->getHatState(RIGHT)) {
					moveRight();
				}
				// izquierda
				if (input->getHatState(LEFT)) {
					moveLeft();
				}
				// arriba		
				if (input->getHatState(UP)) {
					moveUp();
				}
				// abajo
				if (input->getHatState(DOWN)) {
					moveDown();
				}
			}
			else // eliminar
				input->setFalseJoyhat();
		}
		else { // PS4 Controller
			// derecha
			if (input->getButtonState(0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
				transform->setMovState(walking);
				speed.setX(offset);
				transform->setOrientation(east);
			}
			// izquierda
			else if (input->getButtonState(0, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
				transform->setMovState(walking);
				speed.setX(-offset);
				transform->setOrientation(west);
			}
			// arriba		
			else if (input->getButtonState(0, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
				transform->setMovState(walking);
				speed.setY(-offset);
				transform->setOrientation(north);
			}
			// abajo
			else if (input->getButtonState(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
				transform->setMovState(walking);
				speed.setY(offset);
				transform->setOrientation(south);
			}
		}
	}
	else if (input->keyDownEvent()) {
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		// izquierda
		if (currentKeyStates[SDL_SCANCODE_A]) {
			moveLeft();
		}
		// derecha
		if (currentKeyStates[SDL_SCANCODE_D]) {
			moveRight();
		}
		// arriba
		if (currentKeyStates[SDL_SCANCODE_W]) {
			moveUp();
		}
		// abajo
		if (currentKeyStates[SDL_SCANCODE_S]) {
			moveDown();
		}
	}
}

void PlayerMovementController::update() {
	if (!nonKeyPressed() && input->keyUpEvent()) {
		speed = Vector(0, 0);
		transform->setMovState(idle);
	}
	transform->setVel(speed);
	if (!keyboard) {
		speed = Vector(0, 0);
	}
}

bool PlayerMovementController::nonKeyPressed() {
	bool pressed = false;
	if (keyboard) {
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_D])
		{
			moveRight();
			pressed = true;
		}
		if (currentKeyStates[SDL_SCANCODE_A])
		{
			moveLeft();
			pressed = true;
		}	
		if (currentKeyStates[SDL_SCANCODE_S])
		{
			moveDown();
			pressed = true;
		}
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			moveUp();
			pressed = true;
		}		
		return pressed;
	}
	if (!keyboard) {
		if (xbox) {
			if (input->getHatEvent()) {
				// abajo
				if (input->getHatState(DOWN)) {
					moveDown();
					pressed = true;
				}
				// arriba		
				if (input->getHatState(UP)) {
					moveUp();
					pressed = true;
				}
				// izquierda
				if (input->getHatState(LEFT)) {
					moveLeft();
					pressed = true;
				}
				// derecha
				if (input->getHatState(RIGHT)) {
					moveRight();
					pressed = true;
				}
			}
			else // eliminar
				input->setFalseJoyhat();
			return pressed;
		}
	}
	else
		return true;
}

void PlayerMovementController::initP() {
	keyboard = input->getControls();
	if (!keyboard) {
		if (!input->joysticksInitialised())
			input->initialiseJoysticks();
		_joy = input->getPlayerController(player);
		controller = SDL_JoystickName(_joy);
		if (std::string(controller) == "Controller (Xbox One For Windows)") 
			xbox = true;
		else 
			xbox = false;
		input->setXBox(xbox);
	}
}

void PlayerMovementController::moveUp() {
	speed = Vector(0, 0);
	speed.setY(-offset);
	transform->setOrientation(north);
	transform->setMovState(walking);
}

void PlayerMovementController::moveDown() {
	speed = Vector(0, 0);
	speed.setY(offset);
	transform->setOrientation(south);
	transform->setMovState(walking);
}

void PlayerMovementController::moveRight() {
	speed = Vector(0, 0);
	speed.setX(offset);
	transform->setOrientation(east);
	transform->setMovState(walking);
}

void PlayerMovementController::moveLeft() {
	speed = Vector(0, 0);
	speed.setX(-offset);
	transform->setOrientation(west);
	transform->setMovState(walking);
}

const char* PlayerMovementController::getControllerType()
{
	return controller;
}

 bool PlayerMovementController::isXbox()
{
	return xbox;
}
