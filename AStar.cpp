#include <queue>
#include <algorithm>
#include <cmath>
#include <array>
#include "AStar.hpp"

namespace Pathfinding {
	int calculateManhattanDistance(int idxA, int idxB, int size) {
		int Ax = idxA % size;
		int Ay = idxA / size;
		int Bx = idxB % size;
		int By = idxB / size;

		return std::abs(Ax - Bx) + std::abs(Ay - By);
	}

	std::array<int, 4> getPassableNeighbors(const std::vector<Cell>& grid, int current_idx, int size) {
		std::array<int, 4> neighbours{ -1, -1, -1, -1 };
		const Cell& cell = grid[current_idx];

		if (!cell.checkWall(Wall::North) && current_idx / size > 0) { neighbours[0] = current_idx - size; }
		if (!cell.checkWall(Wall::South) && current_idx / size < size - 1) { neighbours[1] = current_idx + size; }
		if (!cell.checkWall(Wall::East) && current_idx % size < size - 1) { neighbours[2] = current_idx + 1; }
		if (!cell.checkWall(Wall::West) && current_idx % size > 0) { neighbours[3] = current_idx - 1; }

		return neighbours;
	}

	std::vector<int> solveAStar(const std::vector<Cell>& grid, int size, int startIdx, int goalIdx) {
		std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
		std::vector<int> cameFrom(size * size, -1);
		std::vector<int> gScore(size * size, -1);

		int h_start = calculateManhattanDistance(startIdx, goalIdx, size);
		gScore[startIdx] = 0;
		openSet.push({ 0, h_start, startIdx });

		while (!openSet.empty()) {
			Node current = openSet.top();
			openSet.pop();

			if (current.index == goalIdx) {
				std::vector<int> path;
				
				int curr = goalIdx;

				while (curr != startIdx) {
					path.push_back(curr);
					curr = cameFrom[curr];
				}

				path.push_back(startIdx);
				std::reverse(path.begin(), path.end());
				return path;
			}

			for (int neighbour : getPassableNeighbors(grid, current.index, size)) {
				if (neighbour == -1) continue;
				int tentative_g = gScore[current.index] + 1;

				if (gScore[neighbour] == -1 || tentative_g < gScore[neighbour]) {
					cameFrom[neighbour] = current.index;
					gScore[neighbour] = tentative_g;

					int h = calculateManhattanDistance(neighbour, goalIdx, size);
					openSet.push({ tentative_g, h, neighbour });
				}
			}
		}

		return {};
	}
}
