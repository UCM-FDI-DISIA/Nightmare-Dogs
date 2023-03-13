#pragma once

#include "../structure/Component.h"
#include "../components/Image.h"
#include "../utilities/Vector.h"
#include "../sdlutils/SDLUtils.h"
#include "../structure/GameInfor.h"

class FreezerComp :public Component {
public:

	constexpr static _ecs::_cmp_id id = _ecs::cmp_FREEZER;

private:
	struct FreezerTex {
		Texture* open1 = nullptr;
		Texture* close2 = nullptr;
		Texture* open2 = nullptr;
	};

	Image* image; 
	FreezerTex textures;
	GameInfor* gameInfo;

	void isClosedWhenKilled();
	void isOpenWhenKilled();
public:
	FreezerComp(GameObject* parent);
	void isOpen();
};

