#pragma once
#pragma once
#include <vector>
#include <random>
#include "StageInformation.h"

// セルオートマトン方式でマップを生成するクラス
namespace map_generation {

	enum class CellType {
		Empty = 0,
		Wall = 1
	};

} // namespace map_generation

struct RoomRect {
	int x, y, w, h;
};

class MapGenerator {
public:
	MapGenerator();

	// マップを生成してoutMapDataに書き込む
	void Generate(
		int outMapData[][stage_information::kMapWidth],
		int width,
		int height,
		unsigned int seed
	);

	int GetStartX() const { return m_startX; }
	int GetStartY() const { return m_startY; }
	const std::vector<RoomRect>& GetRooms() const { return m_rooms; }

private:
	// --- セルオートマトン設定 ---
	static constexpr int kFillProbability = 45;
	static constexpr int kSimulationSteps = 4;

	int m_width;
	int m_height;
	int m_startX;
	int m_startY;
	int m_goalX;
	int m_goalY;
	std::vector<RoomRect> m_rooms;

	using Grid = std::vector<std::vector<map_generation::CellType>>;
	Grid m_grid;

	bool IsOutOfBounds(int x, int y) const;
	int  CountAliveNeighbours(int gridX, int gridY) const;
	map_generation::CellType DetermineNextState(
		map_generation::CellType currentState,
		int aliveNeighbours
	) const;
	void ProtectStartAndGoal();
	void EnsurePath(std::mt19937& gen);
	void DoSimulationStep();
	void InitializeGrid(std::mt19937& gen);
	void RunSimulation();

	// 生成結果をint配列に書き出す
	void WriteToMapData(int outMapData[][stage_information::kMapWidth]) const;
};
