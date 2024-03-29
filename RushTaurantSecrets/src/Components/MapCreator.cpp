#include "MapCreator.h"
#include "../Definitions/Structure_def.h"

#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/Tileset.hpp"

#include "../Utilities/SDLUtils.h"
#include "../Structure/Scene.h"
#include "../Structure/GameObject.h"
#include "../GameObjects/CollisionObject.h"
#include "../GameObjects/CookingMachine.h"
#include "../GameObjects/CashRegister.h"
#include "../GameObjects/KitchenIsland.h"
#include "../GameObjects/IslandIngredient.h"
#include "../GameObjects/Cartel.h"
#include "../GameObjects/Bin.h"
#include "../GameObjects/KitchenSurfaceObj.h"
#include "../GameObjects/goToPantryObj.h"
#include "../GameObjects/Desk.h"
#include "../GameObjects/PantryExitObj.h"
#include "../GameObjects/Freezer.h"
#include "../GameObjects/SuperCashRegister.h"
#include "../Utilities/checkML.h"
using namespace _ecs;

MapCreator::MapCreator(GameObject* parent, const string& filePath) : Component(parent, id), path(filePath), renderer(sdlutils().renderer()), RESIZEFACTOR(parent->getScene()->getResizeFactor()) {
	loadMapDims();

	SDL_RenderClear(renderer);
	SDL_SetTextureBlendMode(bg, SDL_BLENDMODE_BLEND);

	loadTilesets();
	createObject();
}

MapCreator::~MapCreator() {
	if (tileMap != nullptr) delete tileMap;
}


void MapCreator::loadMapDims() {
	// Carga el tilemap
	tileMap = new Map();
	tileMap->load(path);

	// Cuenta las filas y columnas que tiene
	auto tileCount = tileMap->getTileCount();
	rows = tileCount.y;
	cols = tileCount.x;

	// Lee el tamaño de los tiles
	auto tileSize = tileMap->getTileSize();
	tileW = tileSize.x;
	tileH = tileSize.y;

	// Calcula las dimensiones totales del mapa y asigna a la
	// textura del mapa una textura con dichas dimensiones
	int width = cols * tileW;
	int height = rows * tileH;
	bg = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

}


void MapCreator::loadTilesets() {
	// Lee los tilesets del tilemap y los recorre
	auto mapTileSets = tileMap->getTilesets();
	for (auto& ts : mapTileSets) {
		// Lee el nombre del tileset
		string tilesetName = ts.getName();
		// Tilesets precargados
		auto preloadedTiles = sdlutils().getLoadedTilesets();
		// Posición del tileset buscado en los tilesets precargados
		auto preloadedTexture = preloadedTiles.find(tilesetName);

		// Si el tileset buscado est?en los tilesets precargados,
		if (preloadedTexture != preloadedTiles.end()) {
			// Guarda la textura precargada en los tilesets del mapa,
			// siendo su key la ID del primer tile que haya en el tileset
			tilesets.insert({ ts.getFirstGID(), preloadedTexture->second });
		}
	}

}


void MapCreator::render() {
	// Lee las capas del tilemap y las recorre
	auto& layers = tileMap->getLayers();
	for (auto& ly : layers) {
		// Si el tipo de la capa es de tiles
		if (ly->getType() == Layer::Type::Tile) {
			// Se carga la capa y se leen sus tiles
			TileLayer* tileLayer = dynamic_cast<TileLayer*>(ly.get());
			auto& tiles = tileLayer->getTiles();

			bool found = false;
			// Se recorre todo el mapa 
			for (int rw = 0; rw < rows; rw++) {
				// Además, si no existe tileset para el tile, pasa a la siguiente casilla
				for (int cl = 0; cl < cols; cl++) {
					// Se obtiene la ID del tile en el mapa y se
					// usa para obtener la ID del tile en el tileset
					int tileInTilemap = cl + rw * cols;
					int tileID = tiles[tileInTilemap].ID;

					// Si la casilla no está vacía
					if (tileID != 0) {
						found = false;
						// Recorre los tilesets cargados y busca el primero que
						// el ID más cercano y <= al ID del tile
						int tilesetFile = 0, tilesetID = -1;
						for (auto ts = tilesets.begin(); ts != tilesets.end() && !found; ++ts) {
							if (ts->first <= tileID) {
								tilesetID = ts->first;
								tilesetFile++;
							}
							else found = true;
						}

						// Si se encuentra el tileset que contiene el tile
						if (tilesetID != -1) {
							// Se normaliza el índice
							tileID -= tilesetID;

							// Se calculan las dimensiones del tileset
							int tilesetW = 0, tilesetH = 0;
							SDL_QueryTexture(tilesets[tilesetID]->getTexture(), NULL, NULL, &tilesetW, &tilesetH);
							int tilesetCols = tilesetW / tileW;
							int tilesetRows = tilesetH / tileH;

							// Se calcula la zona del tileset en la que está el tile a dibujar
							int tilesetRegionX = (tileID % tilesetCols) * tileW;
							int tilesetRegionY = (tileID / tilesetCols) * tileH;


							// Se calcula la posición en la que dibujar el tile
							int tileX = cl * tileW * RESIZEFACTOR;
							int tileY = rw * tileH * RESIZEFACTOR;

							// Tile del tileset
							SDL_Rect srcRect = { tilesetRegionX, tilesetRegionY, tileW, tileH };
							// Parte del mapa en el que se va a dibujar el tile
							SDL_Rect destRect = { tileX, tileY, tileW * RESIZEFACTOR, tileH * RESIZEFACTOR };

							// Dibuja el el tile del tileset (tilesets[tilesetID) srcRect en la posición destRect
							tilesets[tilesetID]->render(srcRect, destRect);

						}
					}

				}
			}
		}
	}
	SDL_SetRenderTarget(renderer, nullptr);
};


void MapCreator::createObject() {
	//layers
	Scene* scene = parent->getScene();
	auto& layers = tileMap->getLayers();
	for (auto& layer : layers) {
		if (layer->getType() == Layer::Type::Object) {		
			auto& objs = static_cast<tmx::ObjectGroup*>(layer.get())->getObjects();
			for (auto& obj : objs) {
				auto& aabb = obj.getAABB();
				string name = obj.getName();
				vector<tmx::Property> p = obj.getProperties();
				Vector pos = { aabb.left * RESIZEFACTOR, aabb.top * RESIZEFACTOR };
				float width_ = aabb.width * RESIZEFACTOR;
				float height_ = aabb.height * RESIZEFACTOR;
				if (name == "") {
					new CollisionObject(scene, pos,width_,height_);
				}
				else if (name == "CookingMachine") {
					new CookingMachine(scene, pos, width_, height_,p[0].getIntValue());
				}
				else if (name == "Bin") {
					new Bin(scene, pos, width_, height_);
				}
				else if (name == "CashRegister") {
					new CashRegister(scene, pos, width_, height_);
				}
				else if (name == "Cartel") {
					Vector aux = { p[3].getFloatValue() * RESIZEFACTOR,p[4].getFloatValue() * RESIZEFACTOR };
					new Cartel2(scene, (_ecs::_ingredients_id)p[1].getIntValue(), pos, width_, height_,aux , p[2].getFloatValue(), p[0].getFloatValue());
				}else if (name == "Cartel1") {
					Vector aux = { p[3].getFloatValue() * RESIZEFACTOR,p[4].getFloatValue() * RESIZEFACTOR };
					new Cartel1(scene, (_ecs::_ingredients_id)p[1].getIntValue(), pos, width_, height_,aux , p[2].getFloatValue(), p[0].getFloatValue());
				}else if (name == "Cartel3") {
					Vector aux = { p[3].getFloatValue() * RESIZEFACTOR,p[4].getFloatValue() * RESIZEFACTOR };
					new Cartel3(scene, (_ecs::_ingredients_id)p[1].getIntValue(), pos, width_, height_,aux , p[2].getFloatValue(), p[0].getFloatValue());
				}
				else if (name == "KitchenIsland") {
					KitchenIsland* kitchenIsland = new KitchenIsland(scene, pos, width_, height_);
					GameManager::get()->setKichenIsland(kitchenIsland->getComponent<KitchenIslandComp>());
				}
				else if (name == "KitchenIslandTri") {
					new IslandIngredient(scene, pos, width_, height_,p[0].getIntValue(), p[1].getIntValue());
				}
				else if (name == "kitchen") {
					new KitchenSurfaceObj(scene, pos, width_, height_);
				}
				else if (name == "goToPantry") {
					new GoToPantryObj(scene, pos, width_, height_);

				} else if(name == "table") {
					new Desk(scene, pos, width_, height_, p[0].getIntValue());
				}
				else if (name == "pantryExit") {
					new PantryExitObj(scene, pos, width_, height_);
				}
				else if (name == "Freezer") {
					new Freezer(scene, pos, width_, height_);
				}
				else if (name == "Caja"){
					new SuperCashRegister(scene, pos, width_, height_);
				}
			}

		}
	}
}
