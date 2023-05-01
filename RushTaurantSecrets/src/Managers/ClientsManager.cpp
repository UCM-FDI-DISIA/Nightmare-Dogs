#include "ClientsManager.h"
#include "../Components/DeskComp.h"

#include "../Structure/GameManager.h"
#include "../Scenes/GameScenes/Restaurant.h"
#include "DayManager.h"

#include "../Utilities/checkML.h"
#include "../Structure/Game.h"

void ClientsManager::addFrequently() {
	if (!active_) return;
	// se crea un grupo si: el día no ha terminado y el restaurante y la cola de entrada no están llenas
	if (!ClockComponent::get()->dayHasFinished() && clientsGroups.size() < maxClients && entrance.size() < MAX_ENTRANCE) {
		elapsedTime += deltaTime;

		if (elapsedTime > timer) {
			elapsedTime = 0;
			// se crea un nuevo grupo de clientes
			createGroupClients();
		}
	}
}

void ClientsManager::recolocateEntranceAll(std::list<vector<Client*>>::iterator it) {
	for (it; it != entrance.end(); ++it) {
		vector<Client*> group = *it;
		// las búsquedas modernas se utilizan para recorrer un contenedor de elementos completo
		for (auto client : group) {
			client->getComponent<ClientMovement>()->recolocateEntrance();
		}
	}
}

void ClientsManager::recolocatePayAll(std::list<Client*>::iterator it) {
	for (it; it != pay.end(); ++it) {
		Client* c = *it;
		c->getComponent<ClientMovement>()->recolotatePay();
	}
}

void ClientsManager::createGroupClients() {
	Message m;
	m.id = Message::msg_ADD_CLINETS;
	vector<Client*> group;
	// número de clientes que hay en el grupo
	int num = sdl->rand().nextInt(1, 5);
	m.grp_clients.num = num;

	for (int i = num - 1; i >= 0; --i) {
		int aux = sdl->rand().nextInt(1, 15);
		group.push_back(createClient(i, aux));
		m.grp_clients.clients.push_back(aux);
	
	}
	entrance.push_back(group);
	clientsGroups.push_back(group);

	for (auto client : group) {
		client->getComponent<ClientMovement>()->setGroup(group);
	}
	Game::get()->getCoopHandler()->send(m);
}

void ClientsManager::createGroupClients(const Message& message) {
	vector<Client*> group;
	auto it = message.grp_clients.clients.cbegin();
	for (int i = message.grp_clients.num - 1; i >= 0; --i) {
		group.push_back(createClient(i, *it));
		++it;
	}
	entrance.push_back(group);
	clientsGroups.push_back(group);

	for (auto client : group) {
		client->getComponent<ClientMovement>()->setGroup(group);
	}
}

Client* ClientsManager::createClient(int posGroup,int sprite_) {
	string sprite = "Client_" + to_string(sprite_);
	// a partir de la posición el grupo se calcula donde empieza
	Vector origin = _ecs::OUT_ENTRY;
	origin.setY(origin.getY() - posGroup);

	return new Client(scene, sprite, RelativeToGlobal::pointRestaurant(origin), GameManager::instance()->getTodaysMenu(), entrance.size(), speed, posGroup);
}



void ClientsManager::checkCashRegister() {
	// lista de grupos
	for (auto group : clientsGroups) {
		// se recorre cada cliente del grupo
		for (auto client : group) {
			if (client->getComponent<ClientState>()->getState() == ClientState::CASH_REGISTER) {
				// se le indica la posición a ocupar en la cola
				client->getComponent<ClientMovement>()->setPosPay(pay.size());
				pay.push_back(client);
			}
		}
	}
}

void ClientsManager::firstClientAssigned() {
	if (assignedClient) {
		entrance.pop_front();
		recolocateEntranceAll(entrance.begin());
		assignedClient = false;
	}
}

bool ClientsManager::notAbandonedEntrance(Client* client) {
	return !client->getComponent<ClientMovement>()->hasAbandonedEntrance();
}

void ClientsManager::checkHappinessEntrance() {
	// encontrar a un grupo en el que todos sus integrantes han abandonado la entrada
	bool found = false;
	auto it = entrance.begin();
	while (it != entrance.end() && !found) {
		vector<Client*> group = *it;
		// hasta que todos los clientes del grupo no hayan abandonado la entrada,
		// no se elimina el grupo de la entrada
		// se busca uno que no haya abandonado la entrada y si no se encuentra,
		// es que todos la han abandonado
		found = std::find_if(group.begin(), group.end(), notAbandonedEntrance) == group.end();

		if (found) {
			it = entrance.erase(it);
			recolocateEntranceAll(it);
		}
		if (it != entrance.end()) {
			++it;
		}
	}
}

void ClientsManager::checkHappinessPay() {
	// se utiliza un for porque los clientes de un mismo grupo, la abandonan a la vez
	for (auto it = pay.begin(); it != pay.end(); it) {
		Client* client = (*it);
		bool found = false;
		// si uno de los clientes ha abandonado la caja, se elimian y se recoloca
		if (client->getComponent<ClientMovement>()->hasAbandonedPay()) {
			it = pay.erase(it);
			recolocatePayAll(it);
			found = true;
		}
		// si no se ha eliminado ninguno se avanza el iterador
		// en caso contrario, no se toca porque ya ha avanzado
		if (!found && it != pay.end()) {
			++it;
		}
	}
}

bool ClientsManager::deskIsNotOccupied(DeskComp* desk) {
	return !desk->isOccupied();
}

bool ClientsManager::checkFirstTableEmpty(int& table) {
	auto it = std::find_if(tables.begin(), tables.end(), deskIsNotOccupied);
	// se ha encontrado una mesa no ocupada
	if (it != tables.end()) {
		table = (*it)->getID();
		return true;
	}
	return false;
}

void ClientsManager::assignTable(int table, vector<Client*> firstGroup,bool send) {
	// se le asigna la mesa a cada miembro del grupo
	for (auto client : firstGroup) {
		client->getComponent<ClientMovement>()->assignTable(table);
	}

	// se asigna el grupo de clientes a la mesa
	tables[table - 1]->assignClients(firstGroup);
	assignedClient = true;

	if (send) {
		Message m;
		m.id = Message::msg_ASSIGN_CLIENT;
		m.assignClients.table = table;
		Game::get()->getCoopHandler()->send(m);
	}
}

// comprobar si un cliente si un cliente est?de camino a pagar o pagando
bool ClientsManager::isPaying(GameObject* client) {
	ClientState* state = client->getComponent<ClientState>();
	ClientState::States currentState = state->getState();

	return currentState == ClientState::HAS_LEFT ||
		currentState == ClientState::REGISTER ||
		currentState == ClientState::CASH_REGISTER ||
		currentState == ClientState::PAYING;
}

bool ClientsManager::notOutOfLocal(Client* client) {
	return client->getComponent<ClientState>()->getState() != ClientState::OUT;
}

// comprobar si algún grupo ha abandonado el local para quitarlo de la lista
void ClientsManager::refreshClientsGroup() {
	bool found = true;
	auto it = clientsGroups.begin();
	while (it != clientsGroups.end() && found) {
		vector<Client*> group = *it;

		// se comprueba que todos los integrantes del grupo están saliendo del local
		found = std::find_if(group.begin(), group.end(), notOutOfLocal) != group.end();

		if (!found) {
			it = clientsGroups.erase(it);
		}
		if (it != clientsGroups.end()) {
			++it;
		}
	}
}

void ClientsManager::initTables() {
	for (auto obj : *scene->getGroup(_ecs::grp_DESK)) {
		DeskComp* desk = obj->getComponent<DeskComp>();
		tables[desk->getID() - 1] = desk;
	}
}

ClientsManager::ClientsManager(GameObject* parent, vector<_ecs::_dish_id> menu, float frequencyClients, float speedClients, int maxClients)
	: Manager(parent), menu(GameManager::instance()->getTodaysMenu()), timer(frequencyClients), speed(speedClients), assignedClient(false), maxClients(maxClients), elapsedTime(0), tables() {
	scene = parent->getScene();

	UIRestaurant* uiRest = GameManager::get()->getRestaurant()->getUI();
	if (uiRest == nullptr) {
		throw new string("Error conversión Scene en UIRestaurant");
	}
	GameObject* c = uiRest->getGameObject(_ecs::hdr_CLOCK);
	active_ = true;
	sdl = SDLUtils::instance();
}

void ClientsManager::assignFirstGroup(int table) {
	if (!isEntranceEmpty()) {
		// se coge el primer cliente
		vector<Client*> firstGroup = getFirstEntrance();
		// con que se compruebe que el primero del grupo est?en la entrada,
		// vale para verificar que todo el grupo est?en la entrada
		Client* firstClient = firstGroup.front();
		ClientState::States clientState = firstClient->getComponent<ClientState>()->getState();
		if (clientState == ClientState::ENTRANCE) {
			if (!tables[table - 1]->isOccupied()) {
				assignTable(table, firstGroup);
			}
			else {
				int table = -1;
				if (checkFirstTableEmpty(table)) {
					assignTable(table, firstGroup);
				}
			}
		}
	}
}



bool ClientsManager::notAllGroupPaying(Client* client) {
	return !client->getComponent<ClientMovement>()->isEveryonePaying();
}

bool ClientsManager::canCollect() const {
	// se comprueba si:
	// - la cola de pagar no est?vacía
	// - todos los integrantes de cada grupo están en colocados en la cola listos para pagar
	return !pay.empty() && std::find_if(pay.begin(), pay.end(), notAllGroupPaying) == pay.end();
}

void ClientsManager::collectAndLeave() {
	// se marchan todos los clientes hasta que la caja quede vacía
	while (!pay.empty()) {
		Client* firstPay = pay.front();
		firstPay->getComponent<ClientMovement>()->payAndLeave();
		pay.pop_front();
		auto it = pay.begin();
		recolocatePayAll(it);
	}
}

bool ClientsManager::canOccupyPay(vector<Client*> mates) {
	int goingPay = 0;
	for (auto group : clientsGroups) {
		// con que uno del grupo est?en una de esas posiciones,
		// todos los del grupo están en la misma

		// no se cuenta el grupo que quiere ocupar la caja
		Client* client = group.front();
		if (client != mates.front()) {
			// se cuenta el resto de grupos
			if (isPaying(client)) {
				// se le suma el número de integrantes del grupo
				goingPay = goingPay + group.size();
			}
		}
	}
	// número de clientes que quiere ir + número de clientes que hay de camino a la caja o pagando
	if (mates.size() + goingPay <= MAX_PAY) {
		return true;
	}
	else {
		return false;
	}
}

void ClientsManager::initComponent() {
	initTables();
}

void ClientsManager::update() {
	// se añade con frecuencia un cliente
	addFrequently();

	// se comprueba si se ha asignado el primer grupo a una mesa para quitarlo de la entrada
	firstClientAssigned();

	// añadir los clientes que han llegado a la caja registradora a la cola de pago
	checkCashRegister();

	// eliminar grupos de clientes enfadados de la entrada
	checkHappinessEntrance();

	// eliminar grupos de clientes enfadados de la cola de pagar
	checkHappinessPay();

	refreshClientsGroup();
}

void ClientsManager::nextDay() {
	timer = GameManager::get()->getDayManager()->getClientFrequency();


}


void ClientsManager::receive(const Message& message) {
	if (message.id == Message::msg_ASSIGN_CLIENT) {
		vector<Client*> firstGroup = getFirstEntrance();
		assignTable(message.assignClients.table,firstGroup, false);
	}
	else if (message.id == Message::msg_ADD_CLINETS) {
		createGroupClients(message);
	}
}