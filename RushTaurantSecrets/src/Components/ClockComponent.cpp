#include "ClockComponent.h"
#include "../Utilities/checkML.h"

ClockComponent::ClockComponent(GameObject* parent) :
	Manager(parent), sdl(SDLUtils::instance()), elapsedTime(0), numFullClock(0) {

	fillData(clock, "CLOCK", Vector(sdl->width() - ICONX - ICONSIZE * 2, ICONY), ICONSIZE * 2, ICONSIZE * 2);
	fillData(arrow, "ARROW", Vector(sdl->width() - ICONX - ICONSIZE - 8, ICONY), ICONSIZE / 3, ICONSIZE);
}

void ClockComponent::updateClock() {
	elapsedTime += deltaTime;
	if (elapsedTime > TIME_CLOCK_REFRESH) {
		elapsedTime = 0;

		arrow.rotation = arrow.rotation + ANGLE_UPDATE;
		// se aumenta una vuelta
		int rot = arrow.rotation;
		if (rot % 360 == 0) {
			numFullClock++;
			
		}
	}
}

void ClockComponent::fillData(Data& data, string textureName, Vector position, float width, float height, float rotation) {
	data.texture = &sdl->images().at(textureName);
	data.position = position;
	data.width = width;
	data.height = height;
	data.rotation = rotation;
}

void ClockComponent::renderData(const Data& data) const {
	SDL_Rect dest;
	dest.x = data.position.getX();
	dest.y = data.position.getY();
	dest.w = data.width;
	dest.h = data.height;
	data.texture->render(dest, data.rotation);
}

void ClockComponent::update() {
	if (!dayHasFinished())
		updateClock();
}

void ClockComponent::render() {
	renderData(clock);
	renderData(arrow);
}

void ClockComponent::nextDay() {
	numFullClock = 0;
	elapsedTime = 0;
	fillData(clock, "CLOCK", Vector(sdl->width() - ICONX - ICONSIZE * 2, ICONY), ICONSIZE * 2, ICONSIZE * 2);
	fillData(arrow, "ARROW", Vector(sdl->width() - ICONX - ICONSIZE - 8, ICONY), ICONSIZE / 3, ICONSIZE * 2);
}

int ClockComponent::getRotation() { return arrow.rotation; }

int ClockComponent::timeToReachDeg(int deg) { return ((deg - ANGLE) / ANGLE_UPDATE) * TIME_CLOCK_REFRESH; }
