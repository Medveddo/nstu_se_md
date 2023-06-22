#include <chrono>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <cuda_runtime.h>
#include <numeric>

__global__ void countTargetLengthSequences(int *a, int *c, int targetLength, int N)
{
    int threadId = (blockIdx.x * blockDim.x) + threadIdx.x;

    if (threadId >= N) {return;} // boundary check

    int currentValue = a[threadId];
    int iterationsCount = 1;
    while (currentValue != 1 && currentValue > 0) // for integer overflow (e.g. 113383)
    {
        iterationsCount++;
        currentValue = currentValue % 2 == 0 ? currentValue / 2 : 3 * currentValue + 1;
    }
    if (currentValue < 0) {return;}
    if (iterationsCount == targetLength) {
        c[threadId] = 1;
    }
}

int main()
{
    auto startTime = std::chrono::steady_clock::now();
    int intervalStart = 1;
    int intervalEnd = 250000;
    int targetLength = 24;

    int foundSequences = 0;

    int N = (intervalEnd - intervalStart);
    size_t bytes = sizeof(int) * N;

    std::vector<int> a(N);
    std::vector<int> c(N);

    for (int i = intervalStart; i < intervalEnd; i++) {
        a[i] = i;
    }
    int *device_a, *device_c;
    cudaError_t err = cudaSuccess;
    err = cudaMalloc(&device_a, bytes);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n",
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

    auto startKernelTime = std::chrono::steady_clock::now();
    err = cudaMemcpy(device_a, a.data(), bytes, cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        fprintf(stderr,
                "Failed to copy vector A from host to device (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    int NUM_THREADS = 1024;

    int NUM_BLOCKS = (N + NUM_THREADS - 1) / NUM_THREADS;
    printf("CUDA kernel launch with %d blocks of %d threads\n", NUM_BLOCKS,
           NUM_THREADS);


    countTargetLengthSequences<<<NUM_BLOCKS, NUM_THREADS>>>(device_a, device_c, targetLength, N);
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
    auto endKernelTime = std::chrono::steady_clock::now();
    
    foundSequences = std::accumulate(c.begin(), c.end(), 0);
    auto endKernelAndAccumTime = std::chrono::steady_clock::now();
    
    cudaFree(device_a);
    cudaFree(device_c);

    auto endTime = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime -
                                                            startTime).count();
    const auto durationKernel = std::chrono::duration_cast<std::chrono::microseconds>(endKernelTime -
                                                            startKernelTime).count();
    const auto durationKernelAccum = std::chrono::duration_cast<std::chrono::microseconds>(endKernelAndAccumTime -
                                                            startKernelTime).count();
    std::cout << "(CUDA_KernelOnly) - Interval[" << intervalStart << ":" << intervalEnd << "]Len["<<targetLength<<"] Found: "
    << foundSequences << " sequences Took: " << durationKernel << "[µs]"<< std::endl;
    std::cout << "(CUDA_KernelAndAccum) - Interval[" << intervalStart << ":" << intervalEnd << "]Len["<<targetLength<<"] Found: "
    << foundSequences << " sequences Took: " << durationKernelAccum << "[µs]"<< std::endl;
    std::cout << "(CUDA_Total) - Interval[" << intervalStart << ":" << intervalEnd << "]Len["<<targetLength<<"] Found: "
    << foundSequences << " sequences Took: " << duration << "[µs]"<< std::endl;
    return 0;
}