all: build run
# without gencode 50 --- (error code no kernel image is available for execution on the device)!
build:
	nvcc -gencode=arch=compute_50,code=sm_50 -Wno-deprecated-gpu-targets main.cu -o main.out

run:
	./main.out

prof:
	nvprof ./main.out