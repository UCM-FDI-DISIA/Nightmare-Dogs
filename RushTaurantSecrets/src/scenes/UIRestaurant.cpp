#include "UIRestaurant.h"

UIRestaurant::UIRestaurant() : UI() {
	// se a�aden los iconos a la lista de gameObjects
	// icono de diana (objetivo diario)
	// CAMBIAR POSICI�N DEFINITIVA
	objects.push_back(new Icon(this, 10, 50, texturesUI[4]));
	// icono de reputaci�n (anterior a las estrellitas)
	// CAMBIAR POSICI�N DEFINITIVA 
	objects.push_back(new Icon(this, 10, 50, texturesUI[5]));
	// TO-DO ESTRELLITAS
	// inventario de platos cocinados (tres espacios)
	// CAMBIAR POSICI�N DEFINITIVA 
	objects.push_back(new Icon(this, 10, 50, texturesUI[6]));

}

// pasar a clase Inventory
// si la CookingMachine ha terminado (y hay espacio en el inventario), recoge el plato
void UIRestaurant::takeDish(_ecs::_dish_id newDish) {
	if (cookingMachine->isFinished()) dishes->push_back(newDish);
}