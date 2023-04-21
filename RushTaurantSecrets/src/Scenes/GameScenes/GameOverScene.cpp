#include "GameOverScene.h"
#include "../../Structure/GameManager.h"
#include "../Menus/MainMenu.h"
#include "../../Utilities/checkML.h"

void GameOverScene::mMenu() {
	GameManager::get()->changeScene(GameManager::get()->getMainMenu());
}

GameOverScene::GameOverScene() {
	bg = new GameObject(this);
	new Transform(bg, { 0,0 }, { 0,0 }, sdlutils().width(), sdlutils().height());
	image = new Texture(sdlutils().renderer(), "assets/pauseMenuTemp.png");
	new Image(bg, image);


	buttonMainMenu = new ButtonGO(this, "MAINM_BUTTON_UP", "BUTTON2_HIGHLIGHT",
		Vector((SDLUtils::instance()->width() / 2) - (192 * 2 / 2), 2.8 * SDLUtils::instance()->height() / 5), 385, 130, mMenu);
}

GameOverScene::~GameOverScene() {
	delete image;
}


void GameOverScene::setGameOver(endingType type) {
	switch (type)
	{
	case _ecs::BadRep:
		break;
	case _ecs::Broke:
		break;
	case _ecs::Caught:
		break;
	case _ecs::Robbed:
		break;
	case _ecs::Normal:
		break;
	case _ecs::Happy:
		break;
	default:
		break;
	}
}