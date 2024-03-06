build: src/*.cc
	g++ -c src/*.cc
	ar r libpyrite.a *.o

install: build
	@-mkdir /usr/include/pyrite
	@-mkdir /usr/lib/pyrite
	cp src/*.h /usr/include/pyrite/
	cp pyrite.a /usr/lib/pyrite/

clean:
	rm -rf *.o
	rm -rf *.a
