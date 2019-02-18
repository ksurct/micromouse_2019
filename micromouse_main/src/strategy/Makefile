.PHONY: all
all: strategy_test

.PHONY: clean
clean:
	rm -rf strategy_test \
		strategy.o strategy_test.o

.PHONY: test
test: all
	./strategy_test

strategy_test: strategy.o strategy_test.o
	$(CXX) -o $@ $^
