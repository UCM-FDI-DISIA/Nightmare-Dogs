#pragma once
#include "../structure/Component.h"
#include "TriggerComp.h"
#include <vector>
class Transform;

class DeskComp : public TriggerComp {
private:
	Transform* trans;
	std::vector<GameObject*> assigned;
	bool sucia;
	int num;
public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_DESK;

	DeskComp(GameObject* parent);

	// Se pasan como par�metros los clientes que se quieran asignar a la mesa. Devuelve true si se han podido asignar,
	// o false si no se ha podido (la mesa estaba ocupada o sucia).
	bool assignClients();

	// Se pasa como par�metro el plato que se quiera servir a los clientes asignados a la mesa.
	// Devuelve true si un cliente ha recibido el plato que quer�a,
	// o false si nadie ha aceptado el plato (no lo ped�an o ya lo han recibido) 
	void spreadOverlap();

	// Se llama cuando los clientes se van de la mesa.
	void leaveDesk();

	// Se llama cuando se quiere limpiar la mesa.
	void cleanDesk();

	void isOverlapping();
};