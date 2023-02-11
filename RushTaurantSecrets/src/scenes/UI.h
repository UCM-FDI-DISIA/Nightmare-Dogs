#pragma once
#include "../structure/Scene.h"
#include "../objects/Icon.h"
#include "../sdlutils/Texture.h"

class Icon;

// clase UI de la que heredan el resto de clases UI (UI del restaurante, del supermercado...)
// es una escena que se superpone a la escena que est� presente en el momento
class UI : public Scene
{
private:
	// iconos en com�n que aparecen en todas las UIs
	// TO-DO: dinero del jugador, icono del men� de pausa, men�
	int money; // dinero del jugador
	Texture* texturesUI[50]; // CAMBIAR TAMA�O

public:
	UI();
};

