#pragma once
#include "../structure/Food_def.h"
#include <list>

class Player {
private:
	list<_ingredients_id> ingredients ;

	void addIngredient(list<_ingredients_id>* ingredients);
	void removeLastIngredient(list<_ingredients_id>* ingredients);
	void removeAllIngredients(list<_ingredients_id>* ingredients);
	void removeWhenExit(list<_ingredients_id>* ingredients);

public:

	Player();
	virtual ~Player();

};
