#pragma once

#include "../Structure/GameObject.h"
#include "../Components/Transform.h"
#include "../Utilities/SDLUtils.h"
#include "../Utilities/InputHandler.h"
#include "../Components/BasketMarketComponent.h"
#include "../Components/ShowControlAuto.h"

class ShoppingMenuComp :public Component
{
private:
	const float
		MENU_WIDTH = 230,
		MENU_HEIGHT = 115,
		PLAYER_OFFSETY = -5,

		PRICE_OFFSETX = 33, //respecto del menu
		PRICE_OFFSETY = 9,
		PRICE_WIDTH = 18,
		PRICE_HEIGHT = 30,

		ING_OFFSETX = 15,
		ING_OFFSETY = 50,
		ING_WIDTH = 50,
		ING_HEIGHT = 50,

		NUM_OFFSETX = 108,
		NUM_OFFSETY = 60,
		NUM_WIDTH = 32,
		NUM_HEIGHT = 25,

		TOT_OFFSETX = 122,
		TOT_OFFSETY = 7,
		TOT_WIDTH = 20,
		TOT_HEIGHT = 34;

	int totalPrice;
	int number;
	int price;

	Font* font;
	Texture* menu;
	Texture* priceTex;
	Texture* numberTex;
	Texture* totalPriceTex;
	Texture* ingTex;
	_ecs::_ingredients_id ing;

	Transform* playerTransform;
	BasketMarketComponent* basket;
	SDLUtils* sdl;
	InputHandler* ih;

	SoundEffect* menuSound;
	SoundEffect* selectNum;
	SoundEffect* addIng;
	SoundEffect* confirmSound;

	ShowControlComp* showControl;
public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_SHOP_MENU;
	ShoppingMenuComp(GameObject* parent);
	~ShoppingMenuComp();
	virtual void initComponent();
	virtual void handleEvents();
	virtual void render();
	void increaseN();
	void decreaseN();
	void setIngredient();
	void changePrice();
	void openMenu(_ecs::_ingredients_id _id);
	void closeMenu();
};


