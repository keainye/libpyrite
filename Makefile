build: src/*.cc
	g++ -c src/*.cc
	ar r libpyrite.a *.o

install: build
	@-mkdir /usr/include/pyrite
	@-mkdir /usr/lib/gcc/$(shell uname -m)-linux-gnu
	@-mkdir /usr/lib/gcc/$(shell uname -m)-linux-gnu/pyrite
	cp src/*.h /usr/include/pyrite/
	cp libpyrite.a /usr/lib/gcc/$(shell uname -m)-linux-gnu/pyrite/

clean:
	rm -rf *.o
	rm -rf *.a
