#pragma once
#include <unordered_map>
#include <vector>
#include "../Definitions/Structure_def.h"
#include"../Definitions/Message_def.h"

class Component;
class Scene;

class GameObject {
private:
	friend class Component;

	void deleteCompVector(Component* comp);
protected:
	std::unordered_map<_ecs::_cmp_id, Component*> components;
	std::vector<Component*> cmpOrder;
	Scene* scene;
	bool alive;

public:
	GameObject(Scene* scene, _ecs::_grp_id grp = _ecs::grp_GENERAL, _ecs::_hdr_id handler = _ecs::hdr_INVALID);
	void addComponent(Component* comp, _ecs::_cmp_id id);
	virtual ~GameObject();

	bool isAlive() { return alive; }

	void setAlive(bool alive) { this->alive = alive; }

	virtual Scene* getScene() { return scene; }

	void refresh();
	void update();
	void render();
	void handleEvents();
	void initComponent();
	void nextDay();
	void receive(const Message& message);

	template<typename Comp>
	inline Comp* getComponent() {
		auto it = components.find(Comp::id);
		if (it == components.end()) {
			return nullptr;
		}
		return static_cast<Comp*>(it->second);
	}

	template<typename Comp>
	inline void setActive(bool b) {
		auto it = components.find(Comp::id);
		if (it == components.end())return;
		it->second->setActive(b);
	}

	void setActives(bool b);

	bool isActive();
};