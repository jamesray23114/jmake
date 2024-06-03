.SILENT:

LIB := -I lib -lstdc++ 
CFLAGS := -Wall -Wpedantic -fpermissive -std=c++11
LFLAGS := -Wall -Wpedantic -fpermissive -std=c++11 -lstdc++

.PHONY: all
all: build run

.PHONY: build
build:
	gcc -c $(CFLAGS) src/jmake.cc $(LIB) -o .temp/jmake.o 
	gcc -c $(CFLAGS) src/find.cc $(LIB) -o .temp/find.o
	gcc -c $(CFLAGS) src/run.cc  $(LIB) -o .temp/run.o

	gcc $(LFLAGS) .temp/jmake.o .temp/find.o .temp/run.o -o .temp/jmake

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

