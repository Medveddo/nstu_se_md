all:
	mpic++ -o openmpi.out openmpi.cpp && mpirun -n 4 ./openmpi.out
	# mpic++ -o openmpi.out openmpi.cpp && mpirun --use-hwthread-cpus -n 5 ./openmpi.out

build:
	mpic++ -o openmpi.out openmpi.cpp

run:
	time mpirun -n 4 ./openmpi.out
