#include "person.hpp"


Person::Person()
{
    this->startFloor = 0;
    this->targetFloor = 0;
    this->weight = 0;
}

Person::Person(int startFloor, int targetFloor, unsigned int weight)
{
    this->startFloor = startFloor;
    this->targetFloor = targetFloor;
    this->weight = weight;
    this->id = std::rand();
}

Person::Person(const Person &p)
{
    this->startFloor = p.startFloor;
    this->targetFloor = p.targetFloor;
    this->weight = p.weight;
    this->id = p.id;
}