#pragma once
#include "../Structure/GameObject.h"
#include "../Components/Transform.h"
#include "../Components/KitchenIslandTrigger.h"
#include "../Components/KitchenIslandComp.h"
#include "../Components/Image.h"

using namespace std;
class KitchenIsland :public GameObject
{
public:
	KitchenIsland(Scene* scene, Vector pos, float w, float h) :
		GameObject(scene, _ecs::grp_INTERACTABLE,_ecs::hdr_KITCHENISLAND) 
	{
		new Transform(this, pos, Vector(0, 0), w, h, 0, true); //para saber en que posicion renderizar el bocadillo 
		new KitchenIslandComp(this);
	};
};