#pragma once

#include "../structure/Component.h"
#include "../components/StraightMovement.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../structure/Paths_def.h"
#include <vector>

enum Objective { Secret, Freezer };

class ThiefMovement : public Component {
public:
	enum States { OBJECTIVE, FREEZER, SECRET, DEAD, ESCAPE };

private:
	States currentState;
	StraightMovement* straightMovement;
	Transform* transform;
	SDLUtils* sdl;
	Objective objective;
	float elapsedTime;
	float deadTime;
	float escapeSpeed;

	void addPath(const vector<Vector>& points);

public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_MOVEMENT;

	ThiefMovement(GameObject* parent, Objective objective, int pos, float escapeSpeed);

	// se tendr�a que llamar al pulsar el bot�n de matar
	void die();

	// se tendr�a que llamar al pulsar el bot�n de escapar
	void escape();

	States getState() const {
		return currentState;
	}

	virtual void update();

};