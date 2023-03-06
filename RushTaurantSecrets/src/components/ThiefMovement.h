#pragma once

#include "../structure/Component.h"
#include "../components/StraightMovement.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../structure/Paths_def.h"
#include <vector>

class ThiefMovement : public Component {
public:
	enum States { OBJECTIVE, FREEZER, SECRET, DEAD, ESCAPE };

private:
	enum Objective {Freezer, Secret};

	States currentState;
	StraightMovement* straightMovement;
	Transform* transform;
	SDLUtils* sdl;
	bool canGetFridger;
	Objective objective;
	InputHandler* inputHandler;
	float firstTick;
	float deadTime;
	float escapeSpeed;

public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_MOVEMENT;

	ThiefMovement(GameObject* parent, bool canGetFridger, float escapeSpeed);

	// se tendr�a que llamar al pulsar el bot�n de matar
	void die();

	// se tendr�a que llamar al pulsar el bot�n de escapar
	void escape();

	virtual void update();

	// matar o hacer que huya el ladr�n
	virtual void handleEvents();

	const States getState();
};