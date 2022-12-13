#include <bit>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <ostream>
#include <sstream>
#include <typeinfo>
#include <vector>
#include <sys/stat.h>
#include <string>
#include <fstream>

const int MAX_GEN_INT = 50000;

std::vector<int> GenerateInt(size_t size) {
    std::vector<int> v;
    v.reserve(size);
    for (uint32_t i = 0; i < size; ++i) {
        v.push_back(rand() % MAX_GEN_INT);
    }
    return v;
}
std::vector<float> GenerateFloat(size_t size) {
    std::vector<float> v;
    v.reserve(size);
    for (uint32_t i = 0; i < size; ++i) {
        v.push_back(rand() / static_cast<float>(MAX_GEN_INT));
    }
    return v;
}
std::vector<double> GenerateDouble(size_t size) {
    std::vector<double> v;
    v.reserve(size);
    for (uint32_t i = 0; i < size; ++i) {
        v.push_back(rand() / static_cast<double>(MAX_GEN_INT));
    }
    return v;
}

int TaskInt(int input)
{
    return (((input + 1337) / 228) * 1488) - 3000;
}

int TaskFloat(float input)
{
    return (((input + 1337.F) / 228.F) * 1488.F) - 3000.F;
}
int TaskDouble(double input)
{
    return (((input + 1337.) / 228.) * 1488.) - 3000.;
}

// check if file exists on disk
inline bool isFileExists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int main(int argc, char const *argv[])
{
    std::cout << "START\n";

    std::string iterationsCountStr = argv[1];
    std::string optimizationFlag = argv[2];
    const std::string statFileName = "inner/stats.csv";
    const int iterationsCount = atoi(iterationsCountStr.c_str());
    const int ITERATIONS_COUNT = iterationsCount <= 10 ? 10 : iterationsCount;

    const int TASKS_NUMBERS_COUNT = 10000000;
    std::cout << "Iterations count: " << ITERATIONS_COUNT << std::endl; 

    // Setup seed for random
    srand(static_cast<unsigned>(time(0)));

    std::vector<unsigned long> iterationsTime;
    iterationsTime.reserve(ITERATIONS_COUNT * 3);

    // To calc average iteration time
    unsigned long totalTime = 0L;
    unsigned long iterationTime = 0L;
    unsigned long averageIntIterationTime = 0L;
    unsigned long averageFloatIterationTime = 0L;
    unsigned long averageDoubleIterationTime = 0L;

    std::vector<int> numbers_int = GenerateInt(TASKS_NUMBERS_COUNT);
    std::vector<float> numbers_float = GenerateFloat(TASKS_NUMBERS_COUNT);
    std::vector<double> numbers_double = GenerateDouble(TASKS_NUMBERS_COUNT);
    std::vector<int> int_results;
    int_results.reserve(TASKS_NUMBERS_COUNT);
    std::vector<float> float_results;
    float_results.reserve(TASKS_NUMBERS_COUNT);
    std::vector<double> double_results;
    double_results.reserve(TASKS_NUMBERS_COUNT);

    for (uint32_t i_iteration = 0; i_iteration < ITERATIONS_COUNT; ++i_iteration)
    {
        std::chrono::steady_clock::time_point begin_iteration =
            std::chrono::steady_clock::now();
        for (const auto &number: numbers_int)
        {
            int res = TaskInt(number);
            int_results.push_back(res); // to disable compiler optimizations
        }
        std::chrono::steady_clock::time_point end_iteration =
            std::chrono::steady_clock::now();
        const uint64_t duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_iteration -
                                                                  begin_iteration)
                .count();
        std::cout << "Int Iteration " << i_iteration << " - " << duration << " microseconds\n";
        iterationsTime.push_back(duration);
        iterationTime += duration;
    }
    averageIntIterationTime = iterationTime / ITERATIONS_COUNT;
    iterationTime = 0L;
    for (uint32_t i_iteration = 0; i_iteration < ITERATIONS_COUNT; ++i_iteration)
    {
        std::chrono::steady_clock::time_point begin_iteration =
            std::chrono::steady_clock::now();
        for (const auto &number: numbers_float)
        {
            float res = TaskFloat(number);
            float_results.push_back(res); // to disable compiler optimizations
        }
        std::chrono::steady_clock::time_point end_iteration =
            std::chrono::steady_clock::now();
        const uint64_t duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_iteration -
                                                                  begin_iteration)
                .count();
        std::cout << "Float Iteration " << i_iteration << " - " << duration << " microseconds\n"; 
        iterationsTime.push_back(duration);
        iterationTime += duration;
    }
    averageFloatIterationTime = iterationTime / ITERATIONS_COUNT;
    iterationTime = 0L;
    for (uint32_t i_iteration = 0; i_iteration < ITERATIONS_COUNT; ++i_iteration)
    {
        std::chrono::steady_clock::time_point begin_iteration =
            std::chrono::steady_clock::now();
        for (const auto &number: numbers_double)
        {
            double res = TaskDouble(number);
            double_results.push_back(res); // to disable compiler optimizations
        }
        std::chrono::steady_clock::time_point end_iteration =
            std::chrono::steady_clock::now();
        const uint64_t duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_iteration -
                                                                  begin_iteration)
                .count();
        std::cout << "Double Iteration " << i_iteration << " - " << duration << " microseconds\n"; 
        iterationsTime.push_back(duration);
        iterationTime += duration;
    }
    averageDoubleIterationTime = iterationTime / ITERATIONS_COUNT;

    std::cout << "END\n";

    std::ofstream csvFile;
    if (!isFileExists(statFileName)) {
        csvFile.open(statFileName, std::ios_base::app);
        csvFile << "PModel;Task;OpType;Opt;InsCount;Timer;Time[µs];LNum;AvTime[µs];AbsErr[µs];RelErr[%];NTypicalTasks;TaskPerf[Tasks per second]\n";   
    } else {
        csvFile.open(statFileName, std::ios_base::app);
    }


    for (uint32_t i = 0; i < ITERATIONS_COUNT * 3; ++i){
        uint64_t iteration_time = iterationsTime.at(i);
        float perfomance = static_cast<float>(TASKS_NUMBERS_COUNT) / static_cast<float>(iteration_time) * 1000000; // µs -> s
        float averageTaskTime = static_cast<float>(iteration_time) / static_cast<float>(TASKS_NUMBERS_COUNT);
        std::string operand_type;
        unsigned long averageIterationTime;
        switch (i / 10)
        {
        case 0:
            operand_type = "int";
            averageIterationTime = averageIntIterationTime;
            break;
        case 1:
            operand_type = "float";
            averageIterationTime = averageFloatIterationTime;
            break;
        case 2:
            operand_type = "double";
            averageIterationTime = averageDoubleIterationTime;
            break;
        default:
            operand_type = "unknown";
            averageIterationTime = 0L;
            break;
        }
        csvFile << "Intel(R) Core(TM) i7-4720HQ CPU @ 2.60GHz"
                << ";"
                << "Basic operations (+ - * /)"
                << ";"
                << operand_type << ";"
                << optimizationFlag << ";"
                << 5 << ";"
                << "std::chrono::steady_clock::now()"
                << ";"
                << iteration_time << ";"
                << i % 10 << ";"
                << averageTaskTime << ";"                                                                                                                                      // AvTime
                << static_cast<long>(averageIterationTime - iteration_time) << ";"                                                                                             // AbsError
                << static_cast<float>(static_cast<long>(averageIterationTime - iteration_time)) / static_cast<float>(static_cast<long>(averageIterationTime)) * 100.0f << "%;" // RelError
                << TASKS_NUMBERS_COUNT << ";"
                << perfomance
                << "\n";
    }
    csvFile.close();
}