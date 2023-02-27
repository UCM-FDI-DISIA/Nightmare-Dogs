#pragma once
#include "../utils/Singleton.h"
#include <list>
#include "Scene.h"
#include "../scenes/MainMenu.h"
#include "../scenes/Pantry.h"
#include "../scenes/Restaurant.h"
#include "../scenes/UIRestaurant.h"
#include "../scenes/SuperMarket.h"
#include "../scenes/UIMarket.h"
#include "../scenes/DailyMenuScene.h"
#include "../scenes/PauseMenu.h"


using namespace std;
class SceneManager:public Singleton<SceneManager> {
	friend Singleton<SceneManager>;
public:
	const float PANTRYSIZE = 83/99;
	const float GENERALSIZE = 2/3;
	enum SceneName{MAINMENU,RESTAURANT,PANTRY,DAILYMENU,SUPERMARKET,PAUSEMENU};
private:
	//crea menu
	SceneManager();
	//lista de escenas que vamos a usar
	list<Scene*> Scenes;
	//escenas que estar activa (jugador)
	Scene* currentScene;
	SceneName act;
	SceneName change;

public:
	SceneManager(SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	~SceneManager() { clear(); }
	void update();
	void handleEvents();
	void render();
	void refresh();
	void ChangeScene(SceneName scene);

private:
	void setScene();
	void clear();
};