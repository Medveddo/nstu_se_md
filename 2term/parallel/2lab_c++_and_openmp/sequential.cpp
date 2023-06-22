#include <iostream>
#include <chrono>

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

int main(int argc, char const *argv[])
{
    unsigned int intervalStart = 1;
    unsigned int intervalEnd = 1000000;
    unsigned int targetLength = 52;

    unsigned int foundSequences = 0;

    auto startTime = std::chrono::steady_clock::now();
    for (unsigned int i = intervalStart; i < intervalEnd; i++)
    {
        auto len = getLenOfSquence(i);
        if (len == targetLength)
        {
            foundSequences++;
        }
    }
    auto endTime = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime -
                                                                                startTime)
                              .count();
    std::cout << "(Sequential) - Interval[" << intervalStart << ":" << intervalEnd << "]Len[" << targetLength << "] Found: "
              << foundSequences << " sequences Took: " << duration << "[µs]" << std::endl;
    return 0;
}
