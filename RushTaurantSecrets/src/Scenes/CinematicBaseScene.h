
#pragma once
#include "../Structure/Scene.h"
#include "../Utilities/Texture.h"
#include "../Utilities/SDLUtils.h"
using namespace std;
class CinematicBaseScene : public Scene
{
protected:
	Texture* skipText;
	Font* font;
	float cont;
	const float SHOW_TEXT_DURATION = 20;
	const string FONT_PATH = "assets/Fonts/EpilepsySansBold.ttf";
	const float FONTSIZE = 50;
	const float SKIP_WIDTH = 300,SKIP_HEIGHT=50;
public:
	CinematicBaseScene();
	~CinematicBaseScene();
	void update()override;
	void render() override;
	virtual void renderSkipText();
	virtual void renderCinematic(){};
	void handleEvents()override;
	virtual void finishScene(){}; //lo que se ejecuta al terminar la escena
};