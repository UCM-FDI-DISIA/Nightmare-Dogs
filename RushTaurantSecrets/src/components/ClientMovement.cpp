#include "ClientMovement.h"
#include "../objects/ClientsManager.h"
#include "../gameObjects/Client.h"
#include "../utils/checkML.h"

// cleon: [musica de desastre y muerte]: es una b�squeda de 1�. Esto es 2� (que es m�s cool).
// esto lo tiene implementado C++.
bool ClientMovement::hasEveryoneEaten() const {
	bool found = true;
	int i = 0;
	// se busca un integrante del grupo que no haya termiando de comer
	while (i < mates.size() && found) {
		if (mates[i]->getComponent<ClientState>()->getState() < ClientState::FINISH_EAT) {
			found = false;
		}
		++i;
	}
	return found;
}

void ClientMovement::goOut(ClientState::States currentState) {
	int i = 0;
	bool found = false;
	// si ya se ha ido no se realiza la comprobaci�n
	while (currentState != ClientState::OUT && i < mates.size() && !found) {
		// cuando se hayan marchado, por si uno de sus compa�eros ha sido eliminado y quiere acceder a �l
		if (mates[i] != nullptr) {
			if (mates[i]->getComponent<ClientState>()->getState() == ClientState::OUT) {
				clientState->unHappy();
				found = true;
			}
		}
		++i;
	}
}

string ClientMovement::posGroupToOrientation(int posGroup) const {
	string orientation;
	switch (posGroup) {
	case 0:
		orientation = "LEFT";
		break;
	case 1:
		orientation = "RIGHT";
		break;
	case 2:
		orientation = "UP";
		break;
	case 3:
		orientation = "DOWN";
		break;
	}
	return orientation;

}

// devuelve una ruta desde la entrada hasta la mesa

Route ClientMovement::tableRoute(string type, string orientation) {
	// cleon: millones de gatitos han  muerto.
	// map<tipo, map<direccion>>
	std::string aux = type + "_TABLE_" + std::to_string(this->assignedTable) + "_" + orientation;
	auto it = _ecs::stringToEnum.find(aux);
	if (it != _ecs::stringToEnum.end()) {
		return paths[it->second];
	}
}

// llega al restaurante y se coloca en la entrada
void ClientMovement::colocateEntrance() {
	Vector entrance = _ecs::ENTRY;
	// se calcula a partir de su pos en la cola de entrada
	entrance.setX(entrance.getX() + posEntrance);
	entrance.setY(entrance.getY() - posGroup);
	// se mueve
	straightMovement->addPath(vector<Vector>{entrance});
}

// desde la mesa hasta la caja registradora
void ClientMovement::colocateCashRegister() {
	vector<Vector> payPath = tableRoute("PAY", posGroupToOrientation(posGroup)).points;
	straightMovement->addPath(payPath);
	outTable();
}

// desde la caja registradora hasta la posici�n que le corresponde en la cola de pagar
void ClientMovement::colocatePay() {
	Vector posRegister = _ecs::CASH_REGISTER;
	// se calcula a partir de su pos en la cola de la caja
	posRegister.setX(posRegister.getX() + posPay);
	// se mueve
	straightMovement->addPath(vector<Vector>{posRegister});
}

// ha abandonado la cola de la caja sin pagar
void ClientMovement::abandonPay() {
	// se calcula el camino de salida
	vector<Vector> leave;

	// primer punto
	int fWidth = sdl->width() / 40;
	// la x del primer punto se saca a partir de la x del �ltimo punto a donde llegue
	int xExit = straightMovement->getLastPoint().getX() / fWidth;
	int yExit = _ecs::OUT_PAY.getY();
	Vector firstPoint = Vector(xExit, yExit);
	leave.push_back(firstPoint);

	// se a�ade el punto de afuera
	leave.push_back(_ecs::OUT_PAY);

	straightMovement->addPath(leave);
}

void ClientMovement::abandonEntrance() {
	Vector end = _ecs::OUT_ENTRY;
	end.setY(end.getY() - posGroup);
	straightMovement->addPath(vector<Vector>{end});
}

// se establece su nuevo estado, su orientaci�n y la animaci�n a ejecutar
void ClientMovement::stationary(ClientState::States state, GOOrientation orientation, movementState mov) {
	clientState->setState(state);
	transform->setOrientation(orientation);
	transform->setMovState(mov);
}

ClientMovement::ClientMovement(GameObject* parent, int posEntrance, int posGroup) :
	Component(parent, id), assignedTable(-1), posEntrance(posEntrance), posPay(-1), posGroup(posGroup) {
	sdl = SDLUtils::instance();
	render = parent->getComponent<ClientStateRender>();
	clientState = parent->getComponent<ClientState>();
	straightMovement = parent->getComponent<StraightMovement>();
	transform = parent->getComponent<Transform>();
	clientsManager = ClientsManager::get();

	colocateEntrance();
}

// recolocarse en la entrada si alguien se ha marchado
void ClientMovement::recolocateEntrance() {
	--posEntrance;
	colocateEntrance();
}

// recolocarse en la cola de pagar si alguien se ha marchado
void ClientMovement::recolotatePay() {
	--posPay;
	colocatePay();
}

// asignar mesa
void ClientMovement::assignTable(int assignedTable) {
	this->assignedTable = assignedTable;
	straightMovement->addPath(tableRoute("ARRIVE", posGroupToOrientation(posGroup)).points);
	outEntrance();
	clientState->setState(ClientState::ASSIGNED);
}

// paga por su comida y se marcha
void ClientMovement::payAndLeave() {
	clientState->setState(ClientState::OUT);
	straightMovement->addPath(_ecs::paths[_ecs::PAY_AND_LEAVE].points);
	outPay();
}

void ClientMovement::update() {
	ClientState::States currentState;
	currentState = clientState->getState();

	goOut(currentState);

	switch (currentState) {
	case ClientState::START:
		if (straightMovement->hasFinishedPath()) {
			stationary(ClientState::ENTRANCE, west, idle);
		}
		break;
	case ClientState::ENTRANCE:
		// hacer que al recolocarse se active la animaci�n idle
		if (straightMovement->hasFinishedPath()) {
			transform->setMovState(idle);
		}
		break;
	case ClientState::ASSIGNED:
		if (straightMovement->hasFinishedPath()) {
			stationary(ClientState::THINKING, tableRoute("ARRIVE", posGroupToOrientation(posGroup)).orientation, sitting);
			// para que renderice el estado de pensar
			render->renderThinkingState();
		}
		break;
	case ClientState::FINISH_EAT:
		// se comprueba si puede ir a la cola de pagar
		// es decir, todos los integrantes del grupo han terminado de comer y la cola de pagar tiene espacio
		if (hasEveryoneEaten() && clientsManager->canOccupyPay(mates)) {
			clientState->setState(ClientState::HAS_LEFT);
		}
		break;
		// un ciclo de espera para que las mesas oportunas se desocupen
	case ClientState::HAS_LEFT:
		colocateCashRegister();
		clientState->setState(ClientState::REGISTER);
		break;
	case ClientState::REGISTER:
		if (straightMovement->hasFinishedPath()) {
			clientState->setState(ClientState::CASH_REGISTER);
		}
		break;
	case ClientState::CASH_REGISTER:
		// hasta que no se le ha asignado una posici�n en la caja no se mueve hasta ella
		if (posPay != -1) {
			colocatePay();
			clientState->setState(ClientState::PAYING);
		}
		break;
	case ClientState::PAYING:
		if (straightMovement->hasFinishedPath()) {
			transform->setOrientation(west);
			transform->setMovState(idle);
		}
		break;
	case ClientState::OUT:
		if (posEntrance != -1) {
			abandonEntrance();
			outEntrance();
		}
		else if (assignedTable != -1) {
			straightMovement->addPath(tableRoute("OUT", posGroupToOrientation(posGroup)).points);
			outTable();
		}
		else if (posPay != -1) {
			abandonPay();
			outPay();
		}
		if (straightMovement->hasFinishedPath()) {
			parent->setAlive(false);
		}
		break;
	}
}