#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

#include "../Utilities/checkML.h"

GameObject::GameObject(Scene* scene, _ecs::_grp_id grp, _ecs::_hdr_id handler) : scene(scene), alive(true) {
	scene->addObject(this, grp, handler);
}

GameObject::~GameObject() {
	for (auto& i : components) {
		delete i.second;
	}
}

void GameObject::addComponent(Component* comp, _ecs::_cmp_id id) {
	auto it = components.find(id);
	if(it != components.end()) {
		deleteCompVector(it->second);
		delete it->second;
	}
	components[id] = comp;
	cmpOrder.push_back(comp);
}

void GameObject::deleteCompVector(Component* comp) {
	for(auto it = cmpOrder.cbegin(); it != cmpOrder.cend(); ++it) {
		if(*it == comp) {
			cmpOrder.erase(it);
			break;
		}
	}
}

void GameObject::refresh() {
	auto it = components.begin();
	while(it != components.end()) {
		if(!it->second->isAlive()) {
			deleteCompVector(it->second);
			delete it->second;
			it = components.erase(it);
		}
		else ++it;
	}
}

void GameObject::update() {
	for(auto i : cmpOrder)
		if(i->isActive()) i->update();
}

void GameObject::render() {
	for(auto i : cmpOrder)
		if(i->isActive()) i->render();
}

void GameObject::handleEvents() {
	for(auto i : cmpOrder)
		if(i->isActive()) i->handleEvents();
}

void GameObject::initComponent() {
	for(auto i : cmpOrder)
		i->initComponent();
}

void GameObject::nextDay() {
	for(auto i : cmpOrder)
		i->nextDay();
}

void GameObject::receive(const Message& message) {
	for(auto cmp : cmpOrder)
		cmp->receive(message);
}

void GameObject::setActives(bool b) {
	for (auto &comp : components) {
		comp.second->setActive(b);
	}
}

bool GameObject::isActive() {
	for (auto& comp : components) {
		if(!comp.second->isActive())
			return false;
	}
	return true;
}