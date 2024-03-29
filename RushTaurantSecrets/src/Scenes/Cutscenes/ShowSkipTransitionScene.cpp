#include "ShowSkipTransitionScene.h"
#include "../../Utilities/InputHandler.h"


void ShowSkipTransitionScene::render() {
	cinematicScene->renderCinematic();
	filter->render(build_sdlrect(0, 0, WIDTH, HEIGHT));
	cinematicScene->renderUI();
}


void ShowSkipTransitionScene::update() {
	cont += frameTime;
	cinematicScene->update();
	filter->setOpacity(abs(fadeOutInverter - (cont * 100 / duration)));
	if (cont > duration) {
		GameManager::get()->popScene(this, cinematicScene);
	}
}

void ShowSkipTransitionScene::handleEvents() {
	if (ih->joysticksInitialised() && ih->getButtonState(0, SDL_CONTROLLER_BUTTON_Y) || !ih->joysticksInitialised() && ih->isKeyDown(SDLK_ESCAPE))
	{
		GameManager::get()->skipfromTransition();

	}
}