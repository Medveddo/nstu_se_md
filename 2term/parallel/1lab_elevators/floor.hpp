#pragma once

#include <vector>
#include <mutex>
#include <string>
#include <iostream>
#include <sstream> // std::stringstream

#include "person.hpp"

class Floor
{
private:
    bool downIsPressed;
    bool upIsPressed;

public:
    unsigned int id;
    int responsibleElevator;
    std::vector<Person> people;

    Floor(unsigned int id, unsigned int elevatorsCount);
    Floor(const Floor &f);

    void generatePerson(unsigned int floorsCount, unsigned int startFloor);
    bool hasPeopleInside();
    void pressDown();
    void pressUp();
    void releaseDown();
    void releaseUp();
    bool isUpPressed();
    bool isDownPressed();
    bool isAnyButtonsPressed();
};

class FloorsData
{
public:
    std::vector<Floor> floors;
    std::mutex *mutex;

    FloorsData();
    FloorsData(const FloorsData &fd);
    FloorsData(unsigned int floorsCount, unsigned int elevatorsCount);
    void generatePerson();
    void generatePerson(unsigned int startFloor, unsigned int targetFloor);
};