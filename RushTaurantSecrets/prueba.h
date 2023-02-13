#pragma once
#include "src/structure/GameObject.h"
#include "PruebaComponent.h"
#include "src/structure/Structure_def.h"
#include "src/sdlutils/SDLUtils.h"
class Scene;
class prueba:public GameObject
{
public:
	prueba(Scene* scene) :GameObject(scene) {
		//auto& sdl = *SDLUtils::instance();
		new PruebaComponent( &(sdl.images().at("prueba")), this,_ecs::cmp_TRANSFORM);
	};
	
};

