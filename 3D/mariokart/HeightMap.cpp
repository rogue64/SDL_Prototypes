#include "HeightMap.h"
#include "CSVHelper.h"
#include <fstream>
HeightMap::HeightMap(const std::string filename) {
	// initialize grid with the values in the given csv file
	std::ifstream file(filename);
	std::string curr_line;
	while (std::getline(file, curr_line)) {
		std::vector<std::string> curr_split = CSVHelper::Split(curr_line);
		std::vector<int> curr_row;
		for (size_t i = 0; i < curr_split.size(); i++)
			curr_row.push_back(std::stoi(curr_split[i]));
		grid.push_back(curr_row);
	}
	file.close();
}

HeightMap::~HeightMap() {

}

Vector3 HeightMap::cellToWorld(int row, int col) {
	if (!isCellOnTrack(row, col))
		return Vector3::Zero;
	return Vector3(GRID_TOP - (row * CELL_SIZE), GRID_LEFT + (col * CELL_SIZE), getHeightFromCell(row, col));
}

Vector2 HeightMap::worldToCell(float x, float y) {
	/*float _row = GRID_TOP  - y;
	 _row = _row / CELL_SIZE;
	 int row = (int)_row;
	 float _col = x - GRID_LEFT;
	 _col = _col / CELL_SIZE;
	 int col = (int)_col;
	 return Vector2(row + 1, col + 1);*/
	float row = (Math::Abs(x - GRID_TOP) + CELL_SIZE / 2.0f) / CELL_SIZE;
	float col = (Math::Abs(y - GRID_LEFT) + CELL_SIZE / 2.0f) / CELL_SIZE;
	return Vector2((int)row, (int)col);
}

bool HeightMap::isOnTrack(float x, float y) {
	Vector2 cell = worldToCell(x, y);
	return isCellOnTrack(cell.x, cell.y);
}

float HeightMap::getHeight(float x, float y) {
	if (!isOnTrack(x, y))
		return -1000;
	Vector2 cell = worldToCell(x, y);
	return getHeightFromCell(cell.x, cell.y);
}