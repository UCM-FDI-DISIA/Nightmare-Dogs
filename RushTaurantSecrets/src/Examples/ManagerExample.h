#pragma once
#include "../structure/Manager.h"

class ManagerExample : public Manager<ManagerExample> {
	// para acceder a la constructora privada
	friend class Manager<ManagerExample>;
private:
	// Constructora en privado y que solo tenga de par�metro "parent"
	ManagerExample(GameObject* parent) : Manager(parent) { }
public:
	// A�adir id!!!!!
	constexpr static _ecs::id_type id = _ecs::cmp_INVALID;
	~ManagerExample() { }
};