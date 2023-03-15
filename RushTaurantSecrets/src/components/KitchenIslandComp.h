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
	enum Player{p1,p2};
private:
	const float
		F_W =12,
		F_H = 25,
		B_H=20,
		B_W=20,OFFSETX=-6,OFFSETY=-6;

	struct Ing {
		int n;	// número de ingredientes
		Texture* t;	// imagen del ingrediente
		Texture* f;	// imagen con el número de ingredientes que tienes (delantera)
		Texture* b;	//
	};
	Font* font; //para indicar n de ing
	unordered_map<_ecs::_ingredients_id,Ing>ing;
	vector<_ecs::_ingredients_id>auxID;
	SDLUtils* sdl;

	Ingredients* ingCloud1;
	Ingredients* ingCloud2;
	Texture* highlight;
	Vector hPos1; //player1
	Vector hPos2;//player2

	Ingredients* ingCloud[2];
	Vector hPos[2];
	int selected[2];

	int selected1;
	int selected2;
	float w, h;// para cada ing
	float x, y;

public:
	KitchenIslandComp(GameObject* parent, vector<pair<_ecs::_ingredients_id,int>> _ing);
	virtual ~KitchenIslandComp();

	virtual void render();
	void pickIngredient(int i,Player p);
	void returnIngredient(_ecs::_ingredients_id id);
	void selectedIng(int i, Player p);
	void unselectIng(int i, Player p);
};

