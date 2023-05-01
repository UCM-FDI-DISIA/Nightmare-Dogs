#include "ClientState.h"

#include "../Structure/GameManager.h"
#include "../Managers/Reputation.h"
#include "../Managers/DayManager.h"
#include "../Utilities/checkML.h"

ClientState::ClientState(GameObject* parent, vector<_ecs::DishInfo>* menu) :
	Component(parent, id), state(START), happiness(60), timer(0), availableDishes(menu), 
	orderedDish(NONE_DISH), dishChanged(false), render(parent->getComponent<ClientStateRender>()) , 
	takeNote(&sdlutils().soundEffects().at("TAKE_NOTE"))
{
	availableDishes = menu;
	render->clientStateIsReady(); //decirle que ya esta creado
	decreaseFreq = GameManager::get()->getDayManager()->getHappinessFrequency();
	reputationDecrease = GameManager::get()->getDayManager()->getReputationDecrease();
}

ClientState::States ClientState::getState() const { return state; }

void ClientState::setState(States state) { 
	if (state == ASSIGNED || state == ORDERED || state == EATING) happiness += 5;
	timer = 0;
	this->state = state;
}


void ClientState::update() {
	// Si est?en estados en los que el jugador tenga que interactuar con �l, va bajando la felicidad poco a poco
	if (state == ENTRANCE || state == TAKEMYORDER || state == ORDERED || state == PAYING || state == FINISH_EAT) {
		
		timer += deltaTime;
		if (timer > decreaseFreq) {
			timer = 0;
			happiness--;
		}

		// Si la felicidad llega a 0, se pone el estado a OUT
		if (happiness <= 0) {
			if(state < FINISH_EAT)
				GameManager::get()->getReputation()->reduceReputation(reputationDecrease);
			setState(OUT);
			render->renderFinishEatState();
#ifdef _DEBUG
			cout << "Happiness reached 0, leaving restaurant" << endl;
#endif
		}
	}
	// Si no, si est?pensando o comiendo, se actualiza el temporizador de lo que tarda en realizar la acci�n
	else if (state == THINKING || state == EATING) {
		timer += deltaTime;

		// Si est?pensando y termina de pensar, pasa al estado de pedir la comida (reinicia el contador)
		if (state == THINKING && timer >= THINKINGTIME) {
#ifdef _DEBUG
			cout << "I know what I want to eat" << endl;
#endif
			setState(TAKEMYORDER);	// TAKEMYORDER
			
			takeNote->play();
			render->renderTakingNoteState();
		}
		// Si est?comiendo y termina de comer, pasa al estado de caminar hacia la caja
		else if (state == EATING && timer >= EATINGTIME) {
			setState(FINISH_EAT);
#ifdef _DEBUG
			cout << "I'm done eating" << endl;
#endif
			render->renderFinishEatState();
		}
	}


}


// Funci�n que asigna al plato pedido uno aleatorio de entre los
// disponibles en el men?del d�a y cambia el estado a ORDERED
void ClientState::takeOrder() {
	int rndDish;
	// Si no se ha cambiado el plato, escoge uno aleatorio
	if (!dishChanged) {
		rndDish = rand() % availableDishes->size();
		orderedDish = availableDishes->at(rndDish).id;
	}
	// Si se ha cambiado el plato, escoge uno 
	// aleatorio hasta que sea diferente del anterior
	else {
		_ecs::_dish_id lastDish = orderedDish;
		while (lastDish == orderedDish) {
			rndDish = rand() % availableDishes->size();
			orderedDish = availableDishes->at(rndDish).id;
		}
	}

#ifdef _DEBUG
	cout << "Order taken, I want " << rndDish << endl;
#endif
	setState(ORDERED);
	render->renderOrderingState();
}


// Funci�n que devuelve el plato que se ha pedido
_ecs::_dish_id ClientState::getOrderedDish() { return orderedDish; }


// Funci�n que informa que ha sido servido y cambia el estado a EATING
void ClientState::getServed() {
#ifdef _DEBUG
	cout << "Thanks for the food" << endl;
#endif
	setState(EATING);
	render->renderEatingState();
}


// Funci�n que hace que el cliente vuelva al estado de pensar si no se le ha 
// cambiado y le baja la felicidad o hace que se vaya si se le ha cambiado antes 
void ClientState::changeDish() {
	if (!dishChanged) {
		dishChanged = true;
		happiness -= 20;
		setState(THINKING);
		render->renderThinkingState();
	}
	else {
		unhappy();
		render->renderFinishEatState();
	}
}


// Funci�n que devuelve la felicidad del cliente
float ClientState::getHappiness() {
	return happiness;
}