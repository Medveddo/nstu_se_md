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
  // std::cout << "Generating data = "
  //           << std::chrono::duration_cast<std::chrono::microseconds>(
  //                  end_random - begin_random)
  //                  .count()
  //           << "[µs]" << std::endl;

  return numbers;
}

// Fast inverse square root
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
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

float Isqrt(float number)
{
  return 1 / sqrt(number);
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

  // std::cout << "Calculating = " << duration << "[µs]" << std::endl;
  return duration;
}

uint64_t FloatBenchSlow(std::vector<float> &numbers, std::vector<float> &results)
{
  std::chrono::steady_clock::time_point begin_calc =
      std::chrono::steady_clock::now();

  float result;
  for (const auto &number : numbers)
  {
    result = 2.0F + Isqrt(number);
    results.push_back(result);
  }

  std::chrono::steady_clock::time_point end_calc =
      std::chrono::steady_clock::now();

  const uint64_t duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end_calc -
                                                            begin_calc)
          .count();

  // std::cout << "Calculating = " << duration << "[µs]" << std::endl;
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
  conv.i = 0x5FE6EB50C7B537A9 - (conv.i >> 1);
  conv.f *= threehalfs - x2 * conv.f * conv.f;
  return conv.f;
}

double Isqrt_double(double number)
{
  return 1 / sqrt(number);
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

  // std::cout << "Calculating = " << duration << "[µs]" << std::endl;
  return duration;
}

uint64_t DoubleBenchSlow(std::vector<double> &numbers, std::vector<double> &results)
{
  std::chrono::steady_clock::time_point begin_calc =
      std::chrono::steady_clock::now();

  double result;
  for (const auto &number : numbers)
  {
    result = 2.0 + Isqrt_double(number);
    results.push_back(result);
  }

  std::chrono::steady_clock::time_point end_calc =
      std::chrono::steady_clock::now();

  const uint64_t duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end_calc -
                                                            begin_calc)
          .count();

  // std::cout << "Calculating = " << duration << "[µs]" << std::endl;
  return duration;
}

// Double - end

int main(int argc, char const *argv[])
{
  if (argc < 4)
  {
    std::cout << "Operand type and optimization flag string required!";
    return 1;
  }

  std::string operandType = argv[1];
  std::string optimizationFlag = argv[2];
  std::string operationSpeedVersion = argv[3];
  const bool isFloatOperand = static_cast<bool>(!operandType.compare("float"));
  const bool isDoubleOperand = static_cast<bool>(!operandType.compare("double"));
  const bool isSlowVersion = static_cast<bool>(!operationSpeedVersion.compare("isqrt"));
  std::string taskName = isSlowVersion ? "Inverse square root" : "FAST inverse square root";

  // Variables
  const int NUMBERS_COUNT = 100000000;
  const float RAND_MAX_ = 5.0F;
  const double RAND_MAX_D = 5.0;
  const int ITERATIONS_COUNT = 10;

  // Setup seed for random
  srand(static_cast<unsigned>(time(0)));

  std::vector<unsigned long> iterationsTime;
  iterationsTime.reserve(ITERATIONS_COUNT);

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
      if (!isSlowVersion)
      {
        iterationTime = FloatBench(numbers, results);
      }
      else
      {
        iterationTime = FloatBenchSlow(numbers, results);
      }
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
      if (!isSlowVersion)
      {
        iterationTime = DoubleBench(numbers, results);
      }
      else
      {
        iterationTime = DoubleBenchSlow(numbers, results);
      }
      totalTime += iterationTime;
      iterationsTime.push_back(iterationTime);
      results.clear();
    }
  }

  std::ostringstream stringStream;
  stringStream << "  -- " << taskName << " (" << operandType << ") benchmark (N=" << NUMBERS_COUNT << ", i=" << ITERATIONS_COUNT << ", opt=" << optimizationFlag << ") --  ";
  std::string benchmarkHeader = stringStream.str();

  stringStream.str("");
  stringStream.clear();
  stringStream << "Total time (all iterations): " << totalTime << "[µs]." << std::endl
               << "Average time (of one iteration): " << totalTime / ITERATIONS_COUNT << "[µs]." << std::endl
               << "Average perfomance: " << static_cast<float>(NUMBERS_COUNT) / static_cast<float>(totalTime / ITERATIONS_COUNT) * 1000000 << " tasks per second." << std::endl;
  std::string benchmarkResults = stringStream.str();

  std::cout << "========" << std::endl
            << benchmarkHeader << std::endl
            << "========" << std::endl
            << benchmarkResults;

  std::ofstream csvFile;
  csvFile.open("results.csv", std::ios_base::app);
  csvFile << "PModel;Task;OpType;Opt;InsCount;Timer;Time[µs];LNum;AvTime[µs];AbsErr;RelErr;NTypicalTasks;TaskPerf[Tasks per second]\n";

  unsigned int iterationNumber = 0;
  float perfomance;
  float averageTaskTime;

  for (const auto &time : iterationsTime)
  {
    perfomance = static_cast<float>(NUMBERS_COUNT) / static_cast<float>(time) * 1000000; // µs -> s
    averageTaskTime = static_cast<float>(time) / static_cast<float>(NUMBERS_COUNT);
    csvFile << "Intel(R) Core(TM) i7-4720HQ CPU @ 2.60GHz"
            << ";"
            << taskName
            << ";"
            << operandType << ";"
            << optimizationFlag << ";"
            << taskOperationsCount << ";"
            << "std::chrono::steady_clock::now()"
            << ";"
            << time << ";"
            << iterationNumber++ << ";"
            << averageTaskTime << ";" // AvTime
            << 0 << ";"               // AbsError
            << 0 << ";"               // RelError
            << NUMBERS_COUNT << ";"
            << perfomance << ";"
            << "\n";
  }

  csvFile.close();

  // Result comparation
  bool showResultsComparation = 0;
  if (showResultsComparation)
  {
    float example_x = 3.1415F;
    float fast_result = Q_rsqrt(example_x);
    float regular_result = Isqrt(example_x);
    float diff = regular_result - fast_result;
    std::cout << "\nInput x (float): " << example_x << std::endl
              << "math::sqrt: " << regular_result << std::endl
              << "Fast ISqrt: " << fast_result << std::endl
              << "Diff: " << diff
              << "(" << diff / regular_result * 100 << "%)" << std::endl;

    double example_y = 3.1415;
    double fast_d_result = Q_rsqrt_double(example_y);
    double regular_d_result = Isqrt_double(example_y);
    double diff_d = regular_d_result - fast_d_result;
    std::cout << "\nInput x (double): " << example_y << std::endl
              << "math::sqrt: " << regular_d_result << std::endl
              << "Fast ISqrt: " << fast_d_result << std::endl
              << "Diff: " << diff_d
              << "(" << diff_d / regular_d_result * 100 << "%)" << std::endl;
  }

  return 0;
}
