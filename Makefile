ifeq ($(OS), Windows_NT)
	rm = del
	cp = copy
	d  = \\
	mkdir = md
	INCLUDE_DIR = C:\include
	LIB_DIRS = C:\lib
else
	rm = rm
	cp = cp
	d  = /
	mkdir = mkdir
	INCLUDE_DIR = /usr/include
	LIB_DIRS = $(wildcard /usr/lib/gcc/$(shell uname -m)-linux-gnu/*)
endif

build: src/*.cc
	g++ -c src/*.cc
	ar -crv libpyrite.a *.o

install: build uninstall
	-$(mkdir) $(INCLUDE_DIR)$(d)pyrite
	$(cp) src$(d)*.h $(INCLUDE_DIR)$(d)pyrite$(d)
	-$(foreach dir, $(LIB_DIRS), $(mkdir) $(dir))
	$(foreach dir, $(LIB_DIRS), $(cp) libpyrite.a $(dir)$(d))
	@echo [ok] Installation finished.

uninstall:
	-$(rm) $(INCLUDE_DIR)$(d)pyrite
	-$(foreach dir, $(LIB_DIRS), $(rm) $(dir)$(d)libpyrite.a)

clean:
	$(rm) *.o
	$(rm) *.a
	$(rm) *.cc
	$(rm) *.h

copy:
	$(cp) src$(d)* .
