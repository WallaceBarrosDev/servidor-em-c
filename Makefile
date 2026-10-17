run:
	@clear
	@gcc -o ./bin/program ./src/main.c -lm -Wall -Wextra -Wpedantic
	@./bin/program
