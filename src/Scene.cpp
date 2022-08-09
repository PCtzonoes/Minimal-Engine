#include <fstream>
#include "Scene.hpp"
#include "Game.hpp"
#include "Components/TileComponent.hpp"

extern GameObjectManager Manager;

Scene::Scene(std::string textureId, int tileScale, int tileSize) {
	_textureId = textureId;
	_tileScale = tileScale;
	_tileSize = tileSize;
}

Scene::~Scene() {
}

void Scene::AddTile(int sourceX, int sourceY, int destX, int destY) {
	GameObject &newTile = Manager.AddGameObject("Tile", LayerType::Tile);
	newTile.AddComponent<TileComponent>(sourceX, sourceY, destX, destY, _tileSize, _tileScale,
	                                    _textureId);
}

void Scene::LoadFromFile(std::string sceneFilePath, int mapSizeX, int mapSizeY) {
	_mapSizeX = mapSizeX;
	_mapSizeY = mapSizeY;
	std::fstream sceneFile;
	sceneFile.open(sceneFilePath);
	for (int y = 0; y < mapSizeY; ++y) {
		for (int x = 0; x < mapSizeX; ++x) {
			char ch;
			sceneFile.get(ch); // reads Y
			int sourceY = atoi(&ch) * _tileSize;
			sceneFile.get(ch); // reads X
			int sourceX = atoi(&ch) * _tileSize;
			sceneFile.ignore(); // ignores next character (',', ' ', '\n', etc..)

			AddTile(sourceX, sourceY, x * _tileSize * _tileScale, y * _tileSize * _tileScale);
		}
	}
	sceneFile.close();
}

int Scene::GetMapPixelSizeX() const {
	return _tileScale * _tileSize * _mapSizeX;
}

int Scene::GetMapPixelSizeY() const {
	return _tileScale * _tileSize * _mapSizeY;
}