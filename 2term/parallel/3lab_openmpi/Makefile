# mpic++ -o openmpi.out openmpi.cpp && mpirun --use-hwthread-cpus -n 5 ./openmpi.out
all:
	mpic++ -o openmpi.out openmpi.cpp && mpirun -n 4 ./openmpi.out

build:
	mpic++ -o openmpi.out openmpi.cpp

run:
	mpirun -n 4 ./openmpi.out
