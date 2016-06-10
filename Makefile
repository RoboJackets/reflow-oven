all:
	mkdir build || true
	cd build && $(MAKE) -C ../src
