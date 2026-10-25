#pragma once
#include <cstdint>

enum class Action { UP, DOWN, RIGHT, LEFT };

struct Point {
    int x{ 0 };
    int y{ 0 };
};

struct stepResult {
    Point state;
    int reward{ 0 };
    bool Done{ false };
};

struct rewardList {
    int wall{ -10 };
    int goal{ 50 };
    int step{ -1 };
};

namespace Wall {
    enum Direction : uint8_t {
        North = 1 << 0, // 1
        East = 1 << 1, // 2
        South = 1 << 2, // 4
        West = 1 << 3  // 8
    };

    inline Direction opposite(Direction dir) {
        switch (dir) {
        case North: return South;
        case South: return North;
        case East:  return West;
        case West:  return East;
        default:    return dir;
        }
    }
}

struct Cell {
    uint8_t walls = Wall::North | Wall::East | Wall::South | Wall::West;
    bool visited = false;

    void removeWall(Wall::Direction direction) { walls &= ~direction; }
    void addWall(Wall::Direction direction) { walls |= direction; }
    bool checkWall(Wall::Direction direction) const { return (walls & direction) != 0; }
};