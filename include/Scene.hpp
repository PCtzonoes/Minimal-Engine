#pragma once

#include <string>

class Scene {
private:
	std::string _textureId;
	int _tileScale;
	int _tileSize;
	int _mapSizeX;
	int _mapSizeY;

public:
	Scene(std::string textureId, int scale, int tileSize);

	~Scene();

	void AddTile(int sourceX, int sourceY, int destX, int destY);

	void LoadFromFile(std::string sceneFilePath, int mapSizeX, int mapSizeY);

	[[nodiscard]] int GetMapPixelSizeX() const;

	[[nodiscard]] int GetMapPixelSizeY() const;
};