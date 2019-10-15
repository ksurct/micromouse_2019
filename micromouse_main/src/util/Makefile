.PHONY: all

.PHONY: clean
clean:
	rm -rf conversions.o queue.o queue_test.o queue_test

.PHONY: test
test: queue_test
	./queue_test

queue_test: queue.o queue_test.o
	$(CXX) -o $@ $^
