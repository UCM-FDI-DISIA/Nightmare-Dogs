#pragma once

#include "../structure/Component.h"
#include "../sdlutils/Texture.h"
#include "../components/Image.h"
#include "../sdlutils/SDLUtils.h"

class SceneManager;
class Scene;

using namespace std;

class Button : public Component {
const int BUTTON_W = 96, BUTTON_H = 32;
public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_BUTTON;

	Button(GameObject* parent, SceneManager* sceneManager, void (*callback)(SceneManager* sceneManager));
	Button(GameObject* parent, Texture* t, SDL_Rect d, SceneManager* sceneManager, void (*callback)(SceneManager* sceneManager));
	Button(GameObject* parent, Texture* t, int x, int y, SceneManager* sceneManager, void (*callback)(SceneManager* sceneManager));
	Button(GameObject* parent, string s, int x, int y, SceneManager* sceneManager, void (*callback)(SceneManager* sceneManager));
	~Button();
	void render();
	void handleEvents();

private:
	void (*callback)(SceneManager* sceneManager);

	int frame = 0;

	Texture* texture;

	SDL_Rect src, dest;

	SceneManager* sceneManager;

	SDL_Joystick* _joy;

	SDLUtils* sdl;
};
