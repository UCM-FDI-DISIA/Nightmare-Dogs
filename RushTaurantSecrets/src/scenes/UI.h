#pragma once
#include "../structure/Scene.h"

// clase UI de la que heredan el resto de clases UI (UI del restaurante, del supermercado...)
// es una escena que se superpone a la escena que est� presente en el momento
class UI : public Scene
{
private:
	// iconos en com�n que aparecen en todas las UIs
	// TO-DO: dinero del jugador, icono del men� de pausa, men�

public:
	UI() : Scene() {};
};

