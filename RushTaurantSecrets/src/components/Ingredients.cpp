#include "../utils/checkML.h"
#include "Ingredients.h"


void Ingredients::addIngredient(_ecs::_ingredients_id ingr) {
	if (ingredients.size() == MAX_INGREDIENTS) {
		removeLastIngredient(); //elimina el ultimo ingrediente añadido
	}
	ingredients.push_back(ingr);
	
	if (ingredients.size() > 0){
		int i = 0;
		for (i = 0;i < coord.size();++i) {
			coord[i].first -= (0.5*ING_OFFSET);
			coord[i].second = 0;
		}
		//Nuevas coordenadas del nuevo ingrediente
		coord.push_back({ coord[i - 1].first, coord[i - 1].second });
		coord[i].first += ING_OFFSET;
	}

}

void Ingredients::initComponent() {
	if (parent->getScene()->getGameObject(_ecs::hdr_KITCHENISLAND) != nullptr)
		kitchenIsland = parent->getScene()->getGameObject(_ecs::hdr_KITCHENISLAND)->getComponent<KitchenIslandComp>();
	else kitchenIsland = nullptr;
}

void Ingredients::removeLastIngredient() {
	
 	kitchenIsland->returnIngredient(ingredients[ingredients.size() - 1]);
	
	if (ingredients.size() != 0) ingredients.pop_back();
	else return;

	for (int i = 0; i < coord.size(); ++i) {
		coord[i].first += (0.5 * ING_OFFSET);
		coord[i].second = 0;
	}
	coord.pop_back();
	
}

void Ingredients::removeAllIngredients() {
	//no es un for porque, a pesar de saber las vueltas que da, se va reduciendo el tamaño del vector
	int i = ingredients.size(); //i vale 5 (el numero de ingredientes maximo que se puede llevar) o menos si no esta lleno el vector
	while (i != 0) {
		//devolver a la mesa
		removeLastIngredient();
		--i;
	}
	coord = { { 0,0 } };
	

}

void Ingredients::cookingIngredients() {
	int i = ingredients.size(); 
	while (i != 0) {
		ingredients.pop_back();

		for (int i = 0; i < coord.size(); ++i) {
			coord[i].first += (0.5 * ING_OFFSET);
			coord[i].second = 0;
		}
		coord.pop_back();
		--i;
	}
	coord = { { 0,0 } };
}

void Ingredients::removeWhenExit() {
	
	//el método actual solo se llama cuando el personaje sale de la cocina
	removeAllIngredients();	
}

void Ingredients::render() {
	if (ingredients.size() >= 1) {
		dest_bubble.w = ING_WIDTH * (ingredients.size()) + BUBBLE_OFFSET_X + ING_OFFSET*(ingredients.size()-1)*0.25;
		dest_bubble.h = ING_HEIGHT + 2 * BUBBLE_OFFSET_Y;

		dest.w = ING_WIDTH;
		dest.h = ING_HEIGHT;

		dest_bubble.x = transform->getPos().getX() + PLAYER_CENTER_X - dest_bubble.w / 2;
		dest_bubble.y = transform->getPos().getY() - BUBBLE_POSY;

		bubble_tex = &((*sdl).images().at("BUBBLE"));
		bubble_tex->render(dest_bubble);
	}
	

	//Se añade las coordenadas del jugador sobre las coordenadas centradas en 0,0 para que pasen a estar sobre el player
	for (int i = 0; i < coord.size();++i) {
		coord[i].first += transform->getPos().getX();
		coord[i].second += (transform->getPos().getY() - ING_POSY);
	}
	// se pintan todas las texturas que hay en el vector
	int k = 0; // mejor con un iterador.
	for (auto it = ingredients.begin(); it != ingredients.end(); ++it) {// cleon: bienvenidos a 1999. muy retro.
		_ecs::_ingredients_id ingr = *it;
		texture = &((*sdl).images().at(to_string(ingr)));
		dest.x = coord[k].first - OFFSET_ING_X;
		dest.y = coord[k].second;

		texture->render(dest);

		++k;
	}
	//Se eliminan las coordenadas del player para que las coordenadas vuelvan a ser sobre 0,0 y no se se haga una suma
	//sobre la anterior en cada tick y que salgan de la pantalla
	for (int i = 0; i < coord.size();++i) { // cleon: bienvenidos a 1975.
		coord[i].first -= transform->getPos().getX();
		coord[i].second -= (transform->getPos().getY() - ING_POSY);
	}
}

void Ingredients::debug() {
	/*bool func = false;
	if (ingredients.size() == coord.size()) func = true;*/

	//cout << func;

	cout << (ingredients.size() == coord.size()); // cleon. PLEASE.
}
