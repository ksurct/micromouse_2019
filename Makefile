.PHONY: all
all:
	$(MAKE) -C micromouse_main $@

.PHONY: clean
clean:
	$(MAKE) -C micromouse_main $@

.PHONY: test
test:
	$(MAKE) -C micromouse_main $@
