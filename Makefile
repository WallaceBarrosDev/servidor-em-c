run:
	@clear
	@gcc -o ./bin/program ./src/*.c -lm -Wall -Wextra -Wpedantic
	@./bin/program
