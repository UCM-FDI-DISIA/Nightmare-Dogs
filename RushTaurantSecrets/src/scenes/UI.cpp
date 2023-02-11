#include "UI.h"

UI::UI() : Scene() {
	// se a�aden los iconos a la lista de gameObjects
	// CAMBIAR N�MEROS DE ARRAYS DE TEXTURAS
	// icono de dinero
	objects.push_back(new Icon(this, 10, 50, texturesUI[0]));
	// icono del men� de pausa
	// CAMBIAR (poner ancho de la ventana - 50)
	objects.push_back(new Icon(this, 100, 10, texturesUI[1]));
	// icono del men� del d�a
	// CAMBIAR (poner ancho de la ventana - 50 y alto de la ventana - 50)
	objects.push_back(new Icon(this, 10, 10, texturesUI[2]));

	// cambiar por un getter para obtener el dinero
	money = 0;
}