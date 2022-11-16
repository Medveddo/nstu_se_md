# Parallel data processing - Lab 3 - Matrix Multiplication

## Run

```shell
g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 1000 1
# first arg - matrix size
# second arg - function version (1 - basic, 2 - row optimization, 3 - block optimization (broken), 4 - SSE/AVX ?)
```

or

```shell
make
```

## Count of cache misses

```shell
sudo perf stat -e cache-misses ./dgemm 1000 1
# if you don't have perf you can run
# sudo apt install linux-perf
```
