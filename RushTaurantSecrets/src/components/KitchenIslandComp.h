#pragma once

#include "../structure/Component.h"
#include "../objects/DishCombinator.h"
#include "../components/UIAnimator.h"
#include "../components/Transform.h"

#include "../components/Image.h"
#include "../sdlutils/SDLUtils.h"
#include "../utilities/Vector.h"
#include <unordered_map>

using namespace std;
class Ingredients;
class KitchenIslandComp :public Component
{
public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_KITCHENISLAND;
private:
	const float
		F_W =12,
		F_H = 25,
		B_H=20,
		B_W=20,OFFSETX=-6,OFFSETY=-6;

	struct Ing {
		int n;	// n�mero de ingredientes
		Texture* t;	// imagen del ingrediente
		Texture* f;	// imagen con el n�mero de ingredientes que tienes (delantera)
		Texture* b;	//
	};
	Font* font; //para indicar n de ing
	unordered_map<_ecs::_ingredients_id,Ing>ing;
	vector<_ecs::_ingredients_id>auxID;
	SDLUtils* sdl;
	Ingredients* ingCloud;
	Texture* highlight;
	Vector hPos;
	int selected;
	float w, h;// para cada ing
	float x, y;

public:
	KitchenIslandComp(GameObject* parent);
	virtual ~KitchenIslandComp();

	virtual void render();
	void pickIngredient(int i);
	void returnIngredient(_ecs::_ingredients_id id);
	void selectedIng(int i);
	void unselectIng(int i);

	void setIngredients(vector<pair<_ecs::_ingredients_id, int>> _ing);
};

