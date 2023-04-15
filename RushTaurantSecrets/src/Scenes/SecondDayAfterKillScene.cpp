#include "SecondDayAfterKillScene.h"
#include "MainMenu.h"
#include "../Utilities/checkML.h"
#include "BeforeDayStartScene.h"
#include "DailyMenuScene.h"

#include "ShowSkipTransitionScene.h"
#include "../Structure/GameManager.h"
#include "../GameObjects/Dialogue.h"

SecondDayAfterKillScene::SecondDayAfterKillScene() {
	dialogues = GameManager::get()->getDialogueInfo("ConversationDay4Kill.json");
	cont = 0;
	state = START;
	nightMusic = &sdlutils().musics().at("SILENT_CREEPY_MUSIC");
	nightAmbience = &sdlutils().soundEffects().at("NIGHT_AMBIENCE");
	nightAmbience->setVolume(60);
	bg = &sdlutils().images().at("CINEMATIC_BG_PARENTS_ROOM");
	top = &sdlutils().images().at("CINEMATIC_BG_PARENTS_ROOM_TOP");
	filter = &sdlutils().images().at("CINEMATIC_BG_PARENTS_ROOM_NIGHT");
	filter->setOpacity(80);

	player = new Player(this, 0);
	straightMovement = new StraightMovement(player, 5);

	transform = player->getComponent<Transform>();
	transform->setPos(RelativeToGlobal::pointRestaurant(Vector(15, 10)));
	transform->setMovState(walking);
	straightMovement->changeSpeed(2);
	player->getComponent<PlayerMovementController>()->setActive(false);

	auto anim = player->getComponent<CharacterAnimator>();
	anim->setH(96 * 1.8);
	anim->setW(48 * 1.8);
	anim->setTexture("Player_Casual", 18, 10, 1);
	anim->setframeRate(18);
	//Scene::initRender();
}

void SecondDayAfterKillScene::addPath(const vector<Vector>& points) {
	straightMovement->addPath(RelativeToGlobal::pointsRestaurant(points));
}

void SecondDayAfterKillScene::callAfterCreating() {
	GameManager::get()->changeScene(new ShowSkipTransitionScene(this, 3,false));
}

void SecondDayAfterKillScene::update() {
	CinematicBaseScene::update();
	switch (state)
	{
	case SecondDayAfterKillScene::START:
		addPath(secondDayAfterKillPath[0].points);
		nightAmbience->play(-1);
		nightMusic->play(-1);
		state = PAUSE;
		break;
	case SecondDayAfterKillScene::PAUSE:
		if (straightMovement->hasFinishedPath()) {
			transform->setOrientation(north);
			transform->setMovState(idle);
			state = START2;
		}
		break;
	case SecondDayAfterKillScene::START2:
		cont += frameTime;
		if (cont > STOP_TIME * 1000) {
			addPath(secondDayAfterKillPath[1].points);
			state = M1;
		}
		break;
	case SecondDayAfterKillScene::M1:
		if (straightMovement->hasFinishedPath()) {
			transform->setOrientation(north);
			transform->setMovState(idle);
			dialogueBox = new Dialogue(this, Vector(150, 500), 700, 0.01 * 1000,
				font, dialogues[0].portrait, dialogues[0].text);
			state = M2;
		}
		break;
	case SecondDayAfterKillScene::M2:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 430), 700, 0.01 * 1000,
				font, dialogues[1].portrait, dialogues[1].text);
			state = P1;
		}
		break;
	case SecondDayAfterKillScene::P1:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 550), 700, 0.01 * 1000,
				font, dialogues[2].portrait, dialogues[2].text);
			state = M3;
		}
		break;
	case SecondDayAfterKillScene::M3:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 500), 700, 0.01 * 1000,
				font, dialogues[3].portrait, dialogues[3].text);
			state = P2;
		}
		break;
	case SecondDayAfterKillScene::P2:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 550), 700, 0.01 * 1000,
				font, dialogues[4].portrait, dialogues[4].text);
			state = M4;
		}
		break;
	case SecondDayAfterKillScene::M4:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 500), 700, 0.01 * 1000,
				font, dialogues[5].portrait, dialogues[5].text);
			state = P3;
		}
		break;
	case SecondDayAfterKillScene::P3:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 550), 700, 0.01 * 1000,
				font, dialogues[6].portrait, dialogues[6].text);
			state = WALKING;
		}
		break;
	case SecondDayAfterKillScene::WALKING:
		if (Text::isTextFinished()) {
			straightMovement->changeSpeed(5);
			player->getComponent<CharacterAnimator>()->setframeRate(12);
			addPath(secondDayAfterKillPath[2].points);
			state = M5;
		}
		break;
	case SecondDayAfterKillScene::M5:
		if (straightMovement->hasFinishedPath()) {
			transform->setMovState(idle);
			transform->setOrientation(east);
			dialogueBox = new Dialogue(this, Vector(150, 430), 700, 0.01 * 1000,
				font, dialogues[7].portrait, dialogues[7].text);
			state = M6;
		}
		break;
	case SecondDayAfterKillScene::M6:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 500), 700, 0.01 * 1000,
				font, dialogues[8].portrait, dialogues[8].text);
			state = M7;

		}
		break;
	case SecondDayAfterKillScene::M7:
		if (Text::isTextFinished()) {
			addPath(secondDayAfterKillPath[3].points);
			state = OUT;
		}
		break;
	case SecondDayAfterKillScene::OUT:
		if (straightMovement->hasFinishedPath()) {
			dialogueBox = nullptr;
			state = NONE;
			GameManager::get()->changeScene(new TransitionScene(this, 3, true, true));
		}
		break;
	case SecondDayAfterKillScene::NONE:
		break;
	default:
		break;
	}

}

void SecondDayAfterKillScene::renderCinematic() {
	bg->render(build_sdlrect(0, 0, WIDTH, HEIGHT));
	player->render();
	top->render(build_sdlrect(0, 0, WIDTH, HEIGHT));
	filter->render(build_sdlrect(0, 0, WIDTH, HEIGHT));
}


void SecondDayAfterKillScene::finishScene() {
	nightMusic->haltMusic();
	nightAmbience->haltChannel();
	GameManager::get()->changeScene(GameManager::get()->getBeforeDayStart());

}