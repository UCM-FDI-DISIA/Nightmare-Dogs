#pragma once
#include "../Structure/Component.h"
#include "../Structure/TriggerComp.h"
#include "../Utilities/SDLUtils.h"
#include <vector>
class Transform;
class Client;

class DeskComp : public TriggerComp {
private:
	Transform* trans;
	std::vector<Client*> assigned;
	bool sucia;
	int num;
	const float
		ICON_WIDTH = 40,
		ICON_HEIGHT = 40,
		ICON_OFFSETY = -25,
		PLATE_WIDTH = 30,
		PLATE_HEIGHT = 30;
	Texture* dirtyIcon;
	Texture* dirtyPlate;
	SoundEffect* cleanSound;
	SoundEffect* assignSound;
public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_DESK;

	DeskComp(GameObject* parent, float width, float height, int id);

	int getID() { return num; }

	void assignClients(const std::vector<Client*>& clients);

	// Se pasa como par�metro el plato que se quiera servir a los clientes asignados a la mesa.
	// Devuelve true si un cliente ha recibido el plato que quer�a,
	// o false si nadie ha aceptado el plato (no lo ped�an o ya lo han recibido) 
	void spreadOverlap();

	// Se llama cuando se quiere limpiar la mesa.
	void cleanDesk();

	bool isOccupied();

	void isOverlapping() override;

	void update() override;
	void render() override;
};