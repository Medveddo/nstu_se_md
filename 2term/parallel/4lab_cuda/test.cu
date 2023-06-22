#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include "stdio.h"
#include <cuda_runtime.h>

__global__ void vectorAdd(int *a, int *b, int *c, int N)
{
    int threadId = (blockIdx.x * blockDim.x) + threadIdx.x;

    if (threadId < N)
    {
        c[threadId] = a[threadId] + b[threadId];
    }
}

void verify_results(std::vector<int> a, std::vector<int> b, std::vector<int> c)
{
    for (int i = 0; i < a.size(); i++)
    {
        assert(c[i] == a[i] + b[i]);
    }
}

int main()
{
    constexpr int N = 1 << 16;
    size_t bytes = sizeof(int) * N;

    std::vector<int> a(N);
    std::vector<int> b(N);
    std::vector<int> c(N);

    std::generate(std::begin(a), std::end(a), []()
                  { return rand() % 100; });
    std::generate(std::begin(b), std::end(b), []()
                  { return rand() % 100; });

    int *device_a, *device_b, *device_c;
    cudaError_t err = cudaSuccess;
    err = cudaMalloc(&device_a, bytes);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    err = cudaMalloc(&device_b, bytes);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector B (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    err = cudaMalloc(&device_c, bytes);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector C (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(device_a, a.data(), bytes, cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        fprintf(stderr,
                "Failed to copy vector A from host to device (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    err = cudaMemcpy(device_b, b.data(), bytes, cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        fprintf(stderr,
                "Failed to copy vector B from host to device (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    int NUM_THREADS = 1 << 8; // 1024

    int NUM_BLOCKS = (N + NUM_THREADS - 1) / NUM_THREADS;
    printf("CUDA kernel launch with %d blocks of %d threads\n", NUM_BLOCKS,
           NUM_THREADS);
    vectorAdd<<<NUM_BLOCKS, NUM_THREADS>>>(device_a, device_b, device_c, N);
    err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(c.data(), device_c, bytes, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess)
    {
        fprintf(stderr,
                "Failed to copy vector C from device to host (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    verify_results(a, b, c);

    cudaFree(device_a);
    cudaFree(device_b);
    cudaFree(device_c);

    return 0;
}