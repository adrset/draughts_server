CC=g++
CFLAGS=-Wall -pedantic -std=c++11
AUTHOR= Przemyslaw Nowak and Adrian Setniewski
all: pr clean
pr: main.o Entity.o ListenerServer.o
	$(CC) -o program main.o Entity.o ListenerServer.o $(CFLAGS)
main.o: main.cpp
	$(CC) -o main.o -c main.cpp $(CFLAGS)
Entity.o: Entity.cpp
	$(CC) -o Entity.o -c Entity.cpp $(CFLAGS)
ListenerServer.o: ListenerServer.cpp
	$(CC) -o ListenerServer.o -c ListenerServer.cpp $(CFLAGS)
clean:
	rm -f *.o
info:
	echo Compiler - $(CC) Flags - $(CFLAGS) Copyright $(AUTHOR)

