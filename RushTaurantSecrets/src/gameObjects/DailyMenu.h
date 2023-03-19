#pragma once
#include "../structure/GameObject.h"
#include "../components/Transform.h"
#include "../components/CartelTrigger.h"
#include "../components/Image.h"
#include "../components/DailyMenuComp.h"
#include "../components/ButtonComp.h"
#include "../structure/Food_def.h"
#include <string>

class Scene;
class Vector;

using namespace std;

class DailyMenu : public GameObject
{
public:
	DailyMenu(Scene* scene, string t, Vector pos, float w, float h, std::function<void()>&& callback)
		: GameObject(scene, _ecs::grp_HUD) {
		new Transform(this, pos, Vector(0, 0), w, h);
		new Image(this, &((*SDLUtils::instance()).images().at(t)));
		new ButtonComp(this, "DAILY_MENU_HIGHLIGHT", callback);
		new DailyMenuComp(this, 479, 640, _ecs::cmp_DAILYMENU);
	};

	DailyMenu(Scene* scene, string t, Vector pos, float w, float h, vector<_ecs::DishInfo>* _menu, std::function<void()>&& callback)
		: GameObject(scene, _ecs::grp_HUD) {
		new Transform(this, pos, Vector(0, 0), w, h);
		new Image(this, &((*SDLUtils::instance()).images().at(t)));
		new ButtonComp(this, "DAILY_MENU_HIGHLIGHT", callback);
		new DailyMenuComp(this, 479, 640, _ecs::cmp_DAILYMENU, _menu);
	};
};

