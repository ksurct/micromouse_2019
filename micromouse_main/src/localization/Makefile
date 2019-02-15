.PHONY: all
all: probabilistic_maze_test

.PHONY: clean
clean:
	rm -rf probabilistic_maze_test \
		probabilistic_maze.o probabilistic_maze_test.o

.PHONY: test
test: all
	./probabilistic_maze_test

probabilistic_maze_test: probabilistic_maze.o probabilistic_maze_test.o
	$(CXX) -o $@ $^
