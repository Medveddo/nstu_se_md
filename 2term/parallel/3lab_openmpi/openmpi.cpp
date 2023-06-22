#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <string>
#define MASTER 0

struct TaskPrams
{
    unsigned int intervalStart;
    unsigned int intervalEnd;
    unsigned int targetLength;
} gTaskParams;

unsigned int countSequencesWithLenTask(
    unsigned int startNumber,
    unsigned int endNumber,
    unsigned int targetLength,
    int offset,
    int taskId);
int getLenOfSquence(int input);

int main(int argc, char *argv[])
{
    gTaskParams.intervalStart = 1;
    gTaskParams.intervalEnd = 250000;
    gTaskParams.targetLength = 24;

    int numtasks, taskid;
    unsigned int mysum;
    double update(int myoffset, int chunk, int myid);
    MPI_Status status;

    double start, end;

    auto startTime = std::chrono::steady_clock::now();
    /***** Initializations *****/
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    start = MPI_Wtime();

    std::stringstream ss;
    ss << "MPI task " << taskid << " has started..." << std::endl;
    std::cout << ss.str();

    int startNumberTag = 0;
    int endNumberTag = 1;
    int targetLengthTag = 2;
    int workDoneTag = 3;

    unsigned int totalSum = 0;

    /***** Master task only ******/
    if (taskid == MASTER)
    {
        ss.str(std::string());
        ss << "[m] numtasks = " << numtasks << std::endl;
        std::cout << ss.str();

        for (int dest = 1; dest < numtasks; dest++)
        {
            int startNumber = gTaskParams.intervalStart + dest;
            MPI_Send(&startNumber, 1, MPI_UNSIGNED, dest, startNumberTag, MPI_COMM_WORLD);
            MPI_Send(&gTaskParams.intervalEnd, 1, MPI_UNSIGNED, dest, endNumberTag, MPI_COMM_WORLD);
            MPI_Send(&gTaskParams.targetLength, 1, MPI_UNSIGNED, dest, targetLengthTag, MPI_COMM_WORLD);
        }

        /* Master does its part of the work */
        mysum = countSequencesWithLenTask(gTaskParams.intervalStart, gTaskParams.intervalEnd, gTaskParams.targetLength, numtasks, taskid);

        std::stringstream ss;
        ss << "[m" << taskid << "] musum = " << mysum;
        std::cout << ss.str() << std::endl;
        /* Wait to receive results from each task */
        for (int i = 1; i < numtasks; i++)
        {
            MPI_Recv(NULL, 0, MPI_UNSIGNED, i, workDoneTag, MPI_COMM_WORLD, &status);
        }

        /* Get final sum and print sample results */
        MPI_Reduce(&mysum, &totalSum, 1, MPI_UNSIGNED, MPI_SUM, MASTER, MPI_COMM_WORLD);

        ss.str(std::string());
        ss << "[m] Final sum = " << totalSum << std::endl;
        std::cout << ss.str() << std::endl;
    } /* end of master section */

    /***** Non-master tasks only *****/

    if (taskid > MASTER)
    {
        int source = MASTER;
        int startNumberLocal;
        int endNumberLocal;
        int targetLengthLocal;

        MPI_Recv(&startNumberLocal, 1, MPI_UNSIGNED, MASTER, startNumberTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&endNumberLocal, 1, MPI_UNSIGNED, MASTER, endNumberTag, MPI_COMM_WORLD, &status);
        MPI_Recv(&targetLengthLocal, 1, MPI_UNSIGNED, MASTER, targetLengthTag, MPI_COMM_WORLD, &status);
        /* Do my part of the work */
        mysum = countSequencesWithLenTask(startNumberLocal, endNumberLocal, targetLengthLocal, numtasks, taskid);
        std::stringstream ss;
        ss << "[s" << taskid << "] musum = " << mysum;
        std::cout << ss.str() << std::endl;
        /* Send my results back to the master task */
        MPI_Send(NULL, 0, MPI_UNSIGNED, MASTER, workDoneTag, MPI_COMM_WORLD);

        /* Use sum reduction operation to obtain final sum */
        MPI_Reduce(&mysum, &totalSum, 1, MPI_UNSIGNED, MPI_SUM, MASTER, MPI_COMM_WORLD);
    } /* end of non-master */

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    MPI_Finalize();
    if (taskid == MASTER)
    {
        auto endTime = std::chrono::steady_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime -
                                                                                    startTime)
                                  .count();
        std::cout << "(MPI_chrono::steady_clock) - Interval[" << gTaskParams.intervalStart << ":" << gTaskParams.intervalEnd << "]Len[" << gTaskParams.targetLength << "] Found: "
                  << totalSum << " sequences Took: " << duration << "[µs]" << std::endl;

        std::cout << "(MPI_MPI_Wtime) - Interval[" << gTaskParams.intervalStart << ":" << gTaskParams.intervalEnd << "]Len[" << gTaskParams.targetLength << "] Found: "
                  << totalSum << " sequences Took: " << (end - start) * 1000000 << "[µs]" << std::endl;
    }
}

unsigned int countSequencesWithLenTask(
    unsigned int startNumber,
    unsigned int endNumber,
    unsigned int targetLength,
    int offset,
    int taskId)
{
    /* Perform addition to each of my array elements and keep my sum */
    unsigned int sum = 0;
    int result;
    for (unsigned int i = startNumber; i < endNumber; i += offset)
    {
        result = getLenOfSquence(i);
        if (result == targetLength)
        {
            sum++;
        }
    }
    return sum;
}

/**
 * ni+1 = (ni % 2 == 0? ni / 2 : 3 * ni + 1)
 * eg. 13->40->20->10->5->16->8->4->2->1
 *
 * returns len of sequence
 */
int getLenOfSquence(int input)
{
    int currentValue = input;
    int iterationsCount = 1;
    // -O2 and -O3 optimization flags never exit (currentValue > 0) while currentValue actualy below zero
    // $ g++ -O1 wtf.cpp -o wtf
    // -1
    // $ g++ -O2 wtf.cpp -o wtf
    // endless while loop
    // ????
    while (currentValue != 1 && currentValue > 0) // for integer overflow (e.g. 113383)
    {
        iterationsCount++;
        currentValue = currentValue % 2 == 0 ? currentValue / 2 : 3 * currentValue + 1;
    }
    if (currentValue < 0)
    {
        return -1;
    }
    return iterationsCount;
}