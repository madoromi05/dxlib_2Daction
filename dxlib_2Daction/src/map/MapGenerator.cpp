#include "MapGenerator.h"
#include "Character/Player/PlayerCharacter.h"
#include <stdexcept>

using CellType = map_generation::CellType;

MapGenerator::MapGenerator()
	: m_width(0), m_height(0)
	, m_startX(0), m_startY(0)
	, m_goalX(0), m_goalY(0)
{}

// -------------------------------------------------------
// 外部公開：マップ生成エントリーポイント
// -------------------------------------------------------
void MapGenerator::Generate(
	int outMapData[][stage_information::kMapWidth],
	int width,
	int height,
	unsigned int seed)
{
	m_width = width;
	m_height = height;

	// スタートを下部、ゴールを上部に配置
	m_startX = m_width / 2;
	m_startY = m_height - 3;
	m_goalX = m_width / 2;
	m_goalY = 2;

	// グリッド初期化
	m_grid.assign(m_width, std::vector<CellType>(m_height, CellType::Empty));

	std::mt19937 gen(seed);

	InitializeGrid(gen);
	RunSimulation();
	EnsurePath(gen);

	WriteToMapData(outMapData);

	// スタート情報を保存（Mapクラスから参照される）
	m_rooms.clear();
	RoomRect startRoom{ m_startX - 1, m_startY - 1, 3, 3 };
	m_rooms.push_back(startRoom);
}

// -------------------------------------------------------
// 範囲チェック
// -------------------------------------------------------
bool MapGenerator::IsOutOfBounds(int x, int y) const {
	return x < 0 || x >= m_width || y < 0 || y >= m_height;
}

// -------------------------------------------------------
// 生存近傍カウント
// -------------------------------------------------------
int MapGenerator::CountAliveNeighbours(int gridX, int gridY) const {
	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;

			int neighbourX = gridX + i;
			int neighbourY = gridY + j;

			if (IsOutOfBounds(neighbourX, neighbourY)) {
				count++; // 範囲外は壁扱い
			}
			else if (m_grid[neighbourX][neighbourY] == CellType::Wall) {
				count++;
			}
		}
	}
	return count;
}

// -------------------------------------------------------
// 次ステートの決定
// -------------------------------------------------------
CellType MapGenerator::DetermineNextState(
	CellType currentState,
	int aliveNeighbours) const
{
	if (currentState == CellType::Wall) {
		return (aliveNeighbours < 4) ? CellType::Empty : CellType::Wall;
	}
	else {
		return (aliveNeighbours > 4) ? CellType::Wall : CellType::Empty;
	}
}

// -------------------------------------------------------
// スタートとゴール周辺を空間に保護
// -------------------------------------------------------
void MapGenerator::ProtectStartAndGoal() {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!IsOutOfBounds(m_startX + i, m_startY + j))
				m_grid[m_startX + i][m_startY + j] = CellType::Empty;

			if (!IsOutOfBounds(m_goalX + i, m_goalY + j))
				m_grid[m_goalX + i][m_goalY + j] = CellType::Empty;
		}
	}
}

// -------------------------------------------------------
// スタートからゴールまで強制的に道を掘る
// -------------------------------------------------------
void MapGenerator::EnsurePath(std::mt19937& gen) {
	int currentX = m_startX;
	int currentY = m_startY;

	std::uniform_int_distribution<> distrib(0, 1);

	while (currentX != m_goalX || currentY != m_goalY) {
		// 現在地周辺3x3を空間化
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (!IsOutOfBounds(currentX + i, currentY + j))
					m_grid[currentX + i][currentY + j] = CellType::Empty;
			}
		}

		// ゴールへ1歩進む（ジグザグ）
		if (currentX != m_goalX && currentY != m_goalY) {
			if (distrib(gen) == 0) {
				currentX += (m_goalX > currentX) ? 1 : -1;
			}
			else {
				currentY += (m_goalY > currentY) ? 1 : -1;
			}
		}
		else if (currentX != m_goalX) {
			currentX += (m_goalX > currentX) ? 1 : -1;
		}
		else {
			currentY += (m_goalY > currentY) ? 1 : -1;
		}
	}

	ProtectStartAndGoal();
}

// -------------------------------------------------------
// 1ステップのシミュレーション
// -------------------------------------------------------
void MapGenerator::DoSimulationStep() {
	Grid newGrid = m_grid;

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			int aliveNeighbours = CountAliveNeighbours(x, y);
			newGrid[x][y] = DetermineNextState(m_grid[x][y], aliveNeighbours);
		}
	}
	m_grid = newGrid;
	ProtectStartAndGoal();
}

// -------------------------------------------------------
// グリッドのランダム初期化
// -------------------------------------------------------
void MapGenerator::InitializeGrid(std::mt19937& gen) {
	std::uniform_int_distribution<> distrib(0, 100);

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			if (x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1) {
				m_grid[x][y] = CellType::Wall;
			}
			else {
				int randValue = distrib(gen);
				m_grid[x][y] = (randValue < kFillProbability)
					? CellType::Wall
					: CellType::Empty;
			}
		}
	}
	ProtectStartAndGoal();
}

// -------------------------------------------------------
// シミュレーション全ステップ実行
// -------------------------------------------------------
void MapGenerator::RunSimulation() {
	for (int i = 0; i < kSimulationSteps; i++) {
		DoSimulationStep();
	}
}

// -------------------------------------------------------
// グリッドをint配列（TileLayer用）に変換して書き出す
// CellType::Wall → タイルID 0（壁）
// CellType::Empty → タイルID -1 or 空きタイルID
// -------------------------------------------------------
void MapGenerator::WriteToMapData(
	int outMapData[][stage_information::kMapWidth]) const
{
	constexpr int kWallTileId = 0;
	constexpr int kEmptyTileId = -1;

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			outMapData[y][x] =
				(m_grid[x][y] == CellType::Wall)
				? kWallTileId
				: kEmptyTileId;
		}
	}
}
