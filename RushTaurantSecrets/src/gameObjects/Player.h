#pragma once
#include "../structure/GameObject.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Animator.h"
#include "../sdlutils/Texture.h"
#include "../components/Ingredients.h"

class Player :public GameObject
{
public:
	Player(Scene * scene) :GameObject(scene, _ecs::grp_PLAYER, _ecs::hdr_PLAYER) {
	//new Ingredients(this);

	//auto& sdl = *SDLUtils::instance();
	//new CookingMachineComp(this);
	//new Animator(this, &(sdl.images().at("Player_1")), 6, 2);
}
};

