#pragma once

#include "../Utilities/Vector.h"
#include "../Utilities/SDLUtils.h"
#include <vector>

using namespace std;

namespace RelativeToGlobal {
	// Si no se a�aden casillas extras, los personajes quedan
	// dos casillas por debajo de lo previsto
	// Cada casilla extra supone que se suben 1
	
	// En este caso, hay 1 sola casilla extra a�adida respecto al tilemap
	const int tilesWRest = 40;
	const int tilesHRest = 23;

	const int tilesWPantry = 32;
	const int tilesHPantry = 18;

	// convertir un punto relativo en global
	inline static Vector point(const Vector& point, int tilesW, int tilesH) {
		int fWidth = SDLUtils::instance()->width() / tilesW;
		int fHeight = SDLUtils::instance()->height() / tilesH;
		return Vector(point.getX() * fWidth, point.getY() * fHeight);
	}

	// convertir una serie de puntos relativos en globales
	inline static vector<Vector> points(const vector<Vector>& points, int tilesW, int tilesH) {
		vector<Vector> aux;
		for (int i = 0; i < points.size(); ++i) {
			aux.push_back(point(points[i], tilesW, tilesH));
		}
		return aux;
	}

	// restaurante
	inline static Vector pointRestaurant(const Vector& _point) {
		return point(_point, tilesWRest, tilesHRest);
	}
	inline static vector<Vector> pointsRestaurant(const vector<Vector>& _points) {
		return points(_points, tilesWRest, tilesHRest);
	}

	// despensa
	inline static Vector pointPantry(const Vector& _point) {
		return point(_point, tilesWPantry, tilesHPantry);
	}
	inline static vector<Vector> pointsPantry(const vector<Vector>& _points) {
		return points(_points, tilesWPantry, tilesHPantry);
	}
};