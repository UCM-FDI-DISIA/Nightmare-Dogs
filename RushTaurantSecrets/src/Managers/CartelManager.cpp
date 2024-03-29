#include "CartelManager.h"

#include "../Utilities/checkML.h"

CartelManager::CartelManager(Scene* scene) : scene(scene), selectedCartel(nullptr),
menu(GameManager::get()->getSupermarket()->getUI()->getGameObject(hdr_SHOP_MENU1)->getComponent<ShoppingMenuComp>()),
selectSound(&sdlutils().soundEffects().at("SELECT")){ }

void CartelManager::signIn(CartelTrigger* c) {
	if (c == selectedCartel) return;
	if (selectedCartel != nullptr) //si ya existia un cartel seleccionado
		selectedCartel->unHighlight(); //se deselecciona
	selectedCartel = c; //nuevo seleccionado
	selectedCartel->highlight();
	selectSound->play();
}
void CartelManager::signOut(CartelTrigger* c) {
	if (c == selectedCartel) {
		c->unHighlight();
		selectedCartel = nullptr;
	}

}

void CartelManager::openMenu() {
	if (selectedCartel != nullptr)
		menu->openMenu(selectedCartel->getIngredient());
}
void CartelManager::closeMenu() {
	if (selectedCartel == nullptr)
		menu->closeMenu();
}