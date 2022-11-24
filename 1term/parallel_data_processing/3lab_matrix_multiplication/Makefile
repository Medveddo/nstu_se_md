mm_1_1000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 1000 1 && rm ./dgemm_avx"

mm_2_1000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 1000 2 && rm ./dgemm_avx"

mm_3_1000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 1000 3 && rm ./dgemm_avx"

mm_4_1000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 1000 4 && rm ./dgemm_avx"

test:
	bash -c "g++ -O0 -Wall -mavx test.cpp -o test && ./test && rm ./test"

1: mm_1_1000
2: mm_2_1000
3: mm_3_1000
4: mm_4_1000

block:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && sudo perf stat -e cache-misses ./dgemm_avx 1000 3 && rm ./dgemm_avx"

mm_1_2000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 2000 1 && rm ./dgemm_avx"
	
mm_1_5000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 5000 1 && rm ./dgemm_avx"
	
mm_1_10000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 10000 1 && rm ./dgemm_avx"

mm_2_2000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 2000 2 && rm ./dgemm_avx"
	
mm_2_5000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 5000 2 && rm ./dgemm_avx"
	
mm_2_10000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 10000 2 && rm ./dgemm_avx"

mm_3_2000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 2000 3 && rm ./dgemm_avx"

mm_3_3000:
	bash -c "g++ -O0 -Wall -mavx dgemm_avx.cpp -o dgemm_avx && ./dgemm_avx 3000 3 && rm ./dgemm_avx"
