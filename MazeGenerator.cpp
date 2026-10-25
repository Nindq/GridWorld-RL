#include "MazeGenerator.hpp"
#include <random>
#include <stack>
#include <unordered_map>

namespace MazeGenerator {

    Wall::Direction getDirectionBetweenCells(int size, int idx, int otherIdx) {
        Wall::Direction dir = Wall::North;

        if (idx + size == otherIdx) { dir = Wall::South; }
        if (idx + 1 == otherIdx) { dir = Wall::East; }
        if (idx - 1 == otherIdx) { dir = Wall::West; }

        return dir;
    }

    bool canMove(int index, Wall::Direction direction, int size) {
        int x = index % size;
        int y = index / size;

        switch (direction) {
        case Wall::North: return y > 0;
        case Wall::South: return y < size - 1;
        case Wall::East:  return x < size - 1;
        case Wall::West:  return x > 0;
        }
        return false;
    }

    void moveDirection(std::vector<Cell>& grid, int size, int index, Wall::Direction direction) {
        if (!canMove(index, direction, size)) return;
        grid[index].removeWall(direction);

        int neighbour = index;
        switch (direction) {
        case Wall::North: neighbour = index - size; break;
        case Wall::South: neighbour = index + size; break;
        case Wall::East:  neighbour = index + 1;    break;
        case Wall::West:  neighbour = index - 1;    break;
        }

        if (neighbour >= 0 && neighbour < size * size) {
            grid[neighbour].removeWall(Wall::opposite(direction));
        }
    }

    void binaryTree(std::vector<Cell>& grid, int size, std::mt19937 gen) {
        std::uniform_int_distribution<int> distrib(1, 2);

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                int current_idx = x + y * size;
                if (y == 0 && x == size - 1) continue;
                if (y == 0) { moveDirection(grid, size, current_idx, Wall::East); continue; }
                if (x == size - 1) { moveDirection(grid, size, current_idx, Wall::North); continue; }

                if (distrib(gen) == 1) {
                    moveDirection(grid, size, current_idx, Wall::North);
                }
                else {
                    moveDirection(grid, size, current_idx, Wall::East);
                }
            }
        }
    }

    std::vector<int> getUnvisitedNeighbors(const std::vector<bool>& isVisited, int current_idx, int size) {
        std::vector<int> neighbours;

        if (current_idx / size > 0 && isVisited[current_idx - size] == false) { neighbours.push_back(current_idx - size); }
        if (current_idx / size < size - 1 && isVisited[current_idx + size] == false) { neighbours.push_back(current_idx + size); }
        if (current_idx % size < size - 1 && isVisited[current_idx + 1] == false) { neighbours.push_back(current_idx + 1); }
        if (current_idx % size > 0 && isVisited[current_idx - 1] == false) { neighbours.push_back(current_idx - 1); }

        return neighbours;
    }

    void depthFirstSearch(std::vector<Cell>& grid, int size, std::mt19937 gen) {
        std::uniform_int_distribution<int> distriball(0, (size * size) - 1);
       

        int startIdx = distriball(gen);

        std::stack<int, std::vector<int>> historyStack;
        std::vector<bool> isVisited(size * size, false);

        isVisited[startIdx] = true;
        historyStack.push(startIdx);

        while (!historyStack.empty()) {
            int currentIdx = historyStack.top();
            

            std::vector<int> unVisitied = getUnvisitedNeighbors(isVisited, currentIdx, size);
            if (unVisitied.empty()) {
                historyStack.pop();
                continue;
            }
            
            int nextIdx{};
            size_t amountUnvisited = unVisitied.size();

            if (amountUnvisited == 1) { 
                nextIdx = unVisitied[0]; 
            }
            else {
                std::uniform_int_distribution<int> distribUnVisited(0, int(amountUnvisited) - 1);
                nextIdx = unVisitied[distribUnVisited(gen)];
            }
            
            isVisited[nextIdx] = true;
            moveDirection(grid, size, currentIdx, getDirectionBetweenCells(size, currentIdx, nextIdx));
            historyStack.push(nextIdx);
        }
    }

    void generate(std::vector<Cell>& grid, int size, Algorithm algo, std::mt19937 gen) {
        if (algo == Algorithm::BinaryTree) { binaryTree(grid, size, gen); }
        if (algo == Algorithm::DepthFirstSearch) { depthFirstSearch(grid, size, gen); }
    }
}