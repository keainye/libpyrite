GCC_LIB_DIRS = $(wildcard /usr/lib/gcc/$(shell uname -m)-linux-gnu/*)

build: src/*.cc
	g++ -c src/*.cc
	ar -crv libpyrite.a *.o

install: build uninstall
	mkdir /usr/include/pyrite
	cp src/*.h /usr/include/pyrite/
	$(foreach dir, $(GCC_LIB_DIRS), cp libpyrite.a $(dir)/)
	@echo "[ok] Installation finished."

uninstall:
	-rm -rf /usr/include/pyrite
	-$(foreach dir, $(GCC_LIB_DIRS), rm $(dir)/libpyrite.a)

clean:
	rm -rf *.o
	rm -rf *.a
