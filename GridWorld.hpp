#pragma once
#include "Types.hpp"
#include "MazeGenerator.hpp"
#include <vector>
#include <random>

class Gridworld {
private:
    std::mt19937 gen;
public:
    int size{};
    int goal{};
    Point pos{};
    rewardList rewards{};
    std::vector<Cell> Grid;

    // Optional: Keep specific point/int goal constructors if needed
    Gridworld(int size);
    Gridworld(int size, unsigned int seed);
    Gridworld(int size, Point goal, unsigned int seed);
    Gridworld(int size, int goal, unsigned int seed);

    int get_index() const { return pos.x + pos.y * size; }
    int get_index(int _x, int _y) const { return _x + _y * size; }

    void seed(unsigned int s) { gen.seed(s); }

    bool inBounds(Action action) const;
    void executeAction(Action action);
    void revertAction(Action action);
    bool wallInTheWay(const Cell& cell, Action action) const;

    Point indexToPos(int index) const;
    int posToIndex(const Point& p) const;
    double calculateDistance(const Point& Origin, const Point& Pos) const;

    void setGoal(int index) { goal = index; }
    void setPos(int index) { pos = indexToPos(index); }
    void setrewardList(rewardList r) { rewards = r; }

    void generateMaze(MazeGenerator::Algorithm algo = MazeGenerator::Algorithm::BinaryTree);
    std::vector<float> getLocalObservation();
    Point reset();
    stepResult step(Action action);
    void printGrid();
};