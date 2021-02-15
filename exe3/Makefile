all: comp

OUT_FILE=tsouros3_num_diff
PTS=20000000
build:
	@gcc -Wall -fopenmp -fsanitize=address Tsouros_num_diff.c -o $(OUT_FILE) -lm

run: build
	@for number in 1 2 4; do \
		./$(OUT_FILE) -t $$number -N $(PTS) -s 1 -f 2 -p; \
		done
verify: SHELL := /bin/bash
verify: build run
	./$(OUT_FILE) -t 1 -N $(PTS) -s 1 -f 2; \
	cmp -s res4_threads.txt res_serial.txt; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo "Results are the same for all test cases"; \
	else \
		echo "Results are different for different number of threads"; \
	fi \

clean:
	@rm -f res*_threads.txt
	@rm -f res_serial.txt
	@rm -f $(OUT_FILE)
