#pragma once
#include "../structure/GameObject.h"
#include "../components/CharacterAnimator.h"
#include "../components/StraightMovement.h"
#include "../components/ClientState.h"
#include "../structure/Food_def.h"
#include "../components/ClientMovement.h"
#include "../components/ClientTrigger.h"
#include "../components/UIAnimator.h"
#include "../components/ClientStateRender.h"
#include "../components/HeartRender.h"

class Client : public GameObject {
	const int THINKING_WIDTH = 48;
	const int THINKING_HEIGHT = 48;
	const int THINKING_OFFSETY = -48;

public:

	Client(Scene* scene, string sprite, Vector origin, vector<_ecs::_dish_id> menu, int posEntrance, float speed, int posGroup) 
		: GameObject(scene, _ecs::grp_CLIENTS) {
		Transform* t = new Transform(this, Vector(origin), Vector(0, 0), 48, 96);
		new CharacterAnimator(this, sprite, 18, 24, 1,40);
		new UIAnimator(this, "CLIENT_THINKING_ICON_ANIM", 0, 8, 0, 80, THINKING_WIDTH, THINKING_HEIGHT, Vector(origin.getX(), origin.getY() + THINKING_OFFSETY));
		new StraightMovement(this, speed);
		ClientMovement* mov = new ClientMovement(this, posEntrance, posGroup);
		new ClientStateRender(this);
		new ClientState(this, menu);
		// se guarda en clientMovement la referencia a ClientState
		mov->initState();
		new ClientTrigger(this, Vector(0, 0), 48, 96);
		new HeartRender(this, &sdlutils().images().at("Heart"), 15, 15, 20, -7);
	}
};