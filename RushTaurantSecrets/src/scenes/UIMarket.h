#pragma once
#include "../structure/Scene.h"
#include "../sdlutils/SDLUtils.h"
#include "../structure/GameObject.h"
#include "../components/Transform.h"
#include "../components/Image.h"
#include "../objects/Money.h" // cambiar cuando se cambie la clase Money
#include "../gameObjects/BasketMarket.h"

// UI del supermercado
class UIMarket : public Scene
{
private:
	SDLUtils* sdl = SDLUtils::instance();
	Money* moneyTxt;
	Font* f;
	GameObject* moneyText;
	GameObject* timeText;
	Texture* moneyTextTexture;
	Image* moneyTextImage;
	int intMoney;
	const string FONT_PATH = "assets/Fonts/Arcadepix Plus.ttf";
	InputHandler* ih;
	BasketMarket* basketMarket;

public:
	// constructora
	UIMarket();
	~UIMarket();
	void showMoneyText();
	void update();
	void handleEvents() override;
	void render();
};

