all: aes build test utils
	rm -f bin/*.o

build: main.o utils
	g++ bin/utils.o bin/aes.o bin/main.o -o bin/main -O2 -Wall

test: aes test.o utils
	g++ bin/utils.o bin/aes.o bin/test.o -o bin/test -O2 -Wall

test.o:
	g++ -c -O2 -Wall src/test.cpp -o bin/test.o

main.o:
	g++ -c -O2 -Wall src/main.cpp -o bin/main.o

aes:
	g++ -c -O2 -Wall src/aes.cpp -o bin/aes.o

utils:
	g++ -c -O2 -Wall src/utils.cpp -o bin/utils.o

clean:
	rm -f bin/*.o bin/test

clean-all:
	rm -f bin/*
