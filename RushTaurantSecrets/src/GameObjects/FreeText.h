#pragma once

#include "../Structure/GameObject.h"
#include "../Structure/Scene.h"
#include "../Components/Transform.h"
#include "../Components/Text.h"

class FreeText : public GameObject {
private:

public:
	FreeText(Scene* scene, Vector pos, int widthLetter, int heightLetter, int widthTextBox, float frequency, Font* font, deque<string> texts)
		: GameObject(scene, _ecs::grp_HUD) {
		new Transform(this, pos);
		new Text(this, texts, widthLetter, heightLetter, frequency, font, widthTextBox);
	}
};