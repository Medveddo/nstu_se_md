#include <immintrin.h>
#include <string>
#include <chrono>
#include <iostream>

struct ThreeMatrix
{
    double **a;
    double **b;
    double **c;
};

ThreeMatrix *CreateThreeMatrixWithRandomElements(unsigned int N)
{
    ThreeMatrix * matrixes = new ThreeMatrix;

    matrixes->a = new double *[N];
    matrixes->b  = new double *[N];
    matrixes->c = new double *[N];

    for (unsigned int i = 0; i < N; ++i)
    {
        matrixes->a[i] = new double[N];
        matrixes->b[i] = new double[N];
        matrixes->c[i] = new double[N];
    }

    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            matrixes->a[i][j] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
            matrixes->b[i][j] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        }
    }

    return matrixes;
}

void DeallocateMatixes(unsigned int N, ThreeMatrix *matrixes)
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

void DGEMM_1(unsigned int N, ThreeMatrix *matrixes)
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

void DGEMM_2(unsigned int N, ThreeMatrix *matrixes)
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

void FlatArrayFromMatrix(double **source, double *dest, unsigned int N)
{
    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            dest[i * N + j] = source[i][j];
        }
    }
}

void MatrixFromFlatArray(double *source, double **dest, unsigned int N)
{
    for (unsigned int i = 0; i < N; ++i)
    {
        for (unsigned int j = 0; j < N; ++j)
        {
            dest[i][j] = source[i * N + j];
        }
    }
}

void DGEMM_3(unsigned int N, ThreeMatrix *matrixes, const uint32_t block_size)
{
    double *a = new double[N * N];
    double *b = new double[N * N];
    double *c = new double[N * N];

    FlatArrayFromMatrix(matrixes->a, a, N);
    FlatArrayFromMatrix(matrixes->b, b, N);
    FlatArrayFromMatrix(matrixes->c, c, N);

    double *a0, *b0, *c0;

    std::cout << "Block size: " << block_size << "\n";

    unsigned int i, j, k, i0, j0, k0;

    unsigned int n = N;

    for (i = 0; i < n; i += block_size)
    {
        for (j = 0; j < n; j += block_size)
        {
            for (k = 0; k < n; k += block_size)
            {
                for (i0 = 0, c0 = (c + i * n + j),
                    a0 = (a + i * n + k);
                     i0 < block_size;
                     ++i0, c0 += n, a0 += n)
                {
                    for (k0 = 0, b0 = (b + k * n + j);
                         k0 < block_size; ++k0, b0 += n)
                    {
                        for (j0 = 0; j0 < block_size; ++j0)
                        {
                            c0[j0] += a0[k0] * b0[j0];
                        }
                    }
                }
            }
        }
    }

    MatrixFromFlatArray(a, matrixes->a, N);
    MatrixFromFlatArray(b, matrixes->b, N);
    MatrixFromFlatArray(c, matrixes->c, N);

    delete[] a;
    delete[] b;
    delete[] c;
}

void DGEMM_4(unsigned int N, ThreeMatrix *matrixes)
{
    double *a = new double[N * N];
    double *b = new double[N * N];
    double *c = new double[N * N];

    FlatArrayFromMatrix(matrixes->a, a, N);
    FlatArrayFromMatrix(matrixes->b, b, N);
    FlatArrayFromMatrix(matrixes->c, c, N);

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int j = 0; j < N; j += 4)
        { 
            __m256d m0 = _mm256_setzero_pd();
            for (unsigned int k = 0; k < N; k++)
            {
                __m256d m1 = _mm256_broadcast_sd(a + i * N + k);
                __m256d m2 = _mm256_loadu_pd((b + k * N + j));
                __m256d m3 = _mm256_mul_pd(m1, m2);
                m0 = _mm256_add_pd(m0, m3);
            }
            _mm256_storeu_pd(c + i * N + j, m0);
        }
    }

    MatrixFromFlatArray(a, matrixes->a, N);
    MatrixFromFlatArray(b, matrixes->b, N);
    MatrixFromFlatArray(c, matrixes->c, N);

    delete[] a;
    delete[] b;
    delete[] c;
}


int main(int argc, char const *argv[])
{
    const uint8_t MATRIX_DIMMENSTIONS_SIZE_ARGUMENT_INDEX = 1;
    const uint8_t FUNCTION_VERSION_ARGUMENT_INDEX = 2;
    const uint8_t BLOCK_SIZE_ARGUMENT_INDEX = 3;
    srand(static_cast<unsigned>(time(0)));
    std::string nAsStr = argv[MATRIX_DIMMENSTIONS_SIZE_ARGUMENT_INDEX];
    std::string version = argv[FUNCTION_VERSION_ARGUMENT_INDEX];
    const uint32_t BLOCK_SIZE = static_cast<uint32_t>(atoi(argv[BLOCK_SIZE_ARGUMENT_INDEX]));
    const unsigned int versionNumber = static_cast<unsigned int>(std::stoi(version));
    const unsigned int N = static_cast<unsigned int>(std::stoi(nAsStr));

    auto matrixes = CreateThreeMatrixWithRandomElements(N);
    std::chrono::steady_clock::time_point begin_calc =
        std::chrono::steady_clock::now();


    switch (versionNumber)
    {
    case 1:
        DGEMM_1(N, matrixes);
        break;
    case 2:
        DGEMM_2(N, matrixes);
        break;
    case 3:
        DGEMM_3(N, matrixes, BLOCK_SIZE);
        break;
    case 4:
        DGEMM_4(N, matrixes);
        break;
    default:
        std::cout << "ERROR - Unknown version number" << std::endl;
        return 1;
    }


    std::chrono::steady_clock::time_point end_calc =
        std::chrono::steady_clock::now();

    int64_t diff_micro = std::chrono::duration_cast<std::chrono::microseconds>(
                             end_calc - begin_calc)
                             .count();
    std::cout << "  -- DGEMM (v" << versionNumber << ") (N = " << N << ") --" << std::endl
              << "  Time =\t"
              << diff_micro
              << "[µs]" << std::endl;

    DeallocateMatixes(N, matrixes);
    delete matrixes;
    return 0;
}
