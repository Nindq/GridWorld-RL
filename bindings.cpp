#include "Types.hpp"
#include "GridWorld.hpp"
#include "AStar.hpp"
#include "MazeGenerator.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

PYBIND11_MODULE(gridworld_module, m) {
	py::enum_<Action>(m, "Action")
		.value("UP", Action::UP)
		.value("DOWN", Action::DOWN)
		.value("LEFT", Action::LEFT)
		.value("RIGHT", Action::RIGHT)
		.export_values();
	py::enum_<MazeGenerator::Algorithm>(m, "Algorithm")
		.value("BinaryTree", MazeGenerator::Algorithm::BinaryTree)
		.value("DepthFirstSearch", MazeGenerator::Algorithm::DepthFirstSearch)
		.export_values();
	py::enum_<Wall::Direction>(m, "Direction")
		.value("North", Wall::North)
		.value("East", Wall::East)
		.value("South", Wall::South)
		.value("West", Wall::West)
		.export_values();
	py::class_<Point>(m, "Point")
		.def_readwrite("x", &Point::x)
		.def_readwrite("y", &Point::y);
	py::class_<stepResult>(m, "StepResult")
		.def_readwrite("state", &stepResult::state)
		.def_readwrite("reward", &stepResult::reward)
		.def_readwrite("Done", &stepResult::Done);
	py::class_<rewardList>(m, "RewardList")
		.def_readwrite("wall", &rewardList::wall)
		.def_readwrite("step", &rewardList::step)
		.def_readwrite("goal", &rewardList::goal);
	py::class_<Cell>(m, "Cell")
		.def(py::init<>())
		.def_readwrite("walls", &Cell::walls)
		.def_readwrite("visited", &Cell::visited)
		.def("removeWall", &Cell::removeWall)
		.def("addWall", &Cell::addWall)
		.def("checkWall", &Cell::checkWall);
	m.def(
		"solveAStar",
		&Pathfinding::solveAStar,
		"Solves A* pathfinding on grid",
		py::arg("grid"),
		py::arg("size"),
		py::arg("startIdx"),
		py::arg("goalIdx")
	);
	py::class_<Gridworld>(m, "Gridworld")
		.def(py::init<int>())
		.def(py::init<int, unsigned int>())
		.def(py::init<int, int, unsigned int>())
		.def(py::init<int, Point, unsigned int>())
		.def("reset", &Gridworld::reset)
		.def("printGrid", &Gridworld::printGrid)
		.def("step", &Gridworld::step)
		//.def("setTile", &Gridworld::setTile)
		.def("setGoal", &Gridworld::setGoal)
		.def("setPos", &Gridworld::setPos)
		.def("seed", &Gridworld::seed)
		.def("setrewardList", &Gridworld::setrewardList)
		.def("calculateDistance", &Gridworld::calculateDistance)
		.def("inbounds", &Gridworld::inBounds)
		.def("indexToPos", &Gridworld::indexToPos)
		.def("posToIndex", &Gridworld::posToIndex)
		.def("generateMaze", &Gridworld::generateMaze, py::arg("algorithm") = MazeGenerator::Algorithm::DepthFirstSearch)
		.def("getLocalObservation", &Gridworld::getLocalObservation)
		.def_readwrite("Grid", &Gridworld::Grid)
		.def_readwrite("pos", &Gridworld::pos)
		.def_readwrite("goal", &Gridworld::goal)
		.def_readwrite("rewards", &Gridworld::rewards)
		.def_readwrite("size", &Gridworld::size);
	

}
