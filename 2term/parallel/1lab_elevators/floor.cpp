#include "floor.hpp"

Floor::Floor(unsigned int id, unsigned int elevatorsCount)
{
    this->id = id;
    this->responsibleElevator = -1;
    this->downIsPressed = false;
    this->upIsPressed = false;
    this->people = std::vector<Person>();
}
Floor::Floor(const Floor &f)
{
    this->id = f.id;
    this->responsibleElevator = f.responsibleElevator;
    this->downIsPressed = f.downIsPressed;
    this->upIsPressed = f.upIsPressed;
    this->people = f.people;
}
void Floor::generatePerson(unsigned int startFloor, unsigned int targetFloor)
{
    auto p = Person(startFloor, targetFloor, 100);

    this->people.push_back(p);
    if (targetFloor > startFloor)
    {
        std::stringstream ss;
        ss << "spawn- Person[" << p.id << "] (going " << p.startFloor << "->" << p.targetFloor << ") is pressing UP button..." << std::endl;
        std::cout << ss.str();
        this->pressUp();
    }
    else
    {
        std::stringstream ss;
        ss << "spawn- Person[" << p.id << "] (going " << p.startFloor << "->" << p.targetFloor << ") is pressing DOWN button..." << std::endl;
        std::cout << ss.str();
        this->pressDown();
    }
}

bool Floor::hasPeopleInside()
{
    return !this->people.empty();
}

void Floor::pressDown()
{
    this->downIsPressed = true;
}
void Floor::pressUp()
{   
    this->upIsPressed = true;
}
void Floor::releaseDown()
{
    for (auto &&person : this->people)
    {
        if (person.targetFloor < this->id){
            return;
        }
    }
    this->downIsPressed = false;
}
void Floor::releaseUp()
{
    for (auto &&person : this->people)
    {
        if (person.targetFloor > this->id){
            return;
        }
    }
    this->upIsPressed = false;
}
bool Floor::isUpPressed()
{
    return this->upIsPressed;
}
bool Floor::isDownPressed()
{
    return this->downIsPressed;
}
bool Floor::isAnyButtonsPressed()
{
    return this->downIsPressed || this->upIsPressed;
}

FloorsData::FloorsData()
{
    this->floors = std::vector<Floor>();
    this->mutex = new std::mutex();
}
FloorsData::FloorsData(const FloorsData &fd)
{
    this->floors = fd.floors;
    this->mutex = fd.mutex;
}
FloorsData::FloorsData(unsigned int floorsCount, unsigned int elevatorsCount)
{
    this->floors = std::vector<Floor>();
    this->floors.reserve(floorsCount);
    for (unsigned int i = 0; i < floorsCount; i++)
    {
        this->floors.emplace_back(Floor(i, elevatorsCount));
    }
    this->mutex = new std::mutex();
}
void FloorsData::generatePerson()
{
    unsigned int startFloor = std::rand() % this->floors.size();

    int targetFloor = startFloor;
    while (targetFloor == startFloor)
    {
        targetFloor = std::rand() % this->floors.size();
    }
    this->floors.at(startFloor).generatePerson(startFloor, targetFloor);
}
void FloorsData::generatePerson(unsigned int startFloor, unsigned int targetFloor){
    this->floors.at(startFloor).generatePerson(startFloor, targetFloor);
}