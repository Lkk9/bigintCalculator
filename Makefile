all: calculator

calculator: src/*.c
	$(CC) -o build/calculator src/*.c -Wall -Wextra -Wpedantic