# Parallel data processing - Lab 2 - Benchmark

In this work you need to run some programming task that calculates somethig with mathematical operations (sin, cos, multiplication, ...). You need to run it multiple times to measure run time. Then you can calculate CPU perfomance and compare it with CPU frequency.

## How to run

```bash
make bench_float_o0
make bench_float_o3
make bench_double_o0
make bench_double_o3

# All benchmarks
make bench_float_o0 && make bench_float_o1 && make bench_float_o2 && make bench_float_o3 && make bench_double_o0 && make bench_double_o1 && make bench_double_o2 && make bench_double_o3
```

## Results

```text
========
  -- FAST inverse square root (float) benchmark (N=1000000, i=10, opt=O3) --  
========
Total time (all iterations): 21274[µs].
Average time (of one iteration): 2127[µs].
Average perfomance: 4.70146e+08 tasks per second.

>>> cpu_freq = 3.4 * 10 ** 9
>>> bench_perf = 4.7 * 10 ** 8
>>> tacts_per_task = cpu_freq / bench_perf
>>> tacts_per_task
7.23404255319149
```
