#include <iostream>
#include <string>
#include <chrono>

struct ThreeMatrixHandler
{
    double **a;
    double **b;
    double **c;
};

ThreeMatrixHandler *CreateThreeMatrixWithRandomElements(unsigned int N)
{
    std::chrono::steady_clock::time_point begin_random =
        std::chrono::steady_clock::now();

    auto matrixes = new ThreeMatrixHandler;

    double **first_matrix = new double *[N];
    double **second_matrix = new double *[N];
    double **third_matrix = new double *[N];

    for (unsigned int i = 0; i < N; ++i)
    {
        first_matrix[i] = new double[N];
        second_matrix[i] = new double[N];
        third_matrix[i] = new double[N];
    }

    matrixes->a = first_matrix;
    matrixes->b = second_matrix;
    matrixes->c = third_matrix;

    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            matrixes->a[i][j] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
            matrixes->b[i][j] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        }
    }

    std::chrono::steady_clock::time_point end_random =
        std::chrono::steady_clock::now();
    std::cout << "Generating data = "
              << std::chrono::duration_cast<std::chrono::microseconds>(
                     end_random - begin_random)
                     .count()
              << "[µs]" << std::endl;

    return matrixes;
}

void DeallocateMatixes(unsigned int N, ThreeMatrixHandler *matrixes)
{
    for (unsigned int i = 0; i < N; ++i)
    {
        delete[] matrixes->a[i];
        delete[] matrixes->b[i];
        delete[] matrixes->c[i];
    }
    delete[] matrixes->a;
    delete[] matrixes->b;
    delete[] matrixes->c;
}

// Basic matrix multiplication
void DGEMM_1(unsigned int N, ThreeMatrixHandler *matrixes)
{
    double **a = matrixes->a;
    double **b = matrixes->b;
    double **c = matrixes->c;

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int j = 0; j < N; j++)
        {
            for (unsigned int k = 0; k < N; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Rows multiplication
void DGEMM_2(unsigned int N, ThreeMatrixHandler *matrixes)
{
    double **a = matrixes->a;
    double **b = matrixes->b;
    double **c = matrixes->c;

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int k = 0; k < N; k++) // j and k for loops are swaped
        {
            for (unsigned int j = 0; j < N; j++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Blocks multiplication
void DGEMM_3(unsigned int N, ThreeMatrixHandler *matrixes)
{
    double *a = (double *)(void *)(matrixes->a);
    double *b = (double *)(void *)(matrixes->b);
    double *c = (double *)(void *)(matrixes->c);

    double *a0, *b0, *c0;

    const unsigned int blockSize = 2;

    unsigned int i, j, k, i0, j0, k0;

    unsigned int n = N / blockSize;

    for (i = 0; i < n; i += blockSize)
    {
        for (j = 0; j < n; j += blockSize)
        {
            for (k = 0; k < n; k += blockSize)
            {
                for (i0 = 0, c0 = (c + i * n + j),
                    a0 = (a + i * n + k);
                     i0 < blockSize;
                     ++i0, c0 += n, a0 += n)
                {
                    for (k0 = 0, b0 = (b + k * n + j);
                         k0 < blockSize; ++k0, b0 += n)
                    {
                        for (j0 = 0; j0 < blockSize; ++j0)
                        {
                            c0[j0] += a0[k0] * b0[j0];
                        }
                    }
                }
            }
        }
    }

    // https://gist.github.com/metallurgix/8ee5262ed818730b5cc0
    // double **a = matrixes->a;
    // double **b = matrixes->b;
    // double **c = matrixes->c;
    // unsigned int bi = 0;
    // unsigned int bj = 0;
    // unsigned int bk = 0;
    // unsigned int i = 0;
    // unsigned int j = 0;
    // unsigned int k = 0;
    // unsigned int blockSize = 10;
    // unsigned int n = N;

    // for (bi = 0; bi < n; bi += blockSize)
    //     for (bj = 0; bj < n; bj += blockSize)
    //         for (bk = 0; bk < n; bk += blockSize)
    //             for (i = 0; i < blockSize; i++)
    //                 for (j = 0; j < blockSize; j++)
    //                     for (k = 0; k < blockSize; k++)
    //                         c[bi + i][bj + j] += a[bi + i][bk + k] * b[bk + k][bj + j];
}

int main(int argc, char const *argv[])
{
    const uint8_t MATRIX_DIMMENSTIONS_SIZE_ARGUMENT_INDEX = 1;
    const uint8_t FUNCTION_VERSION_ARGUMENT_INDEX = 2;
    const unsigned int iterationsCount = 1;

    if (argc < 3)
    {
        std::cout << "Matrix dimmensions size and function version are required as program arguments\n  e.g. ./binary 100 1" << std::endl;
        return 1;
    }
    // Setup seed for random
    srand(static_cast<unsigned>(time(0)));

    std::string nAsStr = argv[MATRIX_DIMMENSTIONS_SIZE_ARGUMENT_INDEX];
    std::string version = argv[FUNCTION_VERSION_ARGUMENT_INDEX];
    const unsigned int versionNumber = static_cast<unsigned int>(std::stoi(version));

    const unsigned int N = static_cast<unsigned int>(std::stoi(nAsStr));

    void (*degmm_func_pointer)(unsigned int, ThreeMatrixHandler *);

    switch (versionNumber)
    {
    case 1:
        degmm_func_pointer = DGEMM_1;
        break;
    case 2:
        degmm_func_pointer = DGEMM_2;
        break;
    case 3:
        // Got an "munmap_chunk(): invalid pointer"
        degmm_func_pointer = DGEMM_3;
        break;
    default:
        std::cout << "ERROR - Unknown version number" << std::endl;
        return 1;
    }

    auto matrixes = CreateThreeMatrixWithRandomElements(N);
    std::chrono::steady_clock::time_point begin_calc =
        std::chrono::steady_clock::now();

    for (unsigned int i = 0; i < iterationsCount; ++i)
    {
        degmm_func_pointer(N, matrixes);
    }

    std::chrono::steady_clock::time_point end_calc =
        std::chrono::steady_clock::now();

    int64_t diff_micro = std::chrono::duration_cast<std::chrono::microseconds>(
                             end_calc - begin_calc)
                             .count();
    std::cout << "  -- DGEMM (v" << versionNumber << ") (N = " << N << ", "
              << iterationsCount << " iterations) --" << std::endl
              << "Average =\t"
              << diff_micro / iterationsCount
              << "[µs]" << std::endl
              << "  Total =\t"
              << diff_micro
              << "[µs]" << std::endl;

    DeallocateMatixes(N, matrixes);
    delete matrixes;
    return 0;
}
