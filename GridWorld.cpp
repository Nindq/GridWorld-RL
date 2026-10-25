#include "Gridworld.hpp"
#include <print>
#include <cmath>
// Implementation (.cpp)
Gridworld::Gridworld(int size)
    : size(size), goal((size* size) - 1), Grid(size* size), gen(std::random_device{}()) {
}

Gridworld::Gridworld(int size, unsigned int seed)
    : size(size), goal((size* size) - 1), Grid(size* size), gen(std::random_device{}()) {
}

Gridworld::Gridworld(int size, Point goal, unsigned int seed)
    : size(size), goal(goal.x + goal.y * size), Grid(size* size), gen(seed) {
}

Gridworld::Gridworld(int size, int goal, unsigned int seed)
    : size(size), goal(goal), Grid(size* size), gen(seed) {
}

bool Gridworld::inBounds(Action action) const {
    if (action == Action::UP)    return pos.y - 1 >= 0;
    if (action == Action::DOWN)  return pos.y + 1 < size;
    if (action == Action::LEFT)  return pos.x - 1 >= 0;
    if (action == Action::RIGHT) return pos.x + 1 < size;
    return false;
}

void Gridworld::executeAction(Action action) {
    if (action == Action::UP)    pos.y -= 1;
    if (action == Action::DOWN)  pos.y += 1;
    if (action == Action::LEFT)  pos.x -= 1;
    if (action == Action::RIGHT) pos.x += 1;
}

void Gridworld::revertAction(Action action) {
    if (action == Action::UP)    pos.y += 1;
    if (action == Action::DOWN)  pos.y -= 1;
    if (action == Action::LEFT)  pos.x += 1;
    if (action == Action::RIGHT) pos.x -= 1;
}

bool Gridworld::wallInTheWay(const Cell& cell, Action action) const {
    switch (action) {
    case Action::UP:    return cell.checkWall(Wall::North);
    case Action::DOWN:  return cell.checkWall(Wall::South);
    case Action::RIGHT: return cell.checkWall(Wall::East);
    case Action::LEFT:  return cell.checkWall(Wall::West);
    }
    return false;
}

Point Gridworld::indexToPos(int index) const {
    return { index % size, index / size };
}

int Gridworld::posToIndex(const Point& p) const {
    return p.x + p.y * size;
}

double Gridworld::calculateDistance(const Point& Origin, const Point& Pos) const {
    int ydelta = Pos.y - Origin.y;
    int xdelta = Pos.x - Origin.x;
    return std::sqrt(ydelta * ydelta + xdelta * xdelta);
}

void Gridworld::generateMaze(MazeGenerator::Algorithm algo) {
    for (auto& cell : Grid) {
        cell.walls = Wall::North | Wall::East | Wall::South | Wall::West;
        cell.visited = false;
    }
    MazeGenerator::generate(Grid, size, algo, gen);
}

std::vector<float> Gridworld::getLocalObservation() {
    std::vector<float> obs(6, 0.0f);

    int currentIdx = posToIndex(pos);
    const Cell& cell = Grid[currentIdx];

    obs[0] = cell.checkWall(Wall::North) ? 1.0f : 0.0f;
    obs[1] = cell.checkWall(Wall::South) ? 1.0f : 0.0f;
    obs[2] = cell.checkWall(Wall::East) ? 1.0f : 0.0f;
    obs[3] = cell.checkWall(Wall::West) ? 1.0f : 0.0f;

    int goalX = goal % size;
    int goalY = goal / size;

    obs[4] = static_cast<float>(goalX - pos.x) / size;
    obs[5] = static_cast<float>(goalY - pos.y) / size;

    return obs;
}

Point Gridworld::reset() {
    pos.x = 0;
    pos.y = 0;
    return pos;
}

stepResult Gridworld::step(Action action) {
    stepResult result;

    if (wallInTheWay(Grid[posToIndex(pos)], action)) {
        result.state = pos;
        result.reward = rewards.wall;
        return result;
    }

    executeAction(action);
    result.reward = rewards.step;
    result.state = pos;

    if (posToIndex(pos) == goal) {
        result.reward = rewards.goal;
        result.Done = true;
    }

    return result;
}

void Gridworld::printGrid() {
    std::print("\n");

    for (int y = 0; y < size; ++y) {
        // Top border line for row y
        for (int i = y * size; i < (y + 1) * size; ++i) {
            std::printf("+");
            if (Grid[i].checkWall(Wall::North)) std::printf("---");
            else std::printf("   ");
            if ((i + 1) % size == 0) std::printf("+\n");
        }
        // Middle content line for row y
        for (int x = 0; x < size; ++x) {
            int i = y * size + x;
            if (Grid[i].checkWall(Wall::West)) std::printf("|");
            else std::printf(" ");

            if (posToIndex(pos) == i) std::printf(" P ");
            else if (goal == i) std::printf(" G ");
            else std::printf("   ");
        }
        std::printf("|\n");
    }

    // Bottom border line for the final row
    for (int i = size * (size - 1); i < size * size; ++i) {
        std::printf("+");
        if (Grid[i].checkWall(Wall::South)) std::printf("---");
        else std::printf("   ");
        if ((i + 1) % size == 0) std::printf("+\n");
    }
    std::print("\n");
}