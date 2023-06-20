#include <vector>
#include <thread>
#include <iostream>
#include <sstream> // std::stringstream
#include <string>
#include "elevator.hpp"
#include "floor.hpp"

std::mutex g_mutex;

Elevator::Elevator()
{
  this->id = -1;
  this->capacity = 0;
  this->targetFloor = 0;
  this->currentFloor = 0;
  this->peopleInside = std::vector<Person>();
  this->working = false;
  this->floorsData = NULL;
}

Elevator::Elevator(
    unsigned int id,
    unsigned int capacity,
    int targetFloor,
    int currentFloor, unsigned int floorMoveTimeMs, unsigned int enterExitPersonTimeMs)
{
  this->id = id;
  this->capacity = capacity;
  this->targetFloor = targetFloor;
  this->currentFloor = currentFloor;
  this->peopleInside = std::vector<Person>();
  this->working = false;
  this->floorMoveTimeMs = floorMoveTimeMs;
  this->enterExitPersonTimeMs = enterExitPersonTimeMs;
  this->floorsData = NULL;
}

Elevator::Elevator(const Elevator &e)
{
  this->id = e.id;
  this->capacity = e.capacity;
  this->targetFloor = e.targetFloor;
  this->currentFloor = e.currentFloor;
  this->peopleInside = e.peopleInside;
  this->working = e.working;
  this->floorsData = e.floorsData;
  this->floorMoveTimeMs = e.floorMoveTimeMs;
  this->enterExitPersonTimeMs = e.enterExitPersonTimeMs;
}

bool Elevator::hasPeopleToExitCurrentFloor()
{
  for (auto &&person : this->peopleInside)
  {
    if (person.targetFloor == this->currentFloor)
    {
      return true;
    }
  }
  return false;
}

bool Elevator::hasResponsibleFloors()
{
  for (auto &&floor : this->floorsData->floors)
  {
    if (floor.responsibleElevator == this->id)
    {
      return true;
    }
  }
  return false;
}

unsigned int Elevator::getCurrentWeight()
{
  unsigned int currWeight = 0;
  for (auto &&person : this->peopleInside)
  {
    currWeight += person.weight;
  }
  return currWeight;
}

void Elevator::start_running(FloorsData *floorsData, unsigned int iterationsCount)
{
  this->floorsData = floorsData;
  this->working = true;
  std::stringstream ss;
  ss << "    Elevator[" << this->id << "]"
     << " start running" << std::endl;
  std::cout << ss.str();

  // Elevator loop
  for (size_t i = 0; i < iterationsCount; i++)
  {
    floorsData->mutex->lock();
    // exiting people
    if (this->hasPeopleToExitCurrentFloor())
    {
      ss.str(std::string());
      ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
         << " arrived at Floor[" << this->currentFloor << "] and has people exiting!" << std::endl;
      std::cout << ss.str();

      bool debugVectorSizes = false;
      if (debugVectorSizes)
      {
        ss.str(std::string());
        ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
           << " has " << this->peopleInside.size() << " People inside. Start exiting..." << std::endl;
        std::cout << ss.str();
      }

      // delete people
      for (auto iter = this->peopleInside.begin(); iter != this->peopleInside.end();)
      {
        if (iter->targetFloor == this->targetFloor)
        {
          ss.str(std::string());
          ss << "    Elevator[" << this->id << "]Iteration[" << i << "]"
             << " Person[" << iter->id << "]"
             << " arrived at Floor [" << this->currentFloor << "] and exited the elevator." << std::endl;
          std::cout << ss.str();

          iter = this->peopleInside.erase(iter);

          floorsData->mutex->unlock();
          std::this_thread::sleep_for(std::chrono::milliseconds(this->enterExitPersonTimeMs));
          floorsData->mutex->lock();

          // in case when this->peopleInside gets mutated while sleep
          iter = this->peopleInside.begin(); 
        }
        else
        {
          ++iter;
        }
      }
      if (debugVectorSizes)
      {
        ss.str(std::string());
        ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
           << " has " << this->peopleInside.size() << " People inside at the end of exiting." << std::endl;
        std::cout << ss.str();
      }
      if (this->peopleInside.empty())
      {
        ss.str(std::string());
        ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
           << " carried all people. Remove target floor." << std::endl;
        std::cout << ss.str();

        this->targetFloor = this->currentFloor;
      }
      else
      {
        for (auto &&personInside : this->peopleInside)
        {
          this->targetFloor = personInside.targetFloor;
          ss.str(std::string());
          ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
             << " targeting Floor[" << this->targetFloor << "] (for Person[" << personInside.id << "])" << std::endl;
          std::cout << ss.str();
          break;
        }
      }
    }

    // Moving
    else if (this->currentFloor != this->targetFloor)
    {
      int delta = (this->targetFloor - this->currentFloor) / abs(this->targetFloor - this->currentFloor);
      ss.str(std::string());
      ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
         << " mooving from Floor[" << this->currentFloor << "] at Floor[" << this->targetFloor << "] with delta=" << delta << std::endl;
      std::cout << ss.str();
      this->currentFloor += delta;
    }
    // Arrived, get people, clear responsibility
    else if (this->currentFloor == this->targetFloor && floorsData->floors.at(currentFloor).isAnyButtonsPressed() && floorsData->floors.at(currentFloor).responsibleElevator == this->id)
    {
      ss.str(std::string());
      ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
         << " arrived at Floor[" << this->currentFloor << "] for people" << std::endl;
      std::cout << ss.str();

      // reference is important to do operations with original one
      auto floorPeople = &floorsData->floors.at(this->currentFloor).people;
      for (auto iter = floorPeople->begin(); iter != floorPeople->end();)
      {

        // set targer floor to first person target
        if (this->currentFloor == this->targetFloor)
        {
          ss.str(std::string());
          ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
             << " is targeting Person[" << iter->id << "] Floor[" << iter->startFloor << "]->Floor[" << iter->targetFloor << "]" << std::endl;
          std::cout << ss.str();
          this->targetFloor = iter->targetFloor;
        }
        // if person is on the way to target - also enter

        if (
            // cur = 3 tar=4 target = 5
            ((this->targetFloor > this->currentFloor && iter->targetFloor <= this->targetFloor && iter->targetFloor > this->currentFloor) ||
             // target = 0 tar=1 cur = 3
             (this->targetFloor < this->currentFloor && iter->targetFloor >= this->targetFloor && iter->targetFloor < this->currentFloor)) &&
            this->getCurrentWeight() + iter->weight <= this->capacity)
        {
          bool debugVectorsSizes = false;
          if (debugVectorsSizes)
          {
            ss.str(std::string());
            ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
               << " attempting to enter Person[" << iter->id << "] Floor[" << iter->startFloor << "]->Floor[" << iter->targetFloor << "] floorPeopleSize=" << floorPeople->size() << " elevatorPeopleSize=" << this->peopleInside.size() << std::endl;
            std::cout << ss.str();
          }

          this->peopleInside.insert(this->peopleInside.end(), std::make_move_iterator(iter), std::make_move_iterator(iter + 1));

          if (debugVectorsSizes)
          {
            ss.str(std::string());
            ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
               << " after attempting to enter Person[" << iter->id << "] Floor[" << iter->startFloor << "]->Floor["
               << iter->targetFloor << "] floorPeopleSize=";
          }
          ss.str(std::string());
          ss << "    Elevator[" << this->id << "]Iteration[" << i << "]"
              << " Person[" << iter->id << "] (" << iter->startFloor << "->"
              << iter->targetFloor << ") entered elevator at Floor[" << this->currentFloor << "]" << std::endl;
          std::cout << ss.str();
          iter = floorPeople->erase(iter);
          if (debugVectorsSizes)
          {
            ss << floorPeople->size() << " elevatorPeopleSize="
               << this->peopleInside.size() << std::endl;
            std::cout << ss.str();
          }
          floorsData->mutex->unlock();
          std::this_thread::sleep_for(std::chrono::milliseconds(this->enterExitPersonTimeMs));
          floorsData->mutex->lock();

          // in case when floorPeople gets mutated while sleep
          iter = floorPeople->begin(); 
        }
        else
        {
          ++iter;
        }
      }

      // if floor empty -> release button
      // release button
      if (this->targetFloor < this->currentFloor)
      {
        floorsData->floors.at(this->currentFloor).releaseDown();
      }
      if (this->targetFloor > this->currentFloor)
      {
        floorsData->floors.at(this->currentFloor).releaseUp();
      }
      floorsData->floors.at(this->currentFloor).responsibleElevator = -1;
    }
    // IDle
    else if (this->currentFloor == this->targetFloor)
    {
      ss.str(std::string());
      ss << "Elevator[" << this->id << "]Iteration[" << i << "]"
         << " stays IDLE at Floor[" << this->currentFloor
         << "] ";

      if (!this->hasResponsibleFloors())
      {
        ss << " has no responsible floors. Start searching. ";

        for (auto &&floor : floorsData->floors)
        {
          if (floor.isAnyButtonsPressed() && floor.responsibleElevator == -1)
          {
            this->targetFloor = floor.id;
            floor.responsibleElevator = this->id;

            ss << " found Floor[" << floor.id << "] with active button."
               << " Set Floor[" << this->targetFloor << "] as target. ";
            break;
          }
        }

        if (!this->hasResponsibleFloors())
        {
          ss << " IDLE after search at Floor[" << this->currentFloor
             << "]";
        }
      }
      ss << std::endl;
      std::cout << ss.str();
    }

    floorsData->mutex->unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(this->floorMoveTimeMs));
  }
  ss.str(std::string());
  ss << "    Elevator[" << this->id << "] "
     << "stopped working with " << this->peopleInside.size() << " people inside" << std::endl;
  std::cout << ss.str();
  this->working = false;
}
