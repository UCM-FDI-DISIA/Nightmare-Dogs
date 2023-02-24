#pragma once
#include "../structure/Scene.h"
#include "../components/Button.h"
#include "../sdlutils/Texture.h"
class MainMenu : public Scene {
private:
	static void start(SceneManager* sceneManager);

	GameObject* bg;
	GameObject* button;
	Texture* image;

public:
	MainMenu(SceneManager* sceneManager);
	~MainMenu();

	void handleEvents()override;
};