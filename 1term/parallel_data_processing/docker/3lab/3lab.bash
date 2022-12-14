#!/bin/bash
g++ -O0 -Wall -mavx 3lab.cpp -o 3lab
# echo "#1 For i j k (simple)"
# perf stat -e cache-misses ./3lab 2000 1 0
# echo "#4 For i j k (AVX)"
# perf stat -e cache-misses ./3lab 2000 4 0
# echo "#2 For i k j (row)"
# perf stat -e cache-misses ./3lab 2000 2 0
# echo "#3 Block (size 2)"
# perf stat -e cache-misses ./3lab 2000 3 2
# echo "#3 Block (size 4)"
# perf stat -e cache-misses ./3lab 2000 3 4
# echo "#3 Block (size 8)"
# perf stat -e cache-misses ./3lab 2000 3 8
# echo "#3 Block (size 16)"
# perf stat -e cache-misses ./3lab 2000 3 16
perf stat -e cache-misses ./3lab 2000 3 64
