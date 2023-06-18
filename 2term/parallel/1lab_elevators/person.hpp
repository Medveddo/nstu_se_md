#pragma once

#include <random>

class Person
{
public:
    int id;
    int startFloor;
    int targetFloor;
    unsigned int weight;
    Person();
    Person(int startFloor, int targetFloor, unsigned int weight);
    Person(const Person &p);
};
