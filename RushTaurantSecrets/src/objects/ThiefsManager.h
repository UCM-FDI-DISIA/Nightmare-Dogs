#pragma once

#include "../gameObjects/Thief.h"
#include "../structure/Manager.h"
#include "../structure/Scene.h"
#include "../objects/RelativeToGlobal.h"

class ThiefsManager : public Manager<ThiefsManager> {

	friend Manager<ThiefsManager>;

private:
	const int MAX_THIEFS = 2;
	Scene* scene;
	vector<GameObject*>* thiefs;
	SDLUtils* sdl;
	float generalSpeed;
	float escapeSpeed;
	bool canGetFreezer;
	float timer;
	float elapsedTime;
	int n;

	void createThief();

	ThiefsManager(GameObject* parent, float generalSpeed, float escapeSpeed, bool canGetFreezer, float frequencyThiefs, int n);

public:
	static constexpr _ecs::_cmp_id id = _ecs::cmp_THIEFS_MANAGER;

	bool isThereThieves() const {
		return !thiefs->empty();
	}

	virtual void update();
};