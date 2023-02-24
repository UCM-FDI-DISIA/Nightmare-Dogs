#include "ClientStateRender.h"
#include "../structure/GameObject.h"
#include "ClientState.h"

#include "../utils/checkML.h"

ClientStateRender::ClientStateRender(GameObject* _parent) :Component(_parent, id),
transform(parent->getComponent<Transform>()), anim(parent->getComponent<UIAnimator>()),
state(nullptr), sdl(SDLUtils::instance())
{
	textures.bubble = &((*sdl).images().at("BUBBLE_ICON"));
	textures.takingNote = &((*sdl).images().at("CLIENT_TAKINGNOTE_ICON_ANIM"));
	textures.thinking = &((*sdl).images().at("CLIENT_THINKING_ICON_ANIM"));

	anim->setActive(false); //al principio no tiene ning��n UIAnim
};


void ClientStateRender::clientStateIsReady() {
	state = parent->getComponent<ClientState>();
}

void ClientStateRender::renderThinkingState() {
	if(!anim->isActive())
		anim->setActive(true);
	anim->setTexture(textures.thinking,0,2,0,800);
	anim->setH(TAKINGNOTE_WIDTH);
}

void ClientStateRender::renderTakingNoteState() {
	if (!anim->isActive())
		anim->setActive(true);
	anim->setTexture(textures.takingNote , 0, 2,0,100);
	anim->setH(TAKINGNOTE_WIDTH);
}

void ClientStateRender::renderOrderingState() {
	if (anim->isActive())
		anim->setActive(false); 
	textures.dish = &((*sdl).images().at(to_string(state->getOrderedDish())));
}

void ClientStateRender::renderEatingState() {
	if (anim->isActive())
		anim->setActive(false);
	//falta anim
}

void ClientStateRender::render() {

	switch (state->getState())
	{
	case ClientState::ORDERED: //esperando el plato
		SDL_Rect dest;
		dest.x = transform->getPos().getX() + BUBBLE_OFFSETX;
		dest.y = transform->getPos().getY() + BUBBLE_OFFSETY;
		dest.w = BUBBLE_WIDTH;
		dest.h = BUBBLE_HEIGHT;
		textures.bubble->render(dest);
		//dish
		if (textures.dish != nullptr) {
			dest.x = (dest.x + BUBBLE_WIDTH / 2) - DISH_WIDTH / 2 + DISH_OFFSETX;
			dest.y = dest.y + BUBBLE_HEIGHT / 2 - DISH_HEIGHT / 2 + DISH_OFFSETY;
			dest.w = DISH_WIDTH;
			dest.h = DISH_HEIGHT;
			textures.dish->render(dest);
		}

		break;
	}
}