#include "CookingMachineComp.h"
#include "../Structure/GameObject.h"

#include "../Utilities/checkML.h"

CookingMachineComp::CookingMachineComp(GameObject* _parent,int num_) : Component(_parent, id),num(num_),
state(available), dish(_ecs::_dish_id::NONE_DISH), cont(0), cookingTime(0), transform(parent->getComponent<Transform>()),
anim(parent->getComponent<UIAnimator>()), dishComb(DishCombinator::instance()), sdl(SDLUtils::instance()),
cookSound(&sdl->soundEffects().at("COOK")),
cookedSound(&sdl->soundEffects().at("COOKED")),
cannotCookSound(&sdl->soundEffects().at("CANNOT_COOK")),
pickDishSound(&sdl->soundEffects().at("PICK_DISH"))
{
	textures.bubble = &((*sdl).images().at("BUBBLE_ICON"));
	textures.cross = &((*sdl).images().at("CROSS"));

	renderPos = transform->getPos();
	anim->setActive(false);
}

pair<_ecs::_dish_id, bool> CookingMachineComp::canFormDish(vector<_ecs::_ingredients_id> ing) {
	return dishComb->existDish(ing);
}

void CookingMachineComp::informCannotCook() {
	cookingTime = CROSS_TIME;
	cannotCookSound->play();
	state = informing;
	cont = 0;
}

void CookingMachineComp::cook(_ecs::_dish_id d) {
	cookSound->play();
	dish = d;
	cookingTime = _ecs::Dishes[d].cookingTime;
	cont = 0;
	state = cooking;
	anim->setActive(true);
	anim->setTexture("KITCHEN_PROCESSING_ICON_ANIM", 0, 8, 0);
	anim->changeFrameRate((cookingTime / anim->getFramesNumber()) * 100);
}

void CookingMachineComp::finishCooking() {
	cookedSound->play();
	state = finished;
	anim->setActive(false);
	textures.dishTex = &((*sdl).images().at(to_string(dish)));
}

_ecs::_dish_id CookingMachineComp::pickDish() {
	_ecs::_dish_id dish_aux = dish;
	state = available;
	dish = _ecs::NONE_DISH;
	pickDishSound->play();
	return dish_aux;
}

void CookingMachineComp::update() {
	if (state == cooking && cont >= cookingTime * 100) {
		finishCooking();
	}
	else if (state == informing && cont >= cookingTime * 100) {
		state = available;
	}
	cont += deltaTime / 10;
}

void CookingMachineComp::render() {

	switch (state)
	{
	case CookingMachineComp::available: //de momento no hace nada
		break;
	case CookingMachineComp::cooking: //puntos suspensivos
		break;
	case CookingMachineComp::informing: //cruz
		textures.cross->render(build_sdlrect(renderPos.getX(), renderPos.getY() + BUBBLE_OFFSETY, CROSS_WIDTH, CROSS_HEIGHT));
		break;
	case CookingMachineComp::finished:
		SDL_Rect dest;
		dest.x = renderPos.getX() + BUBBLE_OFFSETX;
		dest.y = renderPos.getY() + BUBBLE_OFFSETY;
		dest.w = BUBBLE_WIDTH;
		dest.h = BUBBLE_HEIGHT;
		textures.bubble->render(dest);
		//dish
		if (dish != _ecs::NONE_DISH) {
			dest.x = (dest.x + BUBBLE_WIDTH / 2) - DISH_WIDTH / 2 + DISH_OFFSETX;
			dest.y = dest.y + BUBBLE_HEIGHT / 2 - DISH_HEIGHT / 2 + DISH_OFFSETY;
			dest.w = DISH_WIDTH;
			dest.h = DISH_HEIGHT;
			textures.dishTex->render(dest);
		}
		break;
	}
}

void CookingMachineComp::nextDay() {
	state = available;
	dish = _ecs::NONE_DISH;
}

void CookingMachineComp::receive(const Message& message) {
	if (message.id == Message::msg_COOKING_DISH && message.cooking_machine.id == num) {
		cook((_ecs::_dish_id)message.cooking_machine.dish);
	}
	else if (message.id == Message::msg_PICK_DISH && message.cooking_machine.id == num) {
		pickDish();
	}
}