#pragma once

#include <vector>
#include "person.hpp"
#include "floor.hpp"

class Elevator
{
public:
    unsigned int id;
    unsigned int capacity;
    int targetFloor;
    int currentFloor;
    std::vector<Person> peopleInside;
    bool working;
    unsigned int floorMoveTimeMs;
    unsigned int enterExitPersonTimeMs;
    FloorsData *floorsData;

    Elevator();
    Elevator(unsigned int id, unsigned int capacity, int targetFloor, int currentFloor, unsigned int floorMoveTimeMs, unsigned int enterExitPersonTimeMs);
    Elevator(const Elevator &e);

    void start_running(FloorsData *floorsData, unsigned int iterationsCount);
    bool hasPeopleToExitCurrentFloor();
    bool hasResponsibleFloors();
    unsigned int getCurrentWeight();
};
