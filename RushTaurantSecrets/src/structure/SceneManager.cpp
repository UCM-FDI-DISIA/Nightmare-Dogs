#include "SceneManager.h"
SceneManager::SceneManager() {
	currentScene = new MainMenu(this);
	pushScene(currentScene);
	nSceceToPop = 0;
}

void SceneManager::update() {
	if (currentScene != Scenes.back()) {
		setScene();
	}
	currentScene->update();
}

void SceneManager::render() {
	currentScene->render();
}

void SceneManager::handleEvents() {
	currentScene->handleEvents();
}

void SceneManager::refresh() {
	currentScene->refresh();
}
void SceneManager::pushScene(Scene* scene) {
	Scenes.push_back(scene);
}

void SceneManager::popScene(int n) {
	for (int i = 0; i < n; ++i) {
		delete Scenes.back();
		Scenes.pop_back();
	}
}
void SceneManager::changeScene(Scene* _scene, int nPop) {
	nSceceToPop = nPop;
	currentScene = _scene;
}

void SceneManager::clear() {
	currentScene = nullptr;
	for (auto scene : Scenes) {
		delete scene;
	}
	Scenes.clear();
}

void SceneManager::setResize(bool type) {
	if (type) {
		sdlutils().setResizeFactor(GENERALSIZE);
	}
	else {
		sdlutils().setResizeFactor(PANTRYSIZE);
	}
	
}

// cleon: no pong�is esto con un switch, sino con polimorfismo dentro de cada escena. 'SceneManager' no puede saber qu� escenas hay.
void SceneManager::setScene() {
	if (nSceceToPop != -1) {
		popScene(nSceceToPop);
	}
	else {  //caso de que no queremos destruir la escena
		Scenes.pop_back();
	}
	if (currentScene != nullptr) Scenes.push_back(currentScene);
	//Scene* aux;
	//switch(change) {
	//case SceneManager::MAINMENU:
	//	/*aux = *Scenes.begin();
	//	Scenes.clear();
	//	Scenes.push_back(aux);*/
	//	clear();
	//	Scenes.push_back(new MainMenu(this));
	//	break;
	//case SceneManager::RESTAURANT: {
	//	sdlutils().setResizeFactor(GENERALSIZE);
	//	if (act == MAINMENU) {
	//		//creacion de restaurant y pantry
	//		UIRestaurant* uiRest = new UIRestaurant();
	//		Restaurant* rest = new Restaurant(uiRest);
	//		sdlutils().setResizeFactor(PANTRYSIZE);
	//		Pantry* pantry = new Pantry();
	//		rest->linkPantry(pantry);
	//		pantry->linkRestaurant(rest);
	//		Scenes.push_back(rest);
	//		sdlutils().setResizeFactor(GENERALSIZE);
	//	}
	//	else if (act == PANTRY) {
	//		Scenes.pop_back();
	//	}
	//	else if (act == SUPERMARKET) {
	//		/*sdlutils().setResizeFactor(GENERALSIZE);
	//		currentScene = new SuperMarket();
	//		Scenes.push_back(currentScene);*/

	//	}
	//	else if (act == PAUSEMENU) {
	//		Scene* aux = Scenes.back();
	//		Scenes.pop_back();
	//		delete aux;
	//	}
	//	}break;
	//case SceneManager::PANTRY: {
	//	sdlutils().setResizeFactor(PANTRYSIZE);
	//	if (act == RESTAURANT) {			
	//		Scene* aux = Scenes.back();
	//		Scenes.pop_back();
	//		Scenes.push_back(static_cast<Restaurant*>(aux)->getPantry());
	//	}
	//	}break;
	//case SceneManager::DAILYMENU: {
	//	if (act == MAINMENU) {
	//		DailyMenuScene* dailyMenuS = new DailyMenuScene(this);
	//		currentScene = dailyMenuS;
	//		Scenes.push_back(currentScene);
	//	}
	//	break;
	//}
	//case SceneManager::SUPERMARKET:
	//	sdlutils().setResizeFactor(GENERALSIZE); // cleon =D -> Areglado
	//	if (act == MAINMENU || act == DAILYMENU) {
	//		UIMarket* uiMarket = new UIMarket();
	//		SuperMarket* superMarket = new SuperMarket(uiMarket);
	//		currentScene = superMarket;
	//		Scenes.push_back(currentScene);
	//	}
	//	break;
	//case SceneManager::PAUSEMENU:
	//	if (act == RESTAURANT || act == PANTRY) {
	//		PauseMenu* pauseMenu = new PauseMenu(this);
	//		currentScene = pauseMenu;
	//		Scenes.push_back(currentScene);
	//	}		
	//	break;
	//default:
	//	break;
	//}
}