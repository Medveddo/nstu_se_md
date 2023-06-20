#!/bin/bash

echo "g++ O0 ..."
g++ -O0 lab2.cpp -o lab2 && ./lab2 1 O0
echo "g++ O1 ..."
g++ -O1 lab2.cpp -o lab2 && ./lab2 1 O1
echo "g++ O2 ..."
g++ -O2 lab2.cpp -o lab2 && ./lab2 1 O2
echo "g++ O3 ..."
g++ -O3 lab2.cpp -o lab2 && ./lab2 1 O3

echo "Results: ..."
cat inner/stats.csv
