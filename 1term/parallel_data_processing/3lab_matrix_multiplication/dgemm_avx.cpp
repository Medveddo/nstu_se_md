#include <iostream>
#include <string>
#include <chrono>
#include <immintrin.h>

struct ThreeMatrixHandler
{
    double **a;
    double **b;
    double **c;
};

ThreeMatrixHandler *CreateThreeConstMatrixes(unsigned int N)
{
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
            matrixes->a[i][j] = static_cast<double>(i);
            matrixes->b[i][j] = static_cast<double>(j);
        }
    }

    return matrixes;
}

void ShowMatrix(double **m, const char *name, const unsigned int N)
{
    std::cout << "Matrix '" << name << "'" << std::endl;
    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void ShowMatrixes(ThreeMatrixHandler *matrixes)
{
    const unsigned int N = 4;

    ShowMatrix(matrixes->a, "A", N);
    ShowMatrix(matrixes->b, "B", N);
    ShowMatrix(matrixes->c, "C", N);
}

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

void MatrixToFlat(double **source, double *dest, unsigned int N)
{
    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            dest[i * N + j] = source[i][j];
        }
    }
}

void FlatToMatrix(double *source, double **dest, unsigned int N)
{
    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            dest[i][j] = source[i * N + j];
        }
    }
}

// Blocks multiplication
void DGEMM_3(unsigned int N, ThreeMatrixHandler *matrixes)
{
    double *a = new double[N * N];
    double *b = new double[N * N];
    double *c = new double[N * N];

    MatrixToFlat(matrixes->a, a, N);
    MatrixToFlat(matrixes->b, b, N);
    MatrixToFlat(matrixes->c, c, N);

    double *a0, *b0, *c0;

    const unsigned int blockSize = 20;

    unsigned int i, j, k, i0, j0, k0;

    unsigned int n = N;

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

    FlatToMatrix(a, matrixes->a, N);
    FlatToMatrix(b, matrixes->b, N);
    FlatToMatrix(c, matrixes->c, N);

    delete[] a;
    delete[] b;
    delete[] c;
}

// Basic matrix multiplication but with AVX Vectorization
void DGEMM_4(unsigned int N, ThreeMatrixHandler *matrixes)
{
    double *a = new double[N * N];
    double *b = new double[N * N];
    double *c = new double[N * N];

    MatrixToFlat(matrixes->a, a, N);
    MatrixToFlat(matrixes->b, b, N);
    MatrixToFlat(matrixes->c, c, N);

    unsigned int n = N;

    const unsigned int avxVectorDoublesCapacity = 4; // 256 / 64

    // https://www.jsums.edu/robotics/files/2016/12/FECS17_Proceedings-FEC3555.pdf?x14279
    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < n; j += avxVectorDoublesCapacity)
        { 
            __m256d m0 = _mm256_setzero_pd();
            for (unsigned int k = 0; k < n; k++)
            {
                __m256d m1 = _mm256_broadcast_sd(a + i * n + k);
                // https://stackoverflow.com/questions/32612190/how-to-solve-the-32-byte-alignment-issue-for-avx-load-store-operations
                __m256d m2 = _mm256_loadu_pd((b + k * n + j));
                __m256d m3 = _mm256_mul_pd(m1, m2);
                m0 = _mm256_add_pd(m0, m3);
            }
            _mm256_storeu_pd(c + i * n + j, m0);
        }
    }

    FlatToMatrix(a, matrixes->a, N);
    FlatToMatrix(b, matrixes->b, N);
    FlatToMatrix(c, matrixes->c, N);

    delete[] a;
    delete[] b;
    delete[] c;
}

// Blocks multiplication with AVX
// TODO: This is does not work !
void DGEMM_5(unsigned int N, ThreeMatrixHandler *matrixes)
{
    double *a = new double[N * N];
    double *b = new double[N * N];
    double *c = new double[N * N];

    MatrixToFlat(matrixes->a, a, N);
    MatrixToFlat(matrixes->b, b, N);
    MatrixToFlat(matrixes->c, c, N);

    double *a0, *b0, *c0;

    const unsigned int blockSize = 4;
    const unsigned int avxVectorDoublesCapacity = 4; // 256 / 64

    unsigned int i, j, k, i0, j0, k0;

    unsigned int n = N;

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
                        // ???????????????????????????????????????????????????????
                        double temp[4];
                        __m256d m0 = _mm256_setzero_pd();
                        for (j0 = 0; j0 < blockSize; j0+=avxVectorDoublesCapacity)
                        {
                            __m256d m1 = _mm256_broadcast_sd(&a0[k0]);
                            __m256d m2 = _mm256_loadu_pd((&b0[j0]));
                            __m256d m3 = _mm256_mul_pd(m1, m2);   
                            m0 = _mm256_add_pd(m0, m3);
                        }
                        _mm256_storeu_pd(temp, m0);
                        for (int ii = 0; ii < 4; ++ii){
                            c0[j0] += temp[ii];
                        }
                    }
                }
            }
        }
    }

    FlatToMatrix(a, matrixes->a, N);
    FlatToMatrix(b, matrixes->b, N);
    FlatToMatrix(c, matrixes->c, N);

    delete[] a;
    delete[] b;
    delete[] c;
}

// Matrix 'A'
// 0 0 0 0
// 1 1 1 1
// 2 2 2 2
// 3 3 3 3
// Matrix 'B'
// 0 1 2 3
// 0 1 2 3
// 0 1 2 3
// 0 1 2 3
// Matrix 'C'
// 0 0 0 0
// 0 4 8 12
// 0 8 16 24
// 0 12 24 36

int MultiplicationTest()
{
    auto matrixes = CreateThreeConstMatrixes(4);

    // ShowMatrixes(matrixes);
    // DGEMM_1(4, matrixes);
    // DGEMM_2(4, matrixes);
    // DGEMM_3(4, matrixes);
    DGEMM_4(4, matrixes);
    // DGEMM_5(4, matrixes);
    ShowMatrixes(matrixes);

    DeallocateMatixes(4, matrixes);
    delete matrixes;
    return 0;
}

int main(int argc, char const *argv[])
{
    // MultiplicationTest();
    // return 0;

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
        degmm_func_pointer = DGEMM_3;
        break;
    case 4:
        degmm_func_pointer = DGEMM_4;
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
