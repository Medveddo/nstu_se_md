#include <iostream>
#include <string>
#include <chrono>
#include <immintrin.h>

void fun_avx(float *a, float *b, float *c, int n)
{
    int i, k;
    __m256 x, y;
    __m256 *aa = (__m256 *)a;
    __m256 *bb = (__m256 *)b;
    __m256 *cc = (__m256 *)c;
    k = n / 8;
    for (i = 0; i < k; i++)
    {
        x = _mm256_mul_ps(*aa, *aa);
        y = _mm256_mul_ps(*bb, *bb);
        x = _mm256_add_ps(x, y);
        *cc = _mm256_sqrt_ps(x);
        aa++;
        bb++;
        cc++;
    }
    for (i = 0; i < n; i++){
        std::cout << c[i] << " ";
    }
    std::cout << std::endl;
}

void ShowArray(double *arr, int size)
{
    for (int i = 0; i < 4; ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    const int n = 16;

    float a[n] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.};
    float b[n] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.};
    float c[n];

    fun_avx(a, b, c, n);
}