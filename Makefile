CC = g++ -g

all:lab6

lab6.o: lab6.cpp
	$(CC) -c lab6.cpp

lab6: lab6.o
	$(CC) -o lab6 lab6.o

clean:
	rm -f lab6 lab6.o