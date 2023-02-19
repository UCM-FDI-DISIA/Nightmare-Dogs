#include "Animator.h"

Animator::Animator(GameObject* parent, Texture* t, int iniFrame, int endFrame, int currAnim, int frRate, float w, float h) : Component(parent, id), sdl(SDLUtils::instance()) {
	texture = t;
	init(iniFrame, endFrame, currAnim, frRate, w, h);
};

Animator::Animator(GameObject* parent, string s, int iniFrame, int endFrame, int currAnim, int frRate, float w, float h) : Component(parent, id), sdl(SDLUtils::instance()) {
	texture = &((*sdl).images().at(s));
	init(iniFrame, endFrame, currAnim, frRate, w, h);
};

void Animator::init(int iniFrame, int endFrame, int currAnim, int frRate, float w, float h) {
	setCurrentAnim(iniFrame, endFrame, currAnim);
	lastTic = sdlutils().currRealTime();
	frameRate = 100;
	plTf = parent->getComponent<Transform>();
	plMov = parent->getComponent<PlayerMovementController>();
	parentOrientation = plTf->getOrientation();
	currOrientation = parentOrientation;
	resizeFactor = sdlutils().getResizeFactor();

	if (w == 0 && h == 0) { //si no se define w y h, se coge del transform
		width = plTf->getW();
		height = plTf->getH();
	}
	else { //si se define w y h
		width = w;
		height = h;
	}
};

/*mover al siguiente frame*/
void Animator::updateAnim() {
	++currFrame;

	if (currFrame >= endFrame)
		currFrame = initFrame;
}
/*cambiar de frame*/
void Animator::setCurrentAnim(int iniFram, int endFram, int currAnim)
{
	currentAnim = currAnim;
	endFrame = endFram;
	initFrame = iniFram;
	currFrame = iniFram;
}
/*cambiar de textura dada la textura*/
void Animator::setTexture(Texture* t, int iniFrame, int endFrame, int currAnim, int frRate) {
	texture = t;
	setCurrentAnim(iniFrame, endFrame, currAnim);
}
/*cambiar de textura dado el nombre de textura*/
void Animator::setTexture(string s, int iniFrame, int endFrame, int currAnim, int frRate) {
	texture = &((*sdl).images().at(s));
	setCurrentAnim(iniFrame, endFrame, currAnim);
}

void Animator::setAble(bool b) {
	able = b;
}
void Animator::update() {
	if (!plTf->isStatic()) {
		if (plTf->getMovState() == idle) currentAnim = 1;
		else if (plTf->getMovState() == walking) currentAnim = 2;
		else if (plTf->getMovState() == sitting) currentAnim = 4;
	}

	parentOrientation = plTf->getOrientation();
	if (currOrientation != parentOrientation) {
		currOrientation = parentOrientation;
		if (currOrientation == east) {
			setCurrentAnim(0, 6, currentAnim);
		}
		else if (plTf->getOrientation() == north) {
			setCurrentAnim(6, 12, currentAnim);
		}
		else if (plTf->getOrientation() == west) {
			// las animaciones de sentado mirando hacia la izquierda est�n en otros frames
			if (currentAnim == 4) {
				setCurrentAnim(6, 12, currentAnim);
			}
			else {
				setCurrentAnim(12, 18, currentAnim);
			}
		}
		else if (plTf->getOrientation() == south) {
			setCurrentAnim(18, 24, currentAnim);
		}
	}

	if (sdlutils().currRealTime() - lastTic > frameRate) {
		lastTic = sdlutils().currRealTime();
		updateAnim();
	}
}

void Animator::render() {
	SDL_Rect temp;
	temp.x = plTf->getPos().getX();
	temp.y = plTf->getPos().getY();
	temp.w = width * resizeFactor;
	temp.h = height * resizeFactor;
	// indicas la columna y la fila del frame del spritesheet que quieres que se renderice
	texture->renderFrame(temp, currFrame, currentAnim);
};