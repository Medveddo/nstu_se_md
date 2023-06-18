#include <string>   // std::string
#include <iostream> // std::cout
#include <sstream>  // std::stringstream
#include <vector>
#include <thread>
#include <time.h>

#include "person.hpp"
#include "elevator.hpp"
#include "floor.hpp"

class ElevatorsState
{
public:
    std::vector<Elevator> elevators;
    std::vector<std::thread> elevatorThreads;
    ElevatorsState()
    {
        this->elevators = std::vector<Elevator>();
    }
    ElevatorsState(unsigned int elevatorsCount, const std::vector<unsigned int> &capacities, unsigned int floorMoveTimeMs, unsigned int enterExitPersonTimeMs)
    {
        for (unsigned int i = 0; i < elevatorsCount; i++)
        {
            this->elevators.push_back(Elevator(i, capacities.at(i), 0, 0, floorMoveTimeMs, enterExitPersonTimeMs));
        }
    }

    void start_elevators(FloorsData *floorsData, unsigned int iterationsCount)
    {
        for (auto &&elevator : this->elevators)
        {
            std::thread t(&Elevator::start_running, elevator, floorsData, iterationsCount);
            this->elevatorThreads.push_back(move(t));
        }
    }
    void await()
    {
        for (auto &&thread : this->elevatorThreads)
        {
            thread.join();
        }
    }
};

class ElevatorApplication
{
public:
    unsigned int nElevators;
    unsigned int nFloors;
    unsigned int floorMoveTimeMs;
    unsigned int enterExitPersonTimeMs;
    unsigned int spawnIntervalMs;
    std::vector<unsigned int> elevatorsCarryingCapacities;
    ElevatorsState houseState;
    FloorsData floorsData;

    ElevatorApplication()
    {
        this->nElevators = 0;
        this->nFloors = 0;
        this->floorMoveTimeMs = 0;
        this->enterExitPersonTimeMs = 0;
        this->spawnIntervalMs = 0;
        this->elevatorsCarryingCapacities = std::vector<unsigned int>();
    }
    ElevatorApplication(const ElevatorApplication &ea)
    {
        this->nElevators = ea.nElevators;
        this->nFloors = ea.nFloors;
        this->spawnIntervalMs = ea.spawnIntervalMs;
        this->floorMoveTimeMs = ea.floorMoveTimeMs;
        this->enterExitPersonTimeMs = ea.enterExitPersonTimeMs;
        this->elevatorsCarryingCapacities = ea.elevatorsCarryingCapacities;
        this->houseState = ElevatorsState();
    }
    ElevatorApplication(
        unsigned int nElevators,
        unsigned int nFloors,
        unsigned int floorMoveTimeMs,
        unsigned int enterExitPersonTimeMs,
        unsigned int defaultElevatorCapacity,
        unsigned int spawnIntervalMs)
    {
        this->nElevators = nElevators;
        this->nFloors = nFloors;
        this->spawnIntervalMs = spawnIntervalMs;
        this->floorMoveTimeMs = floorMoveTimeMs;
        this->enterExitPersonTimeMs = enterExitPersonTimeMs;
        this->elevatorsCarryingCapacities = std::vector<unsigned int>();
        for (unsigned int i = 0; i < this->nElevators; i++)
        {
            this->elevatorsCarryingCapacities.push_back(defaultElevatorCapacity);
        }

        this->houseState = ElevatorsState(nElevators, this->elevatorsCarryingCapacities, floorMoveTimeMs, enterExitPersonTimeMs);
        this->floorsData = FloorsData(this->nFloors, this->nElevators);
    }

    void periodicallySpawnPersonsRoutine(unsigned int peopleSpawnIterationsCount)
    {
        unsigned int iterations = 0;
        std::stringstream ss;
        do
        {
            this->floorsData.mutex->lock();
            ss.str(std::string());
            ss << "==SpawningPersons[" << iterations << "]==" << std::endl;
            std::cout << ss.str();
            this->floorsData.generatePerson();
            this->floorsData.generatePerson();
            this->floorsData.mutex->unlock();
            iterations++;
            std::this_thread::sleep_for(std::chrono::milliseconds(this->spawnIntervalMs));
        } while (iterations < peopleSpawnIterationsCount);
    }

    void start(unsigned int elevatorIterationsCount, unsigned int peopleSpawnIterationsCount)
    {
        std::thread spawnPersonsThread(&ElevatorApplication::periodicallySpawnPersonsRoutine, this, peopleSpawnIterationsCount);
        this->houseState.start_elevators(&this->floorsData, elevatorIterationsCount);

        spawnPersonsThread.join();
        this->houseState.await();
    }
};

int main(int argc, char const *argv[])
{
    // std::srand(time(NULL));
    std::srand(0);
    unsigned int nElevators = 2u;
    unsigned int nFloors = 5u;
    unsigned int defaultElevatorCapacity = 500u;
    unsigned int floorMoveTimeMs = 1000u;
    // unsigned int floorMoveTimeMs = 0u;
    unsigned int enterExitPersonTimeMs = 500u;
    // unsigned int enterExitPersonTimeMs = 0u;
    unsigned int spawnIntervalMs = 5000u;
    // unsigned int spawnIntervalMs = 0u;

    unsigned int elevatorIterationsCount = 20u;
    unsigned int peopleSpawnIterationsCount = 3u;
    ElevatorApplication(
        nElevators, nFloors, floorMoveTimeMs,
        enterExitPersonTimeMs, defaultElevatorCapacity, spawnIntervalMs)
        .start(elevatorIterationsCount, peopleSpawnIterationsCount);
    return 0;
}
