#pragma once
#include "../Utilities/SDLUtils.h"
#include <string>
#include "../Utilities/Vector.h"
#include "Transform.h"
#include "Image.h"
#include "../Structure/Manager.h"

using namespace std;
using namespace _ecs;

struct Data {
	Texture* texture;
	Vector position;
	float width;
	float height;
	float rotation;
};

class ClockComponent : public Manager<ClockComponent> {
	friend Manager<ClockComponent>;
private:
	// parametrizable: 1000 (cada segundo) / 10000 (cada diez segundos)
	const int TIME_CLOCK_REFRESH = 1000, ANGLE = 90, ANGLE_UPDATE = 5;
	const int ICONX = 20, ICONY = 15, ICONSIZE = 48;
	const int MAX_LAPS = 1;

	SDLUtils* sdl;
	float elapsedTime;
	int numFullClock; // número de vueltas que ha dado el reloj
	Data clock;
	Data arrow;

	void fillData(Data& data, string textureName, Vector position, float width, float height, float rotation = 0);
	void renderData(const Data& data) const;

	ClockComponent(GameObject* parent);
public:
	static constexpr _ecs::_cmp_id id = _ecs::cmp_MANAGER;

	// devuelve cuántas vueltas ha dado reloj cuando se llama al método
	inline int getNumFullClock() const {
		return numFullClock;
	}
	inline bool dayHasFinished() const {
		return numFullClock >= MAX_LAPS;
	}
	void updateClock();

	virtual void update();

	virtual void render();

	void nextDay();

	int getRotation() { return arrow.rotation; }
	int timeToReachDeg(int deg) { return ((deg - ANGLE) / ANGLE_UPDATE) * TIME_CLOCK_REFRESH; }
};