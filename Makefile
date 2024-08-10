all:
	make test
	make build
	make clean-o

build: main aes utils file-handler
	g++ -std=c++11 bin/utils.o bin/aes.o bin/main.o bin/file-handler.o -o bin/main -O2 -Wall
	make clean-o

test: aes utils file-handler s-test m-test
	g++ -std=c++11 bin/utils.o bin/aes.o bin/file-handler.o bin/s-test.o -o bin/s-test -O2 -Wall
	g++ -std=c++11 bin/utils.o bin/aes.o bin/file-handler.o bin/m-test.o -o bin/m-test -O2 -Wall
	make clean-o

s-test:
	g++ -std=c++11 -c -O2 -Wall test/single-thread.cpp -o bin/s-test.o

m-test:
	g++ -std=c++11 -c -O2 -Wall test/multi-thread.cpp -o bin/m-test.o

main:
	g++ -std=c++11 -c -O2 -Wall src/main.cpp -o bin/main.o

aes:
	g++ -std=c++11 -c -O2 -Wall src/aes.cpp -o bin/aes.o

utils:
	g++ -std=c++11 -c -O2 -Wall src/utils.cpp -o bin/utils.o

file-handler:
	g++ -std=c++11 -c -O2 -Wall src/file-handler.cpp -o bin/file-handler.o

clean-o:
	rm -f bin/*.o

clean-t:
	rm -f bin/s-test bin/m-test

clean:
	rm -f bin/*
