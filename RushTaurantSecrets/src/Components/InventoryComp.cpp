#include "InventoryComp.h"

#include "../Utilities/checkML.h"

InventoryComp::InventoryComp(GameObject* parent) :Component(parent, id), 
sdl(SDLUtils::instance()),
serveDishSound(&sdl->soundEffects().at("SERVE_CLIENT"))
{
	
	ih = InputHandler::instance();
	serveDishSound->setVolume(10);
	dishes.reserve(MAX_DISHES);
	dishesBool.reserve(MAX_DISHES);
	// inicializa el vector de booleanos
	for (int i = 0; i < MAX_DISHES; i++) {
		dishesBool.push_back(false);
	}
	for (int i = 0; i < MAX_DISHES; i++) {
		dishes.push_back(_ecs::MEATBALLS);
	}

	cellSelected = -1;
	cellsOcuppied = 0;
	highlight = &sdl->images().at("INVENTORY_HIGHLIGHT");

	// prueba--borrar luego
	/*dishesBool[0] = true;
	dishesBool[1] = true;
	cellsOcuppied = 2;
	dishesBool[2] = true;*/
}

// si hay espacio en el inventario, recoge el plato (suponiendo que previamente ya se ha comprobado si cookingMachine est�� listo en otra clase)
void InventoryComp::takeDish(_ecs::_dish_id newDish) {
	// comprueba que hay espacio libre en el inventario
	int place = freeSpace();
	if (place != -1) {
		// Si el inventario estaba vacío, se pone el primer hueco como seleccionado
		if(cellsOcuppied == 0)
			cellSelected = 0;
		dishes[place] = newDish;
		dishesBool[place] = true;
		cellsOcuppied++;
	}
}

// Libera el espacio de la casilla seleccionado y pone la casilla seleccionada a la siguiente casilla ocupada 
void InventoryComp::freeDish() {
	if (cellsOcuppied > 0) {
		dishesBool[cellSelected] = false;
		nextDish();
		cellsOcuppied--;

		if(cellsOcuppied == 0) cellSelected = -1;
	}
}

// devuelve la primera posici�n libre; si no hay espacio libre, devuelve -1
int InventoryComp::freeSpace() {
	int i = 0;
	bool encontrado = false;
	while (i < MAX_DISHES && !encontrado) {
		// si hay un hueco libre
		if (dishesBool[i] == false) return i;
		// si no hay un hueco libre
		else i++;
	}

	// devuelve -1 si no tiene ning�n hueco libre
	if (i == MAX_DISHES) return -1;
}

bool InventoryComp::serveDish(_ecs::_dish_id dish) {
	if (cellsOcuppied > 0) {
		if (dishes[cellSelected] == dish) {
			freeDish();
			serveDishSound->play();
			return true;
		}
		else return false;

	}
	else return false;
}

void InventoryComp::renderDish(int xD, int yD, _ecs::_dish_id dishID) {
	SDL_Rect dest;
	dest.x = xD;
	dest.y = yD;
	dest.w = DISH_SIZE;
	dest.h = DISH_SIZE;

	string dID = to_string(dishID);

	// renderiza la textura
	texture = &((*sdl).images().at(dID));
	texture->render(dest);
	
}

void InventoryComp::render() {
	// recorre la lista de platos para renderizarlos
	for (int i = 0; i < MAX_DISHES; i++) {
		// si el plato est?en el inventario, se renderiza
		if (dishesBool[i]) {
			int x, y;
			setPosition(i, x, y);
			renderDish(x, y, dishes[i]);
		}
	}

	// Si hay alguna casilla seleccionada, se renderiza el highlight
	if (cellSelected > -1)
		highlight->render({ HIGHLIGHT_X, HIGHLIGHT_Y + DISHOFFSET * cellSelected, HIGHLIGHTSIZE, HIGHLIGHTSIZE });
}

void InventoryComp::setPosition(int i, int& x, int& y) {
	x = DISHX;
	y = DISHY + DISHOFFSET * i;
}


void InventoryComp::handleEvents() {
	//  hacia arriba
	if (cellSelected > -1 && (ih->joysticksInitialised() && ih->getButtonState(0, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) || !ih->joysticksInitialised() && ih->isKeyDown(SDLK_LEFT))) 
		prevDish();
	//  hacia abajo
	else if (ih->joysticksInitialised() && ih->getButtonState(0, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) || !ih->joysticksInitialised() && ih->isKeyDown(SDLK_RIGHT)) 
		nextDish();
}

// busca la primera casilla anterior a la seleccionada que esté ocupada
void InventoryComp::prevDish() {
	int i = 0;

	// Da como máximo 3 vueltas al inventario
	while (i < MAX_DISHES && cellsOcuppied > 1) {
		// Reduce la casilla seleccionada y si es menor que 0, la pone en la última casilla 
		cellSelected--;
		if (cellSelected < 0) cellSelected = MAX_DISHES - 1;

		// Si la casilla está ocupada, sale del método
		if (dishesBool[cellSelected]) return;
		i++;
	}
}

// busca la primera casilla posterior a la seleccionada que esté ocupada
void InventoryComp::nextDish() {
	int i = 0;

	// Da como máximo 3 vueltas al inventario
	while (i < MAX_DISHES && cellsOcuppied > 1) {
		// Aumenta la casilla seleccionada y si es mayor o igual al
		// número máximo de platos, la pone en la primera casilla 
		cellSelected++;
		if (cellSelected >= MAX_DISHES) cellSelected = 0;

		// Si la casilla está ocupada, sale del método
		if (dishesBool[cellSelected]) return;
		i++;
	}
}

void InventoryComp::nextDay() {
	cellSelected = -1;
	for (auto b : dishesBool) {
		b = false;
	}
}