#include "MainMenu.h"

#include "../Structure/Scene.h"
#include "../GameObjects/ButtonGO.h"
#include "../Utilities/Texture.h"

#include "../Structure/GameManager.h"
#include "Restaurant.h"
#include "DailyMenuScene.h"
#include "OptionsMenu.h"
#include "ContinueMenu.h"
#include "CoopMenu.h"
#include "../Components/Transform.h"
#include "../Components/Image.h"
#include "BeforeDayStartScene.h"
#include "IntroScene.h"
//#include "FirstDayAfterKillScene.h"

#include "../Utilities/checkML.h"

void MainMenu::start() {
	if (GameManager::get()->checkload()) {

		GameManager::get()->changeScene(GameManager::get()->getContinueMenu());
		GameManager::get()->getBeforeDayStart()->reset();
		GameManager::get()->getDailyMenu()->reset();
	}
	else {
		GameManager::get()->changeScene(GameManager::get()->getIntroScene());
		GameManager::get()->getCurrentScene()->callAfterCreating();
	}
}

void MainMenu::start2() {
	GameManager::get()->changeScene(GameManager::get()->getCoopMenu());
}

void MainMenu::opt() {
	GameManager::get()->changeScene(GameManager::get()->getOptionsMenu());
}

MainMenu::MainMenu() : sdl(SDLUtils::instance()), supermarketMusic(&sdl->musics().at("SUPERMARKET_MUSIC")) {
	bg = new GameObject(this);
	new Transform(bg, { 0,0 }, { 0,0 }, sdlutils().width(), sdlutils().height());
	image = &(sdlutils().images().at("MAIN_MENU_BG"));
	new Image(bg, image);

	oneplayer = new ButtonGO(this, "1_PLAYER_BUTTON", "BUTTON2_HIGHLIGHT", Vector(BUTTONS_X, BUTTONS_Y), BUTTONS_W, BUTTONS_H, start);
	twoplayer = new ButtonGO(this, "2_PLAYER_BUTTON", "BUTTON2_HIGHLIGHT", Vector(BUTTONS_X, BUTTONS_Y + BUTTONS_H), BUTTONS_W, BUTTONS_H, start2);
	options = new ButtonGO(this, "OPTIONS_BUTTON", "BUTTON2_HIGHLIGHT", Vector(BUTTONS_X, BUTTONS_Y + 2 * BUTTONS_H), BUTTONS_W, BUTTONS_H, opt);
	supermarketMusic->setMusicVolume(MUSIC_VOL);
}

MainMenu::~MainMenu() { }

void MainMenu::handleEvents() {
	if (ih->isKeyDown(SDLK_1)) {
		GameManager::get()->changeScene((Scene*)GameManager::get()->getDailyMenu());
	}
	else if (ih->isKeyDown(SDLK_2)) {
		GameManager::get()->changeScene((Scene*)GameManager::get()->getSupermarket());
		supermarketMusic->play();
	}
	else {
		Scene::handleEvents();
	}
}