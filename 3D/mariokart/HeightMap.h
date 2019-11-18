#pragma once
#include <string>
#include <vector>
#include "Math.h"
class HeightMap {
public:
	// member funcs
	HeightMap(const std::string filename);
	~HeightMap();
	Vector3 cellToWorld(int x, int y);
	Vector2 worldToCell(float x, float y);
	bool isOnTrack(float x, float y);
	float getHeight(float x, float y);
protected:
	// constants
	const float CELL_SIZE = 40.55f;
	const float GRID_TOP = 1280.f;
	const float GRID_LEFT = -1641.f;

	// member vars
	std::vector<std::vector<int>> grid;

	// member funcs
	inline bool isCellOnTrack(int row, int col) {
		if (row > 63 || row < 0 || col > 63 || col < 0 || grid[row][col] == -1)
			return false;
		return true;
	}

	inline int getHeightFromCell(int row, int col) {
		return grid[row][col];
	}
};

