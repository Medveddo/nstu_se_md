mm_1_1000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 1000 1 && rm ./dgemm"

mm_2_1000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 1000 2 && rm ./dgemm"

mm_3_1000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 1000 3 && rm ./dgemm"

mm_4_1000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 1000 4 && rm ./dgemm_avx"

test:
	bash -c "g++ -O0 -Wall -mavx test.cpp -o test && ./test && rm ./test"

1: mm_1_1000
2: mm_2_1000
3: mm_3_1000
4: mm_4_1000

mm_1_2000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 2000 1 && rm ./dgemm"
	
mm_1_5000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 5000 1 && rm ./dgemm"
	
mm_1_10000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 10000 1 && rm ./dgemm"

mm_2_2000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 2000 2 && rm ./dgemm"
	
mm_2_5000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 5000 2 && rm ./dgemm"
	
mm_2_10000:
	bash -c "g++ -O0 -Wall dgemm.cpp -o dgemm && ./dgemm 10000 2 && rm ./dgemm"
