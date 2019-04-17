.PHONY: all
all: probabilistic_maze_test localization_test

.PHONY: clean
clean:
	rm -rf probabilistic_maze_test probabilistic_maze.o probabilistic_maze_test.o \
		localization_test localization.o localization_test.o ../util/conversions.o

.PHONY: test
test: all
	./probabilistic_maze_test
	./localization_test

probabilistic_maze_test: probabilistic_maze.o probabilistic_maze_test.o 
	$(CXX) -o $@ $^

localization_test: localization.o probabilistic_maze.o localization_test.o ../util/conversions.o
	$(CXX) -o $@ $^
