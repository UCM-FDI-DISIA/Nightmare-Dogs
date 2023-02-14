#pragma once
#include "../structure/Food_def.h"
#include "../gameObjects/CookingMachine.h"
#include <vector>

class Inventory
{
	//static Inventory* _instance;

private:
	/*// cosntructora privada (Singleton) porque s�lo tiene que crearse un inventario
	Inventory(CookingMachine* cM) : cookingMachine(cM) {};

	static Inventory* instance(CookingMachine* cM) {
		if (_instance == nullptr)
			_instance = new Inventory(cM);
		return _instance;
	}*/

	// vector de 3 ingredientes
	vector<_ecs::_dish_id> dishes;
	// determina si el contenido del vector de platos es v�lido o no
	vector<bool> dishesBool;
	CookingMachine* cookingMachine;

public:
	Inventory(CookingMachine* cM);
	void takeDish(_ecs::_dish_id newDish);
	void freeDish(int num);
	int freeSpace();
};
