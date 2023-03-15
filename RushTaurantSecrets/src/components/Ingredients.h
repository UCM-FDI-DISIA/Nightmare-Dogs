#pragma once
#include "../structure/Food_def.h"
#include "../structure/Component.h"
#include "../sdlutils/SDLUtils.h"
#include "../structure/GameObject.h"
#include "../components/Transform.h"
#include "../structure/Scene.h"
#include "../components/KitchenIslandComp.h"
#include "../sdlutils/InputHandler.h"
#include <vector>
#include <utility>

using namespace std;
using namespace _ecs;

class KitchenIslandComp;
class Ingredients: public Component {
	const float ING_HEIGHT = 38;
	const float ING_WIDTH = 38;
	//constante que parametriza a qu� altura sobre el jugador se sit�an los ingredientes
	const float ING_POSY = 50;
	//constante que deja un espacio entre ingredientes cuando estos se renderizan
	const float ING_OFFSET = 40;
	//espacio entre los ingredientes y los bordes del bocadillo de pensamiento
	const float BUBBLE_OFFSET_X = 20;
	const float BUBBLE_OFFSET_Y = 20;
	//distancia del bocadillo al jugador
	const float BUBBLE_POSY = 60;
	//centro del jugador en su eje x
	const float PLAYER_CENTER_X = 15;
	//offset de los ingredientes con respecto a la pos X del player para centrarlo sobre su cabeza
	const float OFFSET_ING_X=-15;

private:
	const int MAX_INGREDIENTS = 5;
	// vector de ingredientes
	vector<_ecs::_ingredients_id> ingredients;
	Texture* texture;
	Texture* bubble_tex;
	Transform* transform;
	SDLUtils* sdl;
	//vector de coordenadas de ingredientes para renderizarlos
	vector<pair<float, float>> coord;
	// rectángulo en el mundo donde se va a colocar la textura
	SDL_Rect dest;
	SDL_Rect dest_bubble;	
	KitchenIslandComp* kitchenIsland; //para devolver ing a la mesa

	void debug(); //provisional

public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_INGREDIENTS;
	Ingredients(GameObject* parent) :Component(parent, id), sdl(SDLUtils::instance()), texture(nullptr) {
		transform = parent->getComponent<Transform>();
		coord = { { 0,0 } };
		//para demo, luego se borra; 
		//addIngredient(HUEVO);
		//addIngredient(HARINA);
		//addIngredient(HUEVO);
	}
	//devuelve por ref
	vector<_ecs::_ingredients_id>& getIngredients() { return ingredients; };
	void addIngredient(_ecs::_ingredients_id ingr);
	void removeLastIngredient();
	void removeAllIngredients();
	void cookingIngredients();
	void removeWhenExit();
	void render();
	void initComponent() override;

};