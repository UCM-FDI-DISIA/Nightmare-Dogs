#include "CustomerManager.h"
#include "../structure/Scene.h"
#include "../structure/GameObject.h"
#include "../components/Image.h"
#include "../components/CustPathing.h"

void CustomerManager::createCustomer(Vector origin, Vector end) {
	GameObject* customer = new GameObject(scene, _ecs::grp_CUSTOMERS);
	new CustPathing(customer, origin, end, 3, 50, 50);
	new Image(customer, &((*sdl).images().at("prueba")));

	entrada.push_front(customer);
}

void CustomerManager::deleteFirstCustomer() {
	GameObject* firstCustomer = entrada.back();
	firstCustomer->setAlive(false);
	entrada.pop_back();
}

void CustomerManager::recolocate() {
	for (int i = 0; i < entrada.size(); ++i) {
		CustPathing* c = entrada[i]->getComponent<CustPathing>();
		Vector pos = c->getPos();
		c->newPath(pos * 1.3);
	}
}

CustomerManager::CustomerManager(Scene* scene) : scene(scene), offset(20) {
	sdl = SDLUtils::instance();

	createCustomer(Vector(1000, 1000), Vector(800, 300));
}

void CustomerManager::addCustomerFrequently() {
	if (entrada.size() < 3) {
		if (entrada.front()->getComponent<CustPathing>()->hasArrived()) {
			createCustomer(Vector(1200, 600), Vector(800 + entrada.size() * offset, 300 + entrada.size() * offset));
		}
	}
	else {
		if (entrada.front()->getComponent<CustPathing>()->hasArrived()) {
			deleteFirstCustomer();
			// recolocate();
		}
	}
}