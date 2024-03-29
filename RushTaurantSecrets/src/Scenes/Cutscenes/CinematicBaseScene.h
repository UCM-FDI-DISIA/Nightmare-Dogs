#pragma once

#include "../../Structure/Scene.h"
#include "../../Utilities/Texture.h"
#include "../../Utilities/SDLUtils.h"
#include "../../Definitions/Paths_def.h"
#include <vector>
using namespace std;

class Player;
class Transform;
class StraightMovement;
#include "../../Components/Animator.h"
class CharacterAnimator;

class CinematicBaseScene : public Scene {
protected:
	Font* font;
	const string FONT_PATH = "assets/Fonts/chary___.ttf";
	
	//SKIP
	Texture* skipText;
	const float SHOW_TEXT_DURATION = 5;
	float cont; //contador para skip
	const float SKIP_WIDTH = 300,SKIP_HEIGHT=50;
	const float FONTSIZE = 50;
	
	const float WIDTH, HEIGHT; //ancho y alto de pantalla
	GameObject* dialogueBox;
	SDLUtils* sdl;

	Scene* transition;

	const Animator::AnimParams ANIMATIONSPARAMS = { 18,18,1 };
	GameObject* player;
	Transform* transform;
	StraightMovement* straightMovement;
	CharacterAnimator* anim;

	bool net_active;
public:
	CinematicBaseScene();
	~CinematicBaseScene();
	void update()override;
	void render() override;
	virtual void renderUI(); //texto skip, dialog box,...
	virtual void renderCinematic(){}; //escena
	void handleEvents()override;
	virtual void finishScene(); //lo que se ejecuta al terminar la escena
	void transitionEnded() { transition = nullptr; }


	virtual void initCoopMode(bool server) { net_active = server; };
	virtual void quitCoopMode() { net_active = false; };

	virtual void receive(const Message& message);
};