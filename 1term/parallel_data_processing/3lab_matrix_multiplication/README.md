# Parallel data processing - Lab 3 - Matrix Multiplication

## Run

```shell
g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 1000 1
# first arg - matrix size
# second arg - function version (1 - basic, 2 - row optimization, 3 - block optimization, 4 - AVX)
```

or

```shell
make 1
make 2
make 3
make 4
```

## Count of cache misses

```shell
sudo apt install linux-perf
```

```shell
sudo perf stat -e cache-misses ./dgemm 1000 1
```
