#pragma once
#include "Types.hpp"
#include <vector>
#include <unordered_map>
#include <random>

namespace MazeGenerator {
    enum class Algorithm {
        BinaryTree,
        DepthFirstSearch
    };

    Wall::Direction getDirectionBetweenCells(int size, int idx, int otherIdx);
    bool canMove(int index, Wall::Direction direction, int size);
    void moveDirection(std::vector<Cell>& grid, int size, int index, Wall::Direction direction);
    void binaryTree(std::vector<Cell>& grid, int size);
    std::vector<int> getUnvisitedNeighbors(const std::vector<bool>& isVisited, int current_idx, int size);
    void depthFirstSearch(std::vector<Cell>& grid, int size);
    void generate(std::vector<Cell>& grid, int size, Algorithm algo = Algorithm::BinaryTree, std::mt19937 gen = std::mt19937{ std::random_device{}() });
}