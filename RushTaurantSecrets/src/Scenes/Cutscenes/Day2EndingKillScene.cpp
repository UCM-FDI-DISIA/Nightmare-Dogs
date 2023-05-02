#include "Day2EndingKillScene.h"

#include "../Menus/MainMenu.h"
#include "../../Utilities/checkML.h"
#include "../GameScenes/BeforeDayStartScene.h"
#include "../GameScenes/DailyMenuScene.h"

#include "ShowSkipTransitionScene.h"
#include "../../Structure/GameManager.h"
#include "../../GameObjects/Dialogue.h"


void Day2EndingKillScene::addPath(const vector<Vector>& points) {
	straightMovement->addPath(RelativeToGlobal::pointsHouse(points));
}

Day2EndingKillScene::Day2EndingKillScene() {
	dialogues = GameManager::get()->getDialogueInfo("Day2EndingKill.json");

	state = START;
	bg = &sdlutils().images().at("CINEMATIC_BG_HOUSE");
	black = &sdlutils().images().at("Filter_Black");
	phonecall = &sdlutils().soundEffects().at("TELEPHONE");
}

void Day2EndingKillScene::reset() {
	dialogueBox = nullptr;

	transform->setPos(RelativeToGlobal::pointHouse({ 13, 14 }));
	transform->setMovState(walking);

	anim->setW(48);
	anim->setH(96);
	anim->setTexture("Player_Casual", 0, 0, 0, 10);

	straightMovement->stop();
	addPath(ENTERPATH);
	state = START;

	if (GameManager::instance()->getCurrentScene() == this) {
		transition = new ShowSkipTransitionScene(this, 3);
		GameManager::get()->pushScene(transition, true);
	}
}

void Day2EndingKillScene::renderCinematic()
{
	bg->render(build_sdlrect(0, 0, WIDTH, HEIGHT));
	player->render();
}

void Day2EndingKillScene::finishScene() {
	black->setOpacity(100);
	black->render(build_sdlrect(0, 0, sdlutils().width(), sdlutils().height()));
	if (transition != nullptr)
		delete transition;
	GameManager::get()->changeScene(GameManager::get()->getScene(sc_FIRSTDAYAFTERKILL), false);
}

void Day2EndingKillScene::update()
{
	CinematicBaseScene::update();
	switch (state)
	{
	case Day2EndingKillScene::START:
		//transform->setOrientation(north);
		(&sdlutils().soundEffects().at("OPEN_DOOR"))->play();
		state = D1;
		break;
	case Day2EndingKillScene::D1:
		if (straightMovement->hasFinishedPath()) {
			transform->setMovState(idle);
			dialogueBox = new Dialogue(this, Vector(150, 500), 700, 0.01 * 1000, font, dialogues[0].portrait, dialogues[0].text);
			state = D2;
		}
		break;
	case Day2EndingKillScene::D2:
		if (Text::isTextFinished()) {
			transform->setMovState(idle);
			dialogueBox = new Dialogue(this, Vector(150, 500), 700, 0.01 * 1000, font, dialogues[1].portrait, dialogues[1].text);
			state = D3;
		}
		break;
	case Day2EndingKillScene::D3:
		if (Text::isTextFinished()) {
			transform->setMovState(idle);
			dialogueBox = new Dialogue(this, Vector(150, 430), 700, 0.01 * 1000, font, dialogues[2].portrait, dialogues[2].text);
			state = PHONE;
		}
		break;
	case Day2EndingKillScene::PHONE:
		if (Text::isTextFinished()) {
			phonecall->play(-1);
			dialogueBox = new Dialogue(this, Vector(150, 570), 700, 0.01 * 1000, font, dialogues[3].portrait, dialogues[3].text);
			cont = 0;
			state = PATHING;
		}
		break; 
	case Day2EndingKillScene::PATHING:
		cont += frameTime;
		if (cont > START_TIME * 1000) {
			addPath(PHONEPATH);
			state = D4;
		}
		break;
	case Day2EndingKillScene::D4:
		if (straightMovement->hasFinishedPath()) {
			transform->setMovState(idle);
			if (Text::isTextFinished()) {
				phonecall->haltChannel();
				dialogueBox = new Dialogue(this, Vector(150, 570), 700, 0.01 * 1000, font, dialogues[4].portrait, dialogues[4].text);
				state = D5;
			}
		}
		break;
	case Day2EndingKillScene::D5:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 430), 700, 0.01 * 1000, font, dialogues[5].portrait, dialogues[5].text);
			state = D6;
		}
		break;
	case Day2EndingKillScene::D6:
		if (Text::isTextFinished()) {
			dialogueBox = new Dialogue(this, Vector(150, 500), 700, 0.01 * 1000, font, dialogues[6].portrait, dialogues[6].text);
			state = OUT;
		}
		break;
	case Day2EndingKillScene::OUT:
		if (Text::isTextFinished()) {
			dialogueBox = nullptr;
			state = NONE;
			transition = new TransitionScene(this, 3, true, true);
			GameManager::get()->pushScene(transition, true);
		}
		break;
	case Day2EndingKillScene::NONE:
		break;
	}
}
