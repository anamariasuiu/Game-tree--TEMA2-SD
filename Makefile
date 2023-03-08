build: tema2.c
	gcc -Wall -o minimax tema2.c -lm -std=c11 -ggdb3
	

.PHONY : clean
clean:
	rm -f minimax
	rm -f .out