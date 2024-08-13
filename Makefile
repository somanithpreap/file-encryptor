all: clean test build clean-o

build: main aes utils file-handler
	g++ bin/utils.o bin/aes.o bin/file-handler.o bin/main.o -o bin/file-encryptor -O2 -Wall
	make clean-o

test: aes utils s-test m-test
	g++ bin/utils.o bin/aes.o bin/s-test.o -o bin/s-test -O2 -Wall
	g++ bin/utils.o bin/aes.o bin/m-test.o -o bin/m-test -O2 -Wall
	make clean-o

file-handler:
	g++ -c -O2 -Wall src/file-handler.cpp -o bin/file-handler.o

s-test:
	g++ -c -O2 -Wall test/single-thread.cpp -o bin/s-test.o

m-test:
	g++ -c -O2 -Wall test/multi-thread.cpp -o bin/m-test.o

main:
	g++ -c -O2 -Wall src/main.cpp -o bin/main.o

aes:
	g++ -c -O2 -Wall src/aes.cpp -o bin/aes.o

utils:
	g++ -c -O2 -Wall src/utils.cpp -o bin/utils.o

clean-o:
	rm -f bin/*.o

clean-t:
	rm -f bin/s-test bin/m-test

clean:
	rm -f bin/*
