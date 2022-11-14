#include <bit>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <typeinfo>
#include <vector>

// Float

std::vector<float> GenerateFloatData(int NumbersCount, float RandMax)
{

  std::chrono::steady_clock::time_point begin_random =
      std::chrono::steady_clock::now();

  // Random input data
  std::vector<float> numbers;
  numbers.reserve(NumbersCount);
  for (uint32_t i = 0; i < NumbersCount; ++i)
  {
    // Generate float from 0.0 to 1.0, and append it to vector
    numbers.push_back(static_cast<float>(rand()) /
                      static_cast<float>(RAND_MAX) * RandMax);
  }

  std::chrono::steady_clock::time_point end_random =
      std::chrono::steady_clock::now();
  std::cout << "Generating data = "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   end_random - begin_random)
                   .count()
            << "[µs]" << std::endl;

  return numbers;
}

// Quick inverse square root
//
// ~ 7 operations
// 1. >> 2. - 3. * 4. * 5. * 6. - 7. *=
const unsigned int taskOperationsCount = 7;

float Q_rsqrt(float number)
{
  union
  {
    float f;
    uint32_t i;
  } conv = {number}; // member 'f' set to value of 'number'.
  conv.i = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
  return conv.f;
}

uint64_t FloatBench(std::vector<float> &numbers, std::vector<float> &results)
{
  std::chrono::steady_clock::time_point begin_calc =
      std::chrono::steady_clock::now();

  float result;
  for (const auto &number : numbers)
  {
    result = 2.0F + Q_rsqrt(number);
    results.push_back(result);
  }

  std::chrono::steady_clock::time_point end_calc =
      std::chrono::steady_clock::now();

  const uint64_t duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end_calc -
                                                            begin_calc)
          .count();

  std::cout << "Calculating = " << duration << "[µs]" << std::endl;
  return duration;
}

// Float - end

// Double

std::vector<double> GenerateDoubleData(int NumbersCount, double RandMax)
{

  std::chrono::steady_clock::time_point begin_random =
      std::chrono::steady_clock::now();

  // Random input data
  std::vector<double> numbers;
  numbers.reserve(NumbersCount);
  for (uint32_t i = 0; i < NumbersCount; ++i)
  {
    // Generate double from 0.0 to 1.0, and append it to vector
    numbers.push_back(static_cast<double>(rand()) /
                      static_cast<double>(RAND_MAX) * RandMax);
  }

  std::chrono::steady_clock::time_point end_random =
      std::chrono::steady_clock::now();
  std::cout << "Generating data = "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   end_random - begin_random)
                   .count()
            << "[µs]" << std::endl;

  return numbers;
}

// Quick inverse square root for double
double Q_rsqrt_double(double number)
{
  const double x2 = number * 0.5;
  const double threehalfs = 1.5;

  union
  {
    double f;
    uint64_t i;
  } conv = {number}; // member 'f' set to value of 'number'.
  conv.i = 0x5f3759df - (conv.i >> 1);
  conv.f *= threehalfs - x2 * conv.f * conv.f;
  return conv.f;
}

uint64_t DoubleBench(std::vector<double> &numbers, std::vector<double> &results)
{
  std::chrono::steady_clock::time_point begin_calc =
      std::chrono::steady_clock::now();

  double result;
  for (const auto &number : numbers)
  {
    result = 2.0 + Q_rsqrt_double(number);
    results.push_back(result);
  }

  std::chrono::steady_clock::time_point end_calc =
      std::chrono::steady_clock::now();

  const uint64_t duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end_calc -
                                                            begin_calc)
          .count();

  std::cout << "Calculating = " << duration << "[µs]" << std::endl;
  return duration;
}

// Double - end

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    std::cout << "Operand type and optimization flag string required!";
    return 1;
  }

  std::string operandType = argv[1];
  std::string optimizationFlag = argv[2];
  const bool isFloatOperand = static_cast<bool>(!operandType.compare("float"));
  const bool isDoubleOperand = static_cast<bool>(!operandType.compare("double"));

  // Variables
  const int NUMBERS_COUNT = 100000000;
  const float RAND_MAX_ = 5.0F;
  const double RAND_MAX_D = 5.0;
  const int ITERATIONS_COUNT = 10;

  
  // Setup seed for random
  srand(static_cast<unsigned>(time(0)));
  
  std::vector<unsigned long> iterationsTime;
  iterationsTime.reserve(ITERATIONS_COUNT);

  std::cout << "\n -- Inverse square root (" << operandType << ") benchmark (N="
            << NUMBERS_COUNT << ", i=" << ITERATIONS_COUNT
            << ", opt=" << optimizationFlag << ") --" << std::endl;

  // To calc average iteration time
  unsigned long totalTime = 0;
  unsigned long iterationTime;

  if (isFloatOperand)
  {
    std::vector<float> numbers = GenerateFloatData(NUMBERS_COUNT, RAND_MAX_);
    std::vector<float> results;
    results.reserve(NUMBERS_COUNT);
    for (int i; i < ITERATIONS_COUNT; ++i)
    {

      iterationTime = FloatBench(numbers, results);
      totalTime += iterationTime;
      iterationsTime.push_back(iterationTime);
      results.clear();
    }
  }
  else if (isDoubleOperand)
  {
    std::vector<double> numbers = GenerateDoubleData(NUMBERS_COUNT, RAND_MAX_D);
    std::vector<double> results;
    results.reserve(NUMBERS_COUNT);
    for (int i; i < ITERATIONS_COUNT; ++i)
    {
      iterationTime = DoubleBench(numbers, results);
      totalTime += iterationTime;
      iterationsTime.push_back(iterationTime);
      results.clear();
    }
  }

  std::cout << "Total time: " << totalTime << "[µs]. "
            << "Average: " << totalTime / ITERATIONS_COUNT << "[µs]" << std::endl;

  std::ofstream csvFile;
  csvFile.open("results.csv", std::ios_base::app);
  csvFile <<
  "PModel;Task;OpType;Opt;InsCount;Timer;Time[µs];LNum;AvTime[µs];AbsErr;RelErr;NTypicalTasks;TaskPerf[Tasks per second]\n";
  
  unsigned int iterationNumber = 0;
  float perfomance;
  float averageTaskTime;
  
  for (const auto &time : iterationsTime)
  {
    perfomance = static_cast<float>(NUMBERS_COUNT) / static_cast<float>(time) * 1000000; // µs -> s
    averageTaskTime = static_cast<float>(time) / static_cast<float>(NUMBERS_COUNT);
    csvFile << "Intel(R) Core(TM) i7-4720HQ CPU @ 2.60GHz" << ";"
            << "Fast inverse square root "
               "(https://en.wikipedia.org/wiki/Fast_inverse_square_root)"
            << ";"
            << operandType << ";" 
            << optimizationFlag << ";"
            << taskOperationsCount << ";"
            << "std::chrono::steady_clock::now()" << ";" 
            << time << ";" 
            << iterationNumber++ << ";"
            << averageTaskTime << ";" //AvTime
            << 0 << ";" //AbsError
            << 0 << ";" //RelError
            << NUMBERS_COUNT << ";"
            << perfomance << ";"
            << "\n";
  }

  csvFile.close();
  return 0;
}
