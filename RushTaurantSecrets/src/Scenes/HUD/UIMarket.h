#pragma once
#include "../../Structure/Scene.h"
#include "../../Utilities/SDLUtils.h"
#include "../../Structure/GameObject.h"
#include "../../Components/Transform.h"
#include "../../Components/Image.h"
#include "../../Managers/Money.h"
#include "../../GameObjects/BasketMarket.h"
#include "../../GameObjects/ButtonGO.h"
#include "../../GameObjects/DailyMenu.h"
#include "../../Components/ShowControlAuto.h"

// UI del supermercado
class UIMarket : public Scene
{
private:
	const string FONT_PATH = "assets/Fonts/chary___.ttf";
	const int FONTSIZE = 50, ICONSIZE = 48, ICONX = 20, ICONY = 15, CENTEROUTLINE = 4;
	const int TARGETOFFSET = 60;

	const float BUTTONS_X = 160, BUTTONS_Y = 550,
		BUTTONS_W = 96 * 3.2, BUTTONS_H = 32 * 3.2;

	Scene* market;
	SDLUtils* sdl = SDLUtils::instance();
	Font* font;
	Font* font1;
	Font* outline;
	Font* outline1;

	Money* moneyTxt;
	Texture* moneyTexture;
	Texture* moneyOutline;
	SDL_Rect moneyRect;
	SDL_Rect moneyOutlineRect;
	int intMoney;

	int target;
	Texture* targetTexture;
	Texture* targetOutline;
	string targetText;
	SDL_Rect targetRect;
	SDL_Rect targetOutlineRect;


	InputHandler* ih;
	BasketMarket* basketMarket;
	GameObject* shopMenu;
	DailyMenu* menu;
	bool menuToggled;

	int accDay;
	string dayText;
	Texture* dayTexture;
	Texture* dayOutline;

	ButtonGO* buybutton;
	bool buying = false;

	void toggleDailyMenu();


public:
	// constructora
	UIMarket(Scene* market);
	~UIMarket();
	void showMoneyText();
	GameObject* createGameObjects(_ecs::_grp_id grp, string textureName, Vector position,
		float width, float height, float rotation, _ecs::_hdr_id handler = _ecs::hdr_INVALID);
	void update();
	void handleEvents() override;
	void render();
	void setDailyMenu();
	void reset();

	virtual Scene* getConnectedScene() { return market; }

	BasketMarket* basketM();
	ButtonGO* BuyButton();
};

