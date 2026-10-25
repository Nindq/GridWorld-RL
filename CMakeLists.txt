cmake_minimum_required(VERSION 3.14)

project(GridWorldRL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(GridWorldCore
	GridWorld.cpp
	MazeGenerator.cpp
	AStar.cpp
)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

pybind11_add_module(gridworld_module
	bindings.cpp
)

target_link_libraries(gridworld_module
	PRIVATE
	GridWorldCore
)
