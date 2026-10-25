#include <queue>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include "AStar.hpp"

namespace Pathfinding {
	int calculateManhattanDistance(int idxA, int idxB, int size) {
		int Ax = idxA % size;
		int Ay = idxA / size;
		int Bx = idxB % size;
		int By = idxB / size;

		return std::abs(Ax - Bx) + std::abs(Ay - By);
	}

	std::vector<int> get_passable_neighbours(const std::vector<Cell>& grid, int current_idx, int size) {
		std::vector<int> neighbours;
		const Cell& cell = grid[current_idx];

		if (!cell.checkWall(Wall::North) && current_idx / size > 0) { neighbours.push_back(current_idx - size); }
		if (!cell.checkWall(Wall::South) && current_idx / size < size - 1) { neighbours.push_back(current_idx + size); }
		if (!cell.checkWall(Wall::East) && current_idx % size < size - 1) { neighbours.push_back(current_idx + 1); }
		if (!cell.checkWall(Wall::West) && current_idx % size > 0) { neighbours.push_back(current_idx - 1); }

		return neighbours;
	}

	std::vector<int> solveAStar(const std::vector<Cell>& grid, int size, int startIdx, int goalIdx) {
		std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
		std::unordered_map<int, int> cameFrom;
		std::unordered_map<int, int> gScore;

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

			for (int neighbour : get_passable_neighbours(grid, current.index, size)) {
				int tentative_g = gScore[current.index] + 1;

				if (gScore.find(neighbour) == gScore.end() || tentative_g < gScore[neighbour]) {
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
