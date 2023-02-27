#pragma once
#include "Animator.h"
using namespace std;
class UIAnimator :public Animator
{
protected:
	Vector pos;
	
	void init(Vector p); //no hereda del padre

public:
	constexpr static _ecs::_cmp_id id = _ecs::cmp_UIANIM;


	// cleon: 9 par�metros. MAL. MAAAAALLL. MAAAAAAAAAAAAAAAAAAAAAAAAAAAALLLLLLLLLLLLLLLLLLLLLLLL. MMMUUUYYY MAAALLLL.
	// estructuras.
	// #dato: no lo quites.
	UIAnimator(GameObject* parent, Texture* t, int iniFrame, int endFrame, int currAnim = 0, int frRate = FRAMERATE, float w = 0, float h = 0, Vector pos = (-1, -1)) :
		Animator(parent, t, iniFrame, endFrame, currAnim, frRate, w, h,id) {init(pos);}

	UIAnimator(GameObject* parent, string s, int iniFrame, int endFrame, int currAnim = 0, int frRate = FRAMERATE, float w = 0, float h = 0, Vector pos = (-1, -1)) :
		Animator(parent, s, iniFrame, endFrame, currAnim, frRate, w, h,id) {init(pos);}
	
	/*cambiar de pos*/
	void setPos(Vector pos) { this->pos = pos; }
	/*si es est��tico, usar�� siempre pos, si no, la del transform*/
	virtual void render();
};



