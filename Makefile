ifeq ($(OS), Windows_NT)
	OS = Windows
	rm = del
	cp = copy
	d  = \\
	mkdir = md
	INCLUDE_DIR = C:\include
	LIB_DIRS = C:\lib
else
	OS = Linux
	rm = rm
	force = -f
	recursion = -r
	cp = cp
	d  = /
	mkdir = mkdir
	INCLUDE_DIR = /usr/include
	LIB_DIRS = $(wildcard /usr/lib/gcc/$(shell uname -m)-linux-gnu/*)
endif

build: src/*.cc
	g++ -c src/*.cc -I$(INCLUDE_DIR) -L$(LIB_DIRS) -lmocutils -D$(OS)
	ar -crv libpyrite.a *.o

test: install
	g++ test/$(t).cc -o $(t).exe \
		-I$(INCLUDE_DIR) -L$(LIB_DIRS) -lpyrite -lmocutils  -lws2_32 \
		-D$(OS)
	./$(t).exe

install: build
	-$(mkdir) $(INCLUDE_DIR)$(d)pyrite
	$(cp) src$(d)*.h $(INCLUDE_DIR)$(d)pyrite$(d)
	-$(foreach dir, $(LIB_DIRS), $(mkdir) $(dir))
	$(foreach dir, $(LIB_DIRS), $(cp) libpyrite.a $(dir)$(d))
	@echo [ok] Installation finished.

uninstall:
	-$(rm) $(recursion) $(INCLUDE_DIR)$(d)pyrite
	-$(foreach dir, $(LIB_DIRS), $(rm) $(dir)$(d)libpyrite.a)

clean:
	$(rm) $(force) *.o
	$(rm) $(force) *.a
	$(rm) $(force) *.cc
	$(rm) $(force) *.h

copy:
	$(cp) src$(d)* .
