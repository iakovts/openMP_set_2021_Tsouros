all: comp

OUT_FILE=tsouros2_matmul
build:
	@gcc -Wall -fopenmp -fsanitize=address matmul_omp_tsouros.c -o $(OUT_FILE) -lm

run: build
	@for number in 1 2 4; do \
		./$(OUT_FILE) -t $$number -N 2000 ; \
		done
clean:
	@rm -f res*_threads.txt
	@rm -f $(OUT_FILE)

