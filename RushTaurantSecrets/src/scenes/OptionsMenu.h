#pragma once
#include "../structure/Scene.h"
#include "../sdlutils/Texture.h"
#include "../gameObjects/ButtonGO.h"

class OptionsMenu : public Scene
{
private:
	static void mMenu();
	static void bResume();

	GameObject* bg;
	ButtonGO* buttonResume;
	Texture* image;
public:
	OptionsMenu();
	~OptionsMenu();

	void handleEvents();

};

