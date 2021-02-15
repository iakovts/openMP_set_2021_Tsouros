all: comp

OUT_FILE=tsouros2_matmul
build:
	@gcc -Wall -fopenmp -fsanitize=address matmul_omp_tsouros.c -o $(OUT_FILE) -lm

run: build
	@for number in 1 2 4; do \
		./$(OUT_FILE) -t $$number -N 2000 -p; \
		done
verify: build run
	@./$(OUT_FILE) -N 2000 -t 1
	python ../compare_res.py --files res4_threads.txt res_serial.txt

clean:
	@rm -f res*_threads.txt
	@rm -f res_serial.txt
	@rm -f $(OUT_FILE)

