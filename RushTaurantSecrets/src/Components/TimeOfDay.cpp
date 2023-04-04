#include "TimeOfDay.h"

#include "../structure/GameManager.h"
#include "../Scenes/Restaurant.h"
#include "../Components/ClockComponent.h"

#include "../Utilities/SDLUtils.h"
#include "../Utilities/checkML.h"

TimeOfDay::TimeOfDay(Texture* aftTexture, Texture* nightTexture) : topTexture(nullptr),
	afternoon(aftTexture), night(nightTexture)
{
	clock = GameManager::get()->getRestaurant()->getUI()->getClock()->getComponent<ClockComponent>();
}

void TimeOfDay::update() {
	if (clock->getRotation() > 30 && clock->getRotation() < 270) 
		topTexture = afternoon;
	else if (clock->getRotation() > 270) 
		topTexture = night;
	else 
		topTexture = nullptr;
}

void TimeOfDay::render() {
	if (topTexture != nullptr) {
		topTexture->render( build_sdlrect( 0, 0, topTexture->width() * sdlutils().getResizeFactor(), topTexture->height() * sdlutils().getResizeFactor()));
	}
}