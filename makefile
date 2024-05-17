.SILENT:

LIB := -I lib 
CFLAGS := -Wextra -Werror -Wpedantic 

.PHONY: all
all: build run

.PHONY: build
build:
	gcc -c $(CFLAGS) src/jmake.c $(LIB) -o .temp/jmake.o 
	gcc -c $(CFLAGS) src/find.c $(LIB) -o .temp/find.o
	gcc -c $(CFLAGS) src/run.c  $(LIB) -o .temp/run.o

	gcc .temp/jmake.o .temp/find.o .temp/run.o -o .temp/jmake

.PHONY: run
run:
	.temp/jmake

.PHONY: clean
clean:
	rm -f .temp/jmake.o
	rm -f .temp/find.o
	rm -f .temp/run.o
	rm -f .temp/jmake
	rm -f .temp/test
	rm -f .temp/test.o

