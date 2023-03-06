#include "ClientTrigger.h"
#include "../objects/ClientsManager.h"

#include "../utils/checkML.h"

void ClientTrigger::isOverlapping()
{
	if (ih->isKeyDown(SDLK_SPACE) || inputMando()) {
		ClientState::States state = clientState->getState();
		switch (state)
		{
		case ClientState::ENTRANCE:
			ClientsManager::get()->assignFirstGroup(randomTable());
			break;
		case ClientState::TAKEMYORDER:
			clientState->takeOrder();
			break;
		case ClientState::ORDERED:
			//temporal, no comprueba si esta el plato que quiere, solo comprueba que el jugador tenga platos
			if (inventory->freeSpace() != 0) {
				inventory->freeDish();
				clientState->getServed();
			}
			break;
		default:
			break;
		}
	}
}

int ClientTrigger::randomTable()
{
	return sdlutils().rand().nextInt(1, 3);
}

bool ClientTrigger::inputMando() {
	//if (ih->joysticksInitialised()) {
	//	if (ih->getButtonState(0, SDL_CONTROLLER_BUTTON_B)) {
	//		return true;
	//	}
	//}
	//else
	//	return false;

	return ih->joysticksInitialised() && ih->getButtonState(0, SDL_CONTROLLER_BUTTON_B);
}
