#pragma once
#include "../Structure/GameObject.h"
#include "../Components/Transform.h"
#include "../Components/CashRegisterTrigger.h"
#include "../Components/Image.h"

using namespace std;
class CashRegister :public GameObject
{
private:
	const float OFFSETTRI_Y = 50;

public:
	CashRegister(Scene* scene, Vector pos, float w, float h) :GameObject(scene, _ecs::grp_INTERACTABLE) {
		// para saber en donde tiene q estar el jugador para poder cobrar
		new Transform(this, pos + Vector(0, OFFSETTRI_Y), Vector(0, 0), w, h, 0, true);
		// renderizar el highlight
		new Image(this, "CASH_HIGHLIGHT", { 0,-OFFSETTRI_Y });
		new CashRegisterTrigger(this,Vector(0, -OFFSETTRI_Y-20),w,h);
	};
};

