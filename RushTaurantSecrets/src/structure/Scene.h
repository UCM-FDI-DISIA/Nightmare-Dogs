#pragma once
#include <vector>
#include <array>
#include "Structure_def.h"
#include "../sdlutils/InputHandler.h"
#include "GameInfor.h"
class GameObject;
using namespace _ecs;
class Scene {
private:
	friend class GameObject;
	void addObject(GameObject* object, _ecs::_grp_id grp, _ecs::_hdr_id handler);
protected:
	GameInfor* Infor;
	InputHandler* ih;
	std::array<std::vector<GameObject*>, _ecs::grpNum> objGroups;
	// se utiliza para conectar objetos de una misma escena
	std::array<GameObject*, _ecs::hdrNum> handlers;
	Scene();
public:
	virtual ~Scene();
	virtual Scene* getUI() { return nullptr; };
	

	void refresh();
	inline std::vector<GameObject*>* getGroup(_ecs::_grp_id grp) { return &objGroups[grp]; }
	inline GameObject* getGameObject(_ecs::_hdr_id handler) { return handlers[handler]; }

	virtual void update();
	virtual void render();
	virtual void handleEvents();
	virtual void init() {};
	virtual void end() {};
};