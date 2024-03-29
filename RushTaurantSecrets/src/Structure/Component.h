#pragma once
#include "../Definitions/Structure_def.h"
#include "../Definitions/Message_def.h"

class GameObject;
class Component {
protected:
	GameObject* parent;
	bool alive;
	bool active; //indica si el componente est?activo (si se actualizan update, render,handleEvents,...)
	const uint32_t& deltaTime;
public:
	static constexpr _ecs::_cmp_id id = _ecs::cmp_INVALID;

	Component(GameObject* parent, _ecs::_cmp_id id,bool active=true);
	virtual ~Component() { }

	inline bool isAlive() { return alive; } //para comprobar su existencia, en caso contrario se borra
	inline bool isActive() { return active; } //para comprobar si est?activo, en caso contrario no se actualiza

	virtual void update() { }
	virtual void render() { }
	virtual void handleEvents() { }
	virtual void setActive(bool b); //activar/desactivar componente
	virtual void initComponent(){};
	virtual void nextDay() { }
	virtual void receive(const Message& message) { }
};