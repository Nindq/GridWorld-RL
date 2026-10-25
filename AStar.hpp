#pragma once
#include "Types.hpp"
#include <vector>

struct Node {
    int g{ 0 };
    int h{ 0 };
    int index{ -1 };

    int f() const { return g + h; }
    bool operator>(const Node& other) const { return f() > other.f(); }
};

namespace Pathfinding {
    int calculateManhattanDistance(int idxA, int idxB, int size);
    std::vector<int> getPassableNeighbors(const std::vector<Cell>& grid, int current_idx, int size);
    std::vector<int> solveAStar(const std::vector<Cell>& grid, int size, int startIdx, int goalIdx);
}