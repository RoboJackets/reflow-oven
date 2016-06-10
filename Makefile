oven:
	$(MAKE) -C src
oven-prog:
	$(MAKE) upload -C src
clean:
	rm -rf build
