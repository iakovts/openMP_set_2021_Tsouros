all: comp

OUT_FILE=tsouros1
build:
	@gcc -Wall -fopenmp -fsanitize=address Tsouros_exe1.c -o $(OUT_FILE) -lm

run: build
	@for number in 1 2 3 4; do \
		./$(OUT_FILE) -t $$number -N 2000 ; \
		done
clean:
	@rm -f res*_threads.txt
	@rm -f $(OUT_FILE)



