.PHONY: all
all: movement_test

.PHONY: clean
clean:
	rm -rf movement_test \
		movement.o movement_test.o \
		../localization/localization.o ../localization/probabilistic_maze.o \
		../util/conversions.o

.PHONY: test
test: all
	./movement_test

movement_test: movement.o movement_test.o ../localization/localization.o ../localization/probabilistic_maze.o ../util/conversions.o
	$(CXX) -o $@ $^
